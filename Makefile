CC = gcc
CFLAGS = -Isrc -O2 -fPIC -pie -s
LIBS = -lconfig

CFG_DIR = /etc/kbd-backlight

SOURCE = $(wildcard src/*.c)
TARGET = kbd-backlight

.PHONY: build
build:
	$(CC) $(SOURCE) $(CFLAGS) $(LIBS) -o kbd-backlight

.PHONY: install
install: build
	# Install the main binary
	install -Dm755 $(TARGET) /usr/bin/$(TARGET)
	
	# Setup the config directory
	mkdir -p $(CFG_DIR)
	cp -r files/* /etc/kbd-backlight
	ln $(CFG_DIR)/backlight.service /etc/systemd/system/kbd-backlight.service
	systemctl daemon-reload
	systemctl enable kbd-backlight
	systemctl start kbd-backlight

.PHONY: uninstall
uninstall:
	-rm /usr/bin/$(TARGET)
	-systemctl disable kbd-backlight
	-rm /etc/systemd/system/kbd-backlight.service
	-systemctl daemon-reload
	-rm -drf $(CFG_DIR)

.PHONY: clean
clean:
	-rm -f $(TARGET)
