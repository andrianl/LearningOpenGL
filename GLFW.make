# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_linux
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
INCLUDES += -IGLFW/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += -lpthread -ldl -lX11 -lGL
LDDEPS +=
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_linux)
TARGETDIR = Build/Debug/Linux
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = Intermediate/Debug/Linux/Linux/Debug/GLFW
DEFINES += -DGLEW_STATIC -DPLATFORM_DESKTOP -DLINUX -DDEBUG -D_GLFW_X11
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -g -std=c11 -Wall -Wextra -Wpedantic -Werror
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -g -Wall -Wextra -Wpedantic -Werror

else ifeq ($(config),release_linux)
TARGETDIR = Build/Release/Linux
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = Intermediate/Release/Linux/Linux/Release/GLFW
DEFINES += -DGLEW_STATIC -DPLATFORM_DESKTOP -DLINUX -DRELEASE -D_GLFW_X11
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -std=c11 -Wall -Wextra -Wpedantic -Werror
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -Wall -Wextra -Wpedantic -Werror

endif

# Per File Configurations
# #############################################


# File sets
# #############################################


# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking GLFW
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning GLFW
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif