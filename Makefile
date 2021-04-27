CC = gcc
CFLAGS = -Isrc -O2 -fPIC -pie -s
LIBS = -lconfig

TARGET = kbd-backlight

.PHONY: build install uninstall deps clean
build:
	$(CC) src/main.c $(CFLAGS) $(LIBS) -o kbd-backlight

install: build
	install -Dm755 $(TARGET) /usr/bin/$(TARGET)
	install -Dm644 backlight.conf /etc/kbd-backlight/backlight.conf
	install -Dm644 backlight.service /etc/kbd-backlight/kbd-backlight.service
	ln -s /etc/kbd-backlight/kbd-backlight.service /etc/systemd/system/kbd-backlight.service
	systemctl daemon-reload
	systemctl enable kbd-backlight
	systemctl start kbd-backlight

uninstall:
	rm /usr/bin/$(TARGET)
	systemctl disable kbd-backlight
	systemctl stop kbd-backlight
	unlink /etc/systemd/system/kbd-backlight.service
	rm -drf /etc/kbd-backlight

deps:
	@./install_deps.sh

clean:
	rm $(TARGET)
