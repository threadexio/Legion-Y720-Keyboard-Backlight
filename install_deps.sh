#!/usr/bin/sh

# Debian-based
command -v apt && sudo apt-get install -y libconfig-dev

# Arch-based
command -v pacman && sudo pacman -S --needed --noconfirm libconfig

# Fedora-based
command -v dnf && sudo dnf install -y libconfig
