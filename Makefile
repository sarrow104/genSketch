.PHONY: all release debug clean install clean-debug clean-release

CMAKE_FLAGS= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

ifeq ($(OS),Windows_NT)
	ifeq ($(TERM),xterm)
	    CMAKE_FLAGS= -G"MSYS Makefiles"
    endif
	ifeq ($(TERM),cygwin)
	    CMAKE_FLAGS= -G"MSYS Makefiles"
    endif
endif

all: release
release:
	@mkdir -p Release
	cd Release && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/bin .. && make
	ln -s -f Release/compile_commands.json

debug:
	@mkdir -p Debug
	@cd Debug && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Debug .. && make
	ln -s -f Debug/compile_commands.json

install:
	@ln -s -r genSketch ~/bin/

clean: clean-debug clean-release
clean-release:
	@if [ -f Release/Makefile ]; then cd Release && make clean; fi
	@if [ -d Release ]; then rm -rf Release; fi

clean-debug:
	@if [ -f Debug/Makefile ]; then cd Debug && make clean; fi
	@if [ -d Debug ]; then rm -rf Debug; fi
