#!/usr/bin/bash
set -e

get_property() {
	awk "/^$1:/{print \$NF}" "$pkg_file"
}

pkg_base_dir="/repo/packages/rpm"
pkg_file="${pkg_base_dir}/package.spec"
name="$(get_property Name)"
version="${v:-$(git rev-parse --short HEAD)}"
release="$(rpm -E "$(get_property Release)")"
arch="$(arch)"
fullname="$name-$version-$release"

git archive --format "tar.gz" --prefix "$name-$version/" \
	-o "$HOME/rpmbuild/SOURCES/$name-$version.tar.gz" HEAD

cp "${pkg_file%.spec}.sysusers" \
	"$HOME/rpmbuild/SOURCES/$name.sysusers"

cd ~/rpmbuild/SPECS
cp "$pkg_file" "$name.spec"
sed -i "s/VERSION_NUMBER/$version/g" "$name.spec"

rpmbuild -ba "$name.spec"

cd ~/rpmbuild
mkdir -p "$pkg_base_dir/out"
cp "SRPMS/$fullname.src.rpm" "$pkg_base_dir/out"
cp "RPMS/$arch/$fullname.$arch.rpm" "$pkg_base_dir/out"
