#!/usr/bin/bash
set -e

export pkg_base_dir="/repo/packages/rpm"
export pkg_file="${pkg_base_dir}/package.spec"

# Get property from package file (control or spec file)
get_property() {
	grep "$1:" "$pkg_file" | awk '{print $NF}'
}


name="$(get_property Name)"

# Use version from V if its valid,
# otherwise use commit id
if [ "$v" ]; then
	version="$v"
else
	version="$(git rev-parse --short HEAD)"
fi

cd /tmp
mkdir -p "$name-$version"
# Do a bind mount so we dont cp the entire repo for no reason
# This is why we need --privileged
mount --bind "/repo" "/tmp/$name-$version"
tar -czvf "$HOME/rpmbuild/SOURCES/$name-$version.tar.gz" -- "$name-$version"

cd ~/rpmbuild/SPECS
cp "$pkg_file" "$name.spec"
sed -i "s/VERSION_NUMBER/$version/g" "$name.spec"

# Build source and binary rpms
rpmbuild -ba "$name.spec"

arch="$(arch)"
release="$(rpm --eval "$(get_property Release)")"
fullname="$name-$version-$release"

cd ~/rpmbuild

mkdir -p "$pkg_base_dir/out"
cp "SRPMS/$fullname.src.rpm" "$pkg_base_dir/out"
cp "RPMS/$arch/$fullname.$arch.rpm" "$pkg_base_dir/out"
