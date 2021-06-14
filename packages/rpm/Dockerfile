FROM fedora

RUN dnf -y update
RUN dnf -y install rpm-build rpm-devel rpmlint rpmdevtools \
	make gcc libconfig libconfig-devel git

RUN rpmdev-setuptree

RUN mkdir -p /repo

WORKDIR /repo

COPY entry.sh /entry.sh

ENTRYPOINT [ "bash", "/entry.sh" ]