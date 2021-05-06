CC := gcc

RLS_CFLAGS := -Wno-unused-result -O2 -fPIC -pie
DBG_CFLAGS := -Wall -D_DEBUG -g

LFLAGS := -lconfig

TARGET := kbd-backlight

CFG_DIR := /etc/kbd-backlight
SRC_DIR := src
BUILD_DIR := build
RLS_DIR := $(BUILD_DIR)/release
DBG_DIR := $(BUILD_DIR)/debug

SOURCE := $(wildcard $(SRC_DIR)/*.c)

DBG_OBJ := $(patsubst $(SRC_DIR)/%.c,$(DBG_DIR)/%.o,$(SOURCE))
RLS_OBJ := $(patsubst $(SRC_DIR)/%.c,$(RLS_DIR)/%.o,$(SOURCE))

.PHONY: release
release: $(RLS_OBJ)
	$(CC) $^ $(LFLAGS) -o $(TARGET)
	strip $(TARGET)

$(RLS_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(RLS_CFLAGS) -c $< -o $@

.PHONY: debug
debug: $(DBG_OBJ)
	$(CC) $^ $(LFLAGS) -o $(TARGET)

$(DBG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DBG_CFLAGS) -c $< -o $@

.PHONY: install
install: release
	# Install the main binary
	install -Dm755 $(TARGET) /usr/bin/$(TARGET)
	
	# Setup the config directory
	mkdir -p $(CFG_DIR)
	cp -r files/* $(CFG_DIR)
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
	-rm -rf $(TARGET) $(DBG_DIR)/*.o $(RLS_DIR)/*.o
