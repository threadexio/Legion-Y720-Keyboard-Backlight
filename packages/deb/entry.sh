#!/usr/bin/bash
set -e

export pkg_base_dir="/repo/packages/deb"
export pkg_file="${pkg_base_dir}/DEBIAN/control"

# Get property from package file (control or spec file)
get_property() {
	grep "$1:" "$pkg_file" | awk '{print $NF}'
}

arch="$(dpkg --print-architecture)"
name="$(get_property Package)"

# Use version from V if its valid,
# otherwise use commit id
if [ "$v" ]; then
	version="$v"
else
	version="$(git rev-parse --short HEAD)"
fi

fullname="${name}_${version}_${arch}"

make PREFIX="/usr" DESTDIR="/tmp/$fullname" distclean all install apparmor

cp -r "$pkg_base_dir/DEBIAN" "/tmp/$fullname"
cd "/tmp/$fullname/DEBIAN" || exit
sed -i "s/VERSION_NUMBER/$version/g" control
sed -i "s/INSTALLED_SIZE/$install_size/g" control

install_size="$(du -bc "/tmp/$fullname" | tail -n1 | awk '{print $1}')"

mkdir -p "$pkg_base_dir/out"
cd "$pkg_base_dir/out"

dpkg-deb --build --root-owner-group "/tmp/$fullname"
mv "/tmp/$fullname.deb" .