Name: kbd-backlight
Version: VERSION_NUMBER
Release: 1%{?dist}
Summary: Keyboard backlight control for Lenovo Y720 laptop
License: MIT
URL: https://github.com/threadexio/Legion-Y720-Keyboard-Backlight
Source0: %{name}-%{version}.tar.gz
Source1: %{name}.sysusers
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: libconfig-devel
BuildRequires: systemd-rpm-macros
Requires: libconfig

%global source_date_epoch_from_changelog 0

%description
A simple C program to control the keyboard backlight on the Lenovo Y720 laptop.

%prep
%autosetup

%build
%make_build all

%install
%make_install PREFIX="/usr" distclean all install
install -p -D -m 0644 %{SOURCE1} %{buildroot}%{_sysusersdir}/%{name}.conf

%pre
%sysusers_create_compat %{SOURCE1}

%files
%license LICENSE
%dir %{_sysconfdir}/%{name}
%config(noreplace) %attr(0664,-,%{name}) %{_sysconfdir}/%{name}/backlight.conf
%caps(cap_dac_override+eip) %{_bindir}/%{name}
%{_datadir}/%{name}
%{_sysusersdir}/%{name}.conf
