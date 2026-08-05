import argparse
import time
from urllib.parse import urlparse
from urllib.request import urlopen
from pathlib import Path

TEMP_EXTENSIONS = (".crdownload", ".tmp", ".part")


def has_temp_files(download_dir: Path) -> bool:
    return any(p.is_file() and p.suffix.lower() in TEMP_EXTENSIONS for p in download_dir.iterdir())


def newest_mp3(download_dir: Path):
    mp3s = [p for p in download_dir.iterdir() if p.is_file() and p.suffix.lower() == ".mp3"]
    if not mp3s:
        return None
    return max(mp3s, key=lambda p: p.stat().st_mtime)


def wait_for_download(download_dir: Path, before_count: int, timeout: int = 600):
    start = time.time()
    last_size = -1
    stable_ticks = 0

    while time.time() - start < timeout:
        # wait until at least one new mp3 appears
        mp3s = [p for p in download_dir.iterdir() if p.is_file() and p.suffix.lower() == ".mp3"]
        if len(mp3s) <= before_count:
            time.sleep(1)
            continue

        # wait until browser temp files disappear
        if has_temp_files(download_dir):
            time.sleep(1)
            continue

        # ensure newest file size is stable for a few seconds
        newest = newest_mp3(download_dir)
        if newest is None:
            time.sleep(1)
            continue

        size = newest.stat().st_size
        if size == last_size:
            stable_ticks += 1
        else:
            stable_ticks = 0
            last_size = size

        if stable_ticks >= 3:
            return newest

        time.sleep(1)

    raise TimeoutError("Download did not finish in time.")


def filename_from_url(url: str, index: int) -> str:
    parsed = urlparse(url)
    name = Path(parsed.path).name
    if not name:
        return f"download_{index:03d}.mp3"
    return name


def download_one_file(url: str, output_path: Path, timeout: int) -> None:
    temp_path = output_path.with_suffix(output_path.suffix + ".part")
    if temp_path.exists():
        temp_path.unlink()

    with urlopen(url, timeout=timeout) as response, open(temp_path, "wb") as target:
        while True:
            chunk = response.read(64 * 1024)
            if not chunk:
                break
            target.write(chunk)

    temp_path.replace(output_path)


def run_direct(urls, download_dir: Path, timeout: int, skip_existing: bool):
    for i, url in enumerate(urls, 1):
        file_name = filename_from_url(url, i)
        target = download_dir / file_name
        print(f"[{i}/{len(urls)}] Downloading: {url}")

        if skip_existing and target.exists() and target.stat().st_size > 0:
            print(f"    Skipped: {target.name} already exists")
            continue

        download_one_file(url, target, timeout)
        print(f"    Done: {target.name}")


def run_edge(urls, download_dir: Path, timeout: int):
    from selenium import webdriver
    from selenium.webdriver.edge.options import Options

    opts = Options()
    opts.add_experimental_option("prefs", {
        "download.default_directory": str(download_dir),
        "download.prompt_for_download": False,
        "download.directory_upgrade": True,
        "safebrowsing.enabled": True,
    })

    # Selenium Manager usually auto-downloads msedgedriver.
    driver = webdriver.Edge(options=opts)

    try:
        for i, url in enumerate(urls, 1):
            before_count = len([p for p in download_dir.iterdir() if p.is_file() and p.suffix.lower() == ".mp3"])
            print(f"[{i}/{len(urls)}] Opening: {url}")
            driver.get(url)

            finished_file = wait_for_download(download_dir, before_count, timeout=timeout)
            print(f"    Done: {finished_file.name}")
    finally:
        driver.quit()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--links-file", required=True, help="Text file with one URL per line")
    parser.add_argument("--download-dir", required=True, help="Folder for downloads")
    parser.add_argument("--timeout", type=int, default=600, help="Seconds per file")
    parser.add_argument(
        "--mode",
        choices=("auto", "edge", "direct"),
        default="auto",
        help="Download mode: auto (default), edge (Selenium), or direct (HTTP).",
    )
    parser.add_argument(
        "--skip-existing",
        action="store_true",
        help="Skip files that already exist and are non-empty.",
    )
    args = parser.parse_args()

    links_file = Path(args.links_file).expanduser().resolve()
    download_dir = Path(args.download_dir).expanduser().resolve()
    download_dir.mkdir(parents=True, exist_ok=True)

    urls = [line.strip() for line in links_file.read_text(encoding="utf-8").splitlines() if line.strip() and not line.startswith("#")]
    if not urls:
        print("No URLs found.")
        return

    if args.mode == "direct":
        run_direct(urls, download_dir, args.timeout, args.skip_existing)
        return

    if args.mode == "edge":
        run_edge(urls, download_dir, args.timeout)
        return

    # auto mode: try Edge first, then fall back to direct download.
    try:
        run_edge(urls, download_dir, args.timeout)
    except Exception as err:
        print(f"Edge WebDriver failed ({err}). Falling back to direct mode.")
        run_direct(urls, download_dir, args.timeout, args.skip_existing)


if __name__ == "__main__":
    main()