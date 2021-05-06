FROM debian

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get install --no-install-recommends -y libc-dev gcc make libconfig-dev git

RUN mkdir -p /repo

WORKDIR /repo

COPY entry.sh /entry.sh

ENTRYPOINT [ "bash", "/entry.sh" ]