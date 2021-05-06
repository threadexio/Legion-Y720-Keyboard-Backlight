#!/usr/bin/bash

export arch="$(dpkg --print-architecture)"

export pkg
export rls
if [ "$version" ]; then
	rls="$version"
	pkg="kbd-backlight_${version}_${arch}"
else
	commit_id="$(git rev-parse --short HEAD)"
	rls="$commit_id"
	pkg="kbd-backlight_${commit_id}_${arch}"
fi

mkdir -p "/$pkg/DEBIAN"

make clean
make DESTDIR="/$pkg" install

export install_size="$(du -bc /$pkg | tail -n1 | awk '{print $1}')"

cd "/$pkg/DEBIAN"
cp /repo/packages/deb/control .
sed -i "s/VERSION_NUMBER/$rls/g" control
sed -i "s/INSTALLED_SIZE/$install_size/g" control

cd /repo/packages/deb/out
dpkg-deb --build --root-owner-group "/$pkg"
mv "/${pkg}.deb" .

