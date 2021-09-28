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
/usr/bin/make

%install
%make_install PREFIX="/usr" distclean all install

%post
/sbin/groupadd -r %name
/sbin/setcap cap_dac_override+eip /usr/bin/kbd-backlight

%postun
/sbin/groupdel %name

%files
%license LICENSE
/usr/bin/kbd-backlight
/etc/kbd-backlight/backlight.conf
/usr/share/kbd-backlight/backlight.conf.default