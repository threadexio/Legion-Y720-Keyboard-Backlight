Name:           kbd-backlight
Version:        VERSION_NUMBER
Release:        1%{?dist}
Summary:        Keyboard Backlight for the Legion Y720

License:        MIT
URL:            https://github.com/threadexio/Legion-Y720-Keyboard-Backlight
Source0:        %{name}-VERSION_NUMBER.tar.gz

BuildRequires:  gcc make libconfig-devel
Requires:		libconfig

%description
Simple C program to control the keyboard backlight on the Lenovo Legion Y720 

%global debug_package %{nil}

%prep
%setup -q

%build
make

%install
%make_install

%files
%license LICENSE
/usr/bin/kbd-backlight
/etc/kbd-backlight/backlight.conf
/usr/lib/systemd/system/kbd-backlight.service
