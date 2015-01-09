#
# This makefile can be used to develop an iOS app for jailbroken devices
#

# TODO
#  - install
#       cat > output.deb; dpkg -i; rm

CONFIG ?= config
CONFIG := $(CONFIG).mk
include $(CONFIG)

#
# Project
#

ifndef APP_NAME
	$(error APP_NAME is undefined)
endif

APP_PACKAGE ?= $(APP_NAME)
APP_VERSION ?= 0
APP_AUTHOR ?= $(shell whoami)
APP_MAINTAINER ?= $(APP_AUTHOR)
APP_DEPENDS ?=
APP_DESCRIPTION ?= N/A

FILES ?= $(wildcard *.m *.mm)
ifeq ($(strip $(FILES)),)
	$(error No files to compile)
endif

ARCHS ?= armv7 arm64

FRAMEWORKS ?= UIKit CoreGraphics
FRAMEWORKS := Foundation CoreFoundation $(FRAMEWORKS)



#
# Toolchain Configuration
#

ifndef SYSROOT
	$(error SYSROOT is undefined)
endif

CCTOOLS ?= 
ifneq ($(strip $(CCTOOLS)),)
	# put a trailing slash
	CCTOOLS := $(patsubst %/,%,$(CCTOOLS))/
endif

TARGET_CC    ?= clang++ $(target) -x objective-c -std=c99
TARGET_CXX   ?= clang++ $(target) -x objective-c++
TARGET_LD    ?= $(CCTOOLS)ld
TARGET_LIPO  ?= $(CCTOOLS)lipo
TARGET_STRIP ?= $(CCTOOLS)strip
TARGET_STRIP_FLAGS ?= -x
TARGET_CODESIGN_ALLOCATE ?= $(CCTOOLS)codesign_allocate
TARGET_LDID  ?= ldid
TARGET_LDID_FLAGS ?= -S

ifneq ($(TARGET_LIPO),$(notdir $(TARGET_LIPO)))
	LIPO_PATH=PATH=$(dir $(TARGET_LIPO)):$$PATH
endif


#
# Flags
#

target = -target armv7-apple-ios7.0
archs  = $(addprefix -arch ,$(ARCHS))

common_flags = -isysroot $(SYSROOT)
ifeq ($(ENABLE_ARC),y)
	common_flags += -fobjc-arc
endif


DEPFLAGS := -isysroot $(SYSROOT) $(DEPFLAGS)

CFLAGS   ?= -Wall -Werror
CXXFLAGS ?= $(CFLAGS)

CXXFLAGS := $(archs) $(CXXFLAGS) $(common_flags)
CFLAGS   := $(archs) $(CFLAGS) $(common_flags)

ldflags := $(archs)
ldflags += -syslibroot $(SYSROOT)
ldflags += -L $(SYSROOT)/usr/lib
ldflags += $(addprefix -framework ,$(FRAMEWORKS))
ldflags += -lobjc -lSystem
ifeq ($(ENABLE_ARC),y)
ldflags += -lstdc++
endif
ifneq ($(strip $(IOS_VERSION_MIN)),)
ldflags += -ios-version-min $(IOS_VERSION_MIN)
endif
LDFLAGS := $(ldflags) $(LDFLAGS)



#
# Build Settings
#

#TOOD: signature?
BUILD_DIR ?= Build
BUILD_STAMP = $(BUILD_DIR)/.stamp

RESOURCE_DIR ?= Resources

SAY_MKDIR = echo "    [DIR] $@"
SAY_DEP   = echo "    [DEP] $@"
SAY_CC    = echo "    [CC ] $@"
SAY_CXX   = echo "    [CXX] $@"
SAY_LD    = echo "    [LD ] $@"
SAY_STRIP = echo "    [STR] $@"
SAY_SIGN  = echo "    [SIG] $@"
SAY_CLEAN = echo "    [   ] Cleaning..."



#
# DEB Packaging Settings
#

DEB_BNFILE  := $(BUILD_DIR)/buildno
DEB_BUILDNO ?=$(strip $(shell [ -f $(DEB_BNFILE) ] && cat $(DEB_BNFILE) || echo 1))
DEB_ROOT    ?= $(BUILD_DIR)/debroot
DEB_PACKAGE ?= $(APP_PACKAGE)_$(APP_VERSION)$(addprefix -,$(DEB_BUILDNO))_iphoneos-arm.deb

DEB_CONTROL := $(DEB_ROOT)/DEBIAN/control
DEB_BUNDLE  := $(DEB_ROOT)/Applications/$(APP_NAME).app



#
# Files
#

RESOURCES := $(shell find $(RESOURCE_DIR) -type f)
DEPENDS   := $(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(FILES)))
OBJECTS   := $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(FILES)))

OUTPUT    := $(BUILD_DIR)/$(APP_NAME)




.SUFFIXES:
.SUFFIXES: .m .mm .h .o .d

#
# Major Targets
#

.PHONY: config all depend package clean
config: config-package config-toolchain config-flags config-options

all: | depend $(BUILD_DIR)/$(APP_NAME)

depend: $(DEPENDS)

package: $(DEB_PACKAGE)

clean:
	@$(SAY_CLEAN)
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(BUILD_DIR)/*.d
	@rm -rf $(BUILD_DIR)/*.unsigned
	@rm -rf $(OUTPUT)
	@rm -rf $(DEB_ROOT)



#
# General Rules
#   *.d = Dependency
#   *.o = Object
#

$(BUILD_STAMP):
	@mkdir -p $(BUILD_DIR)
	@touch $@

$(BUILD_DIR)/%.m.d: %.m $(CONFIG) $(BUILD_STAMP)
	@($(SAY_DEP))
	@($(TARGET_CC) $(DEPFLAGS) -MP -MM -MT "$@ $(subst .d,.o,$@)" -o $@ $<)

$(BUILD_DIR)/%.m.o: %.m $(CONFIG) $(BUILD_STAMP)
	@($(SAY_CC))
	@($(LIPO_PATH) $(TARGET_CC) -c $(CFLAGS) -o $@ $<)

$(BUILD_DIR)/%.mm.d: %.mm $(CONFIG) $(BUILD_STAMP)
	@($(SAY_DEP))
	@($(TARGET_CXX) $(DEPFLAGS) -MP -MM -MT "$@ $(subst .d,.o,$@)" -o $@ $<)

$(BUILD_DIR)/%.mm.o: %.mm $(CONFIG) $(BUILD_STAMP)
	@($(SAY_CXX))
	@($(LIPO_PATH) $(TARGET_CXX) -c $(CFLAGS) -o $@ $<)



#
# Linking App
#

.INTERMEDIATE: $(OUTPUT).unsigned
$(OUTPUT).unsigned: $(OBJECTS)
	@($(SAY_LD))
	@($(TARGET_LD) $(LDFLAGS) -o $@ $^)
	@($(SAY_STRIP))
	@($(TARGET_STRIP) $(TARGET_STRIP_FLAGS) $@)

$(OUTPUT): $(OUTPUT).unsigned
	@($(SAY_SIGN))
	@(CODESIGN_ALLOCATE="$(TARGET_CODESIGN_ALLOCATE)" \
		$(TARGET_LDID) $(TARGET_LDID_FLAGS) $<)
	@(mv $< $@)



#
# DEB Packaging Rules
#

$(DEB_BUNDLE): $(OUTPUT) $(RESOURCES)
	@(mkdir -p $(DEB_BUNDLE))
	@(cp -u $(RESOURCES) $(DEB_BUNDLE))
	@(cp -u $(OUTPUT) $(DEB_BUNDLE))

$(DEB_CONTROL):
	@(mkdir -p $(dir $(DEB_CONTROL)))
	@(echo "Package: $(APP_PACKAGE)" >> $(DEB_CONTROL))
	@(echo "Name: $(APP_NAME)" >> $(DEB_CONTROL))
	@(echo "Version: $(APP_VERSION)" >> $(DEB_CONTROL))
	@(echo "Author: $(APP_AUTHOR)" >> $(DEB_CONTROL))
	@(echo "Depends: $(APP_DEPENDS)" >> $(DEB_CONTROL))
	@(echo "Architecture: iphoneos-arm" >> $(DEB_CONTROL))
	@(echo "Description: $(APP_DESCRIPTION)" >> $(DEB_CONTROL))
	@(echo "Maintainer: $(APP_MAINTAINER)" >> $(DEB_CONTROL))
	@(echo "Section: Utilities" >> $(DEB_CONTROL))

$(DEB_PACKAGE): $(DEB_CONTROL) $(DEB_BUNDLE)
	@(fakeroot -- dpkg-deb -Zgzip -b "$(DEB_ROOT)" "$(DEB_PACKAGE)")
	@(echo $$(($(DEB_BUILDNO) + 1)) > $(DEB_BNFILE))



#
# Print Configuration
# (For debugging)
#

.PHONY: config-package config-toolchain config-flags config-options
config-package:
	@echo "===== Application Configuration ====="
	@echo "APP_PACKAGE = $(APP_PACKAGE)"
	@echo "APP_NAME    = $(APP_NAME)"
	@echo "APP_VERSION = $(APP_VERSION)"
	@echo "APP_AUTHOR  = $(APP_AUTHOR)"
	@echo "FRAMEWORKS  = $(FRAMEWORKS)"
	@echo

config-toolchain:
	@echo "===== Toolchain Configuration ====="
	@echo "SYSROOT     = $(SYSROOT)"
	@echo "CCTOOLS     = $(CCTOOLS)"
	@echo "TARGET_CC   = $(TARGET_CC)"
	@echo "TARGET_CXX  = $(TARGET_CXX)"
	@echo "TARGET_LD   = $(TARGET_LD)"
	@echo "TARGET_LIPO = $(TARGET_LIPO)"
	@echo

config-flags:
	@echo "===== Compile Flags ====="
	@echo "DEPFLAGS = $(DEPFLAGS)"
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo

config-options:
	@echo "===== Options ====="
	@echo "ENABLE = $(ENABLE_ARC)"
	@echo "IOS_VERSION_MIN = $(IOS_VERSION_MIN)"
	@echo



# include dependency files only when they exist
ifneq ($(wildcard $(BUILD_DIR)/*.d),)
-include $(DEPENDS)
endif

