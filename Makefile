# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_linux
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_linux)
  GLFW_config = debug_linux
  GLEW_config = debug_linux
  Application_config = debug_linux

else ifeq ($(config),release_linux)
  GLFW_config = release_linux
  GLEW_config = release_linux
  Application_config = release_linux

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := GLFW GLEW Application

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

GLFW:
ifneq (,$(GLFW_config))
	@echo "==== Building GLFW ($(GLFW_config)) ===="
	@${MAKE} --no-print-directory -C . -f GLFW.make config=$(GLFW_config)
endif

GLEW:
ifneq (,$(GLEW_config))
	@echo "==== Building GLEW ($(GLEW_config)) ===="
	@${MAKE} --no-print-directory -C . -f GLEW.make config=$(GLEW_config)
endif

Application: GLFW GLEW
ifneq (,$(Application_config))
	@echo "==== Building Application ($(Application_config)) ===="
	@${MAKE} --no-print-directory -C . -f Application.make config=$(Application_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f GLFW.make clean
	@${MAKE} --no-print-directory -C . -f GLEW.make clean
	@${MAKE} --no-print-directory -C . -f Application.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_linux"
	@echo "  release_linux"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   GLFW"
	@echo "   GLEW"
	@echo "   Application"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"