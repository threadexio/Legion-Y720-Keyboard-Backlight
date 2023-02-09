FROM fedora

RUN dnf -y update
RUN dnf -y install rpm-build rpmlint \
	rpmdevtools systemd-rpm-macros gcc \
	make cmake libconfig-devel git

RUN rpmdev-setuptree

RUN mkdir -p /repo
RUN git config --global --add safe.directory /repo

WORKDIR /repo

COPY entry.sh /entry.sh

ENTRYPOINT [ "bash", "/entry.sh" ]
