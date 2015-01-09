ifeq ($(_THEOS_TARGET_LOADED),)
_THEOS_TARGET_LOADED := 1
THEOS_TARGET_NAME := iphone

ARCH ?= armv7-apple-darwin14.0.0

SDK_ROOT ?= /tmp/iPhoneOS8.1.sdk
BIN_ROOT ?= /tmp/bin
LIB_ROOT ?= $(SDK_ROOT)/usr/lib
SYS_ROOT ?= $(SDK_ROOT)

TARGET_CC  ?= clang -target $(ARCH)
TARGET_CXX ?= clang++ -target $(ARCH)
TARGET_LD  ?= $(BIN_ROOT)/ld
TARGET_STRIP ?= $(BIN_ROOT)/strip
TARGET_STRIP_FLAGS ?= -x
TARGET_CODESIGN_ALLOCATE ?= $(BIN_ROOT)/codesign_allocate
TARGET_CODESIGN ?= ldid
TARGET_CODESIGN_FLAGS ?= -S

include $(THEOS_MAKE_PATH)/targets/_common/darwin.mk
include $(THEOS_MAKE_PATH)/targets/_common/darwin_flat_bundle.mk

TARGET_PRIVATE_FRAMEWORK_PATH = $(SYS_ROOT)/System/Library/PrivateFrameworks

_THEOS_TARGET_CFLAGS := -isysroot $(SYS_ROOT)
_THEOS_TARGET_LDFLAGS := -ios_version_min 8.0 -syslibroot $(SDK_ROOT) -L$(LIB_ROOT) -L$(LIB_ROOT) -lSystem -lstdc++

TARGET_INSTALL_REMOTE := $(_THEOS_TRUE)
_THEOS_TARGET_DEFAULT_PACKAGE_FORMAT := deb
endif
