<div align="center">

<h2>Legion-Y720-Keyboard-Backlight</h2>

<hr>

[![GitHub](https://img.shields.io/github/license/threadexio/Legion-Y720-Keyboard-Backlight?style=for-the-badge)](https://github.com/threadexio/Legion-Y720-Keyboard-Backlight/blob/master/LICENSE)
[![Travis (.com)](https://img.shields.io/travis/com/threadexio/Legion-Y720-Keyboard-Backlight?style=for-the-badge)](https://travis-ci.com/github/threadexio/Legion-Y720-Keyboard-Backlight)
[![Last commit](https://img.shields.io/github/last-commit/threadexio/Legion-Y720-Keyboard-Backlight/master?style=for-the-badge)](https://github.com/threadexio/Legion-Y720-Keyboard-Backlight/commits/master)
[![Commits since latest](https://img.shields.io/github/commits-since/threadexio/Legion-Y720-Keyboard-Backlight/latest?style=for-the-badge)](https://github.com/threadexio/Legion-Y720-Keyboard-Backlight/commits/master)
[![Downloads](https://img.shields.io/github/downloads/threadexio/Legion-Y720-Keyboard-Backlight/total?style=for-the-badge)](https://github.com/threadexio/Legion-Y720-Keyboard-Backlight/releases/latest)

<hr>

</div>

Simple C program to control the keyboard backlight on the Lenovo Legion Y720 (should probably work on other laptops with the same keyboard)

It's literally [this](https://github.com/Izurii/Lenovo-Y720-KB-Led-Controller) but written in C without a GUI for the minimalists and purists

## Features
<hr>

- Unlimited profiles
- Easy-ish configuration
- No GUI (terminal ftw)
- Written in C

## ➡ 🏗️ Build
<hr>

```bash
$ make
...
```
- Binary can be found in `build/release/kbd-backlight`

## ➡ ✨ Install
<hr>

```bash
$ sudo make install
...
```
- Binary path is `/usr/local/bin/kbd-backlight`
- Configuration path is `/etc/kbd-backlight/backlight.conf`


## Usage
--------
```bash
$ kbd-backlight

Usage: kbd-backlight [profile name] {config file}
```

`{config file}`: Specify another location for the config file (Optional)