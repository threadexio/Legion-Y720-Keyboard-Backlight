# vim:syntax=apparmor

include <tunables/global>

"/usr/local/bin/kbd-backlight" {
  include <abstractions/base>

  capability dac_override,

  # Config files
  /etc/kbd-backlight/** r,
  /@{HOME}/.config/kbd-backlight.conf r,

  # HID devices
  /sys/** r,
  /dev/hidraw* w,

  /etc/passwd r,
  /etc/group r,
}