# Simple Debian packaging example

This directory now contains a minimal Debian packaging skeleton for the existing C++ example.

## What the control file does

The file `debian/control` describes:

- the source package (`Source`)
- how to build it (`Build-Depends`)
- the binary package that will be produced (`Package`)
- package metadata such as section, architecture, dependencies, and description

## Minimal files in `debian/`

- `control`: package metadata
- `changelog`: version and release history
- `rules`: build instructions used by `dpkg-buildpackage`
- `source/format`: source package format

## This example control file

```debcontrol
Source: using-control-file
Section: utils
Priority: optional
Maintainer: Demo User <demo@example.com>
Standards-Version: 4.6.2
Build-Depends: debhelper-compat (= 13), g++
Rules-Requires-Root: no

Package: using-control-file
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}
Description: simple Debian package example for a C++ program
 This package is a minimal example used to explain the purpose of
 the debian/control file and the basic Debian packaging layout.
```

## Build the package

From this directory run:

```bash
dpkg-buildpackage -us -uc -b
```

What happens:

1. Debian tools read `debian/control` to know build dependencies and package metadata.
2. `debian/rules` runs the build.
3. The built executable is installed into the package staging directory.
4. A `.deb` file is produced in the parent directory.

## Result

After a successful build, you should see a file similar to:

```bash
../using-control-file_0.1-1_amd64.deb
```

## Install the package locally

```bash
sudo dpkg -i ../using-control-file_0.1-1_amd64.deb
```

Then run:

```bash
using-control-file
```