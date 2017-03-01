.PHONY: all release debug
.PHONY: run run-debug
.PHONY: clean clean-debug clean-release
.PHONY: install

.PHONY: qt-ts

CMAKE_PREFIX_PATH=$(shell qmake -query QT_INSTALL_LIBS)/cmake

CMAKE_FLAGS=

ifeq ($(OS),Windows_NT)
  ifeq ($(TERM),xterm)
    CMAKE_FLAGS= -G"MSYS Makefiles"
  endif
endif

TARGET=${Target}
TARGET_D = $(TARGET)D
#TARGET_G = $(TARGET)G

SRC_EXT=cpp

SOURCES = $(wildcard *.$(SRC_EXT))

PREFIX=..
all: release
release:
	@mkdir -p $(PREFIX)/Release
	@cd $(PREFIX)/Release && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH) -DCMAKE_INSTALL_PREFIX=~/bin $(PREFIX)/src && make

debug:
	@mkdir -p $(PREFIX)/Debug
	@cd $(PREFIX)/Debug && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH) $(PREFIX)/src && make

release debug: qt-ts
qt-ts: myapp_zh_CN.ts

%.ts: $(SOURCES)
	@lupdate $^ -ts $@

run: release
	@./$(TARGET)

run-debug: debug
	@./$(TARGET_D)

install:
	@if [ -d $(PREFIX)/Release ]; then cd $(PREFIX)/Release && make install; fi

clean: clean-debug clean-release
clean-release:
	@if [ -f $(PREFIX)/Release/Makefile ]; then cd $(PREFIX)/Release && make clean; fi
	@if [ -d $(PREFIX)/Release ]; then rm -rf $(PREFIX)/Release; fi

clean-debug:
	@if [ -f $(PREFIX)/Debug/Makefile ]; then cd $(PREFIX)/Debug && make clean; fi
	@if [ -d $(PREFIX)/Debug ]; then rm -rf $(PREFIX)/Debug; fi


