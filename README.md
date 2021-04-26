# Legion-Y720-Keyboard-Backlight
![GitHub](https://img.shields.io/github/license/threadexio/Legion-Y720-Keyboard-Backlight?style=for-the-badge)
![Travis (.org)](https://img.shields.io/travis/threadexio/Legion-Y720-Keyboard-Backlight?style=for-the-badge)

Simple C program to control the keyboard backlight on the Lenovo Legion Y720 (should probably work on other laptops with the same keyboard)

It's literally [this](https://github.com/Izurii/Lenovo-Y720-KB-Led-Controller) but written in C without a GUI for the minimalists and purists


## Installation
--------
```bash
cd /opt
# Or any other directory of your choice

git clone https://github.com/threadexio/Legion-Y720-Keyboard-Backlight

sudo make deps
# This will automagically install the one dependency

sudo make install
# This will build and install the binary in /usr/bin and it's config file in /etc/kbd-backlight/backlight.conf

# Now you can edit the configuration file at /etc/kbd-backlight/backlight.conf and add as many profiles as you want
```

## Usage
--------
```bash
Usage: kbd-backlight [profile name] {config file}
```

`profile name`: Name of the profile to use
```bash
profiles = {
	// The format must be strictly followed
	// you have to specify brightness and mode
	// for every section due to the limitations
	// of the current parsing method
	example = (
		/\          {
	profile name        mode = "WAVE";
		                brightness = "HIGH";
		                color = "CRIMSON";
		            },
                    ...
```

`config file`: Path to an alternate location for the config