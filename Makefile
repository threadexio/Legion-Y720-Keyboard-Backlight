CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc

RLS_CFLAGS := -Wall -O3 -fPIC -fPIE
DBG_CFLAGS := -Wall -D_DEBUG -g

LFLAGS := -lconfig

TARGET := kbd-backlight

GROUP := kbd-backlight
CFG_DIR := /etc/kbd-backlight
SRV_PATH := /usr/lib/systemd/system

SRC_DIR := src
BUILD_DIR := build
RLS_DIR := $(BUILD_DIR)/release
DBG_DIR := $(BUILD_DIR)/debug

SOURCE := $(wildcard $(SRC_DIR)/*.c)

DBG_OBJ := $(patsubst $(SRC_DIR)/%.c,$(DBG_DIR)/%.o,$(SOURCE))
RLS_OBJ := $(patsubst $(SRC_DIR)/%.c,$(RLS_DIR)/%.o,$(SOURCE))

.PHONY: all
all: release

.PHONY: release
release: $(RLS_OBJ)
	$(CC) $^ $(LFLAGS) -o $(RLS_DIR)/$(TARGET)
	strip $(RLS_DIR)/$(TARGET)

$(RLS_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(RLS_CFLAGS) -c $< -o $@

.PHONY: debug
debug: $(DBG_OBJ)
	$(CC) $^ $(LFLAGS) -o $(DBG_DIR)/$(TARGET)

$(DBG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DBG_CFLAGS) -c $< -o $@

.PHONY: install
install: all
	# Install the main binary
	install -Dm0755 $(RLS_DIR)/$(TARGET) $(DESTDIR)/usr/local/bin/$(TARGET)
	
	# Yes, this is probably the worst way to allow control without root perms
	# if you find any other way please open an issue or a pull request
	setcap cap_dac_override+eip /usr/local/bin/kbd-backlight
	
	install -Dm0644 files/backlight.conf $(DESTDIR)$(CFG_DIR)/backlight.conf
	install -Dm0644 files/backlight.service $(DESTDIR)$(SRV_PATH)/kbd-backlight.service
	groupadd $(GROUP)
	-systemctl daemon-reload
	-systemctl enable kbd-backlight
	-systemctl start kbd-backlight

.PHONY: uninstall
uninstall:
	-rm $(DESTDIR)/usr/local/bin/$(TARGET)
	-systemctl stop kbd-backlight
	-systemctl disable kbd-backlight
	-rm $(DESTDIR)$(SRV_PATH)/kbd-backlight.service
	-systemctl daemon-reload
	-rm -drf $(DESTDIR)$(CFG_DIR)
	-groupdel $(GROUP)

.PHONY: test
test:
	# define custom actions here

.PHONY: clean
clean:
	-rm -f $(DBG_DIR)/$(TARGET)* $(RLS_DIR)/$(TARGET)* $(DBG_DIR)/*.o $(RLS_DIR)/*.o

.PHONY: setup_env
setup_env:
	-mkdir -p $(SRC_DIR) $(RLS_DIR) $(DBG_DIR)
	-touch $(RLS_DIR)/.placeholder $(DBG_DIR)/.placeholder

# Build the deb package
# make V="x.x.x" deb
.PHONY: deb
deb:
	cd packages/deb && \
	docker build -t kbd-backlight-deb .
	docker run --rm -it \
		-v $$PWD:/repo \
		-e v=$(V) \
		kbd-backlight-deb

# Build the rpm src and bin packages
# make V="x.x.x" rpm
# NOTE: We use --privileged because otherwise docker
#		wont let us do bind mounts inside the container
#		This is just so we dont cp the entire repo for no reason
.PHONY: rpm
rpm:
	cd packages/rpm && \
	docker build -t kbd-backlight-rpm .
	docker run --rm -it \
		--privileged \
		-v $$PWD:/repo \
		-e v=$(V) \
		kbd-backlight-rpm

# Clean docker images used for building packages
.PHONY: clean_docker
clean_docker:
	-docker rmi kbd-backlight-deb kbd-backlight-rpm

.PHONY: clean_pkg
clean_pkg:
	-rm -f packages/deb/out/*.deb packages/rpm/out/*.rpm
