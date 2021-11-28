# Aparently bullseye has a problem and can't link libconfig
FROM debian

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get install --no-install-recommends -y build-essential libc-dev gcc make cmake libconfig-dev git libcap2-bin

RUN mkdir -p /repo

WORKDIR /repo

COPY entry.sh /entry.sh

ENTRYPOINT [ "bash", "/entry.sh" ]