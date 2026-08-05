# Sequential MP3 Downloader Guide

This folder contains a script that downloads links one by one and waits for each file to finish before starting the next.

- Script: `download_links.py`
- Links list: `links_file.txt` (one URL per line)

## What It Does

- Reads URLs from a text file.
- Downloads sequentially (not in parallel).
- Supports three modes:
  - `direct`: HTTP download (recommended, no browser dependency)
  - `edge`: Microsoft Edge + Selenium
  - `auto`: try Edge first, then fallback to direct mode
- Optional resume behavior with `--skip-existing`.

## Windows Setup (Another PC)

1. Install Python 3.10+.
2. Copy these files to the target PC:
   - `download_links.py`
   - `links_file.txt`
3. Open PowerShell in the folder.

## Recommended Run Command (Windows)

Use direct mode for reliability:

```powershell
python download_links.py --mode direct --links-file links_file.txt --download-dir "C:\Users\YOUR_USER\Desktop\quran" --skip-existing
```

If `python` is not recognized, use:

```powershell
py download_links.py --mode direct --links-file links_file.txt --download-dir "C:\Users\YOUR_USER\Desktop\quran" --skip-existing
```

## Edge Mode (Optional)

Use this only if you specifically want browser-driven downloads.

1. Install Microsoft Edge.
2. Install Selenium:

```powershell
python -m pip install selenium
```

3. Run:

```powershell
python download_links.py --mode edge --links-file links_file.txt --download-dir "C:\Users\YOUR_USER\Desktop\quran" --skip-existing
```

## Auto Mode

Auto mode tries Edge first, then falls back to direct mode if Edge/WebDriver fails.

```powershell
python download_links.py --mode auto --links-file links_file.txt --download-dir "C:\Users\YOUR_USER\Desktop\quran" --skip-existing
```

## Useful Flags

- `--timeout 600`: per-file timeout in seconds (default `600`).
- `--skip-existing`: skip files already present and non-empty.

Example:

```powershell
python download_links.py --mode direct --links-file links_file.txt --download-dir "C:\Users\YOUR_USER\Desktop\quran" --timeout 900 --skip-existing
```

## Links File Format

`links_file.txt` must contain one URL per line. Example:

```text
https://server10.mp3quran.net/download/minsh/001.mp3
https://server10.mp3quran.net/download/minsh/002.mp3
https://server10.mp3quran.net/download/minsh/003.mp3
```

## Troubleshooting

- Error about Selenium or Edge driver:
  - Use `--mode direct`.
- Permission issues on download folder:
  - Choose a writable path, such as inside your user profile.
- Interrupted run:
  - Re-run with `--skip-existing`.
