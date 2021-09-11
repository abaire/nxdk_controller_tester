XBE_TITLE = XBControllerTester

DEBUG = y
V = 1

SRCS += \
	$(wildcard $(CURDIR)/*.cpp) \
	$(CURDIR)/third_party/SDL_FontCache/SDL_FontCache.c

NXDK_DIR ?= $(CURDIR)/../nxdk
NXDK_SDL = y
NXDK_CXX = y
NXDK_NET = y
NXDK_DISABLE_AUTOMOUNT_D = n

GEN_XISO = ${XBE_TITLE}.iso

CXXFLAGS += -I$(CURDIR) -I$(CURDIR)/third_party -Wall -Wextra -std=gnu++17
CFLAGS   += -std=gnu17

ifeq ($(DEBUG),y)
CFLAGS += -DDEBUG -D_DEBUG
CXXFLAGS += -DDEBUG -D_DEBUG
endif

include $(NXDK_DIR)/Makefile

RESOURCES = \
	$(OUTPUT_DIR)/resources/fonts/X360.ttf

TARGET += $(RESOURCES)
$(GEN_XISO): $(RESOURCES)

$(OUTPUT_DIR)/resources/%: $(CURDIR)/resources/% $(OUTPUT_DIR)
	$(VE)mkdir -p '$(dir $@)'
	$(VE)cp '$<' '$@'
