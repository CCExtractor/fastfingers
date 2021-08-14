# Building FastFingers From Source

FastFingers releases Debian, Arch, and RPM packages in every release. You can download the proper package for your Linux
system from releases and install them with package managers.

## Install the package for Ubuntu/Debian

```bash
sudo dpkg -i fastfingers*.deb
sudo apt-get install -f
```

## Install the package for Fedora/CentOS

```bash
sudo dnf install fastfingers/packages/rpm/fastfingers*.rpm
```

## Install the package for Arch/Manjaro

```bash
sudo pacman -U fastfingers*.pkg.tar.zst
```