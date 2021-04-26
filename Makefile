CC = gcc
CFLAGS = -Wall -Werror -Isrc -O2 -pie -s
LIBS = -lconfig

TARGET = kbd-backlight

.PHONY: build install uninstall deps clean
build:
	$(CC) $(CFLAGS) $(LIBS) -o kbd-backlight src/main.c

install: build
	install -Dm755 $(TARGET) /usr/bin/$(TARGET)
	install -Dm644 src/backlight.conf /etc/kbd-backlight/backlight.conf

uninstall:
	rm /usr/bin/$(TARGET)
	rm -drf /etc/kbd-backlight

deps:
	@./install_deps.sh

clean:
	rm $(TARGET)
