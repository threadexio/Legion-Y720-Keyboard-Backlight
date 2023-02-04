#!/usr/bin/bash
set -e

get_property() {
	awk "/^$1:/{print \$NF}" "$pkg_file"
}

pkg_base_dir="/repo/packages/deb"
pkg_file="${pkg_base_dir}/DEBIAN/control"
name="$(get_property Package)"
version="${v:-$(git rev-parse --short HEAD)}"
arch="$(dpkg --print-architecture)"
fullname="${name}_${version}_${arch}"

make PREFIX="/usr" DESTDIR="/tmp/$fullname" distclean all install apparmor

cp -r "$pkg_base_dir/DEBIAN" "/tmp/$fullname"
cd "/tmp/$fullname/DEBIAN"
sed -i "s/VERSION_NUMBER/$version/g" control

install_size="$(du -bc "/tmp/$fullname" | tail -n1 | awk '{print $1}')"

sed -i "s/INSTALLED_SIZE/$install_size/g" control
mkdir -p "$pkg_base_dir/out"
cd "$pkg_base_dir/out"

dpkg-deb --build --root-owner-group "/tmp/$fullname"
mv "/tmp/$fullname.deb" .
