import os
import subprocess
import sys
import datetime
from pathlib import Path
import re
from collections import defaultdict

def run(cmd, cwd=None, ignore_error=False):
    print(f"Running: {cmd}")
    result = subprocess.run(cmd, shell=True, cwd=cwd)
    if result.returncode != 0:
        print(f"Command failed: {cmd}")
        if not ignore_error:
            sys.exit(1)

def parse_build_depends(control_path):
    build_deps = set()
    if not control_path.exists():
        return build_deps
    with open(control_path) as f:
        content = f.read()
    match = re.search(r'(Build-Depends:\s*(.*?))(\n\S|\n\n|$)', content, re.DOTALL)
    if match:
        paragraph = match.group(1)
        print(f"\nBuild-Depends paragraph in {control_path}:")
        print(paragraph.strip())
        deps_line = paragraph[len('Build-Depends:'):].strip()
        deps_line = deps_line.split('\n\n')[0]
        for dep in deps_line.split(','):
            dep = dep.strip()
            if dep:
                dep = re.split(r'\s*\(|\[|<|>', dep)[0].strip()
                build_deps.add(dep)
    return build_deps


def build_package(pkg_path, schroot_name):
    print(f"\n=== Building package in {pkg_path} ===")
    # Step 1: Build dependencies
    run(f"nsg-schroot build-deps {schroot_name} {pkg_path}")
    # Step 2: Build the package inside chroot
    run(f"nsg-schroot-run -uroot {schroot_name} -- sh -c 'cd {pkg_path} && dpkg-buildpackage -us -uc -b -j16'")

def move_debs(pkg_path, debian_dir):
    # Find .deb files in parent directory
    debs = list(pkg_path.parent.glob("*.deb"))
    if not debs:
        print(f"No .deb files found after building {pkg_path.name}")
        sys.exit(1)
    # Move .deb files to debian_dir
    for deb in debs:
        target = debian_dir / deb.name
        print(f"Moving {deb} to {target}")
        deb.replace(target)

def install_and_configure_debs(debian_dir, schroot_name, max_attempts=5):
    # Install .deb files in chroot from debian_dir
    debs_in_dir = list(debian_dir.glob("*.deb"))
    deb_files = ' '.join(str(deb) for deb in debs_in_dir)
    print(f"Installing: {deb_files}")
    for attempt in range(1, max_attempts + 1):
        print(f"Install/configure attempt {attempt}...")
        run(f"nsg-schroot-run -uroot {schroot_name} -- sh -c 'cd {debian_dir} && dpkg -i --force-overwrite {deb_files}'", ignore_error=True)
        run(f"nsg-schroot-run -uroot {schroot_name} -- sh -c 'apt-get install -f -y'", ignore_error=True)
        # Check for unconfigured packages
        result = subprocess.run(f"nsg-schroot-run -uroot {schroot_name} -- sh -c 'dpkg --audit'", shell=True, capture_output=True, text=True)
        if not result.stdout.strip():
            print(f"All packages configured after {attempt} attempt(s).")
            break
        else:
            print(f"Unconfigured packages remain after attempt {attempt}:")
            print(result.stdout)
    else:
        print("Warning: Some packages may still be unconfigured after 5 attempts.")

def build_and_install(packages, schroot_name="nsg-schroot"):
    timestamp = datetime.datetime.now().strftime("%d%m%H%M%S")
    debian_dir = Path.cwd() / f"debian_packages_{timestamp}"
    debian_dir.mkdir(exist_ok=True)
    print(f"Build order: {packages}")
    for pkg_dir in packages:
        pkg_path = Path.cwd() / pkg_dir
        if not pkg_path.is_dir():
            print(f"Package directory not found: {pkg_path}")
            sys.exit(1)
        build_package(pkg_path, schroot_name)
        move_debs(pkg_path, debian_dir)
        install_and_configure_debs(debian_dir, schroot_name)
        print(f"=== Done with {pkg_path.name} ===\n")
        import time
        time.sleep(10)
    print(f"All packages built and installed in schroot {schroot_name}. Debian packages are in {debian_dir}")

def main():
    if len(sys.argv) < 2:
        print("Usage: python build_and_install_ordered.py <pkg1> <pkg2> ...")
        sys.exit(1)
    schroot_name = input("Enter the schroot name: ").strip()
    packages = sys.argv[1:]
    build_and_install(packages, schroot_name)

if __name__ == "__main__":
    main()
