.PHONY: all release debug clean install clean-debug clean-release gprof run-gprof run run-debug perf-view

CMAKE_FLAGS=
RUN_PARAMS=

ifeq ($(OS),Windows_NT)
  ifeq ($(TERM),xterm)
    CMAKE_FLAGS= -G"MSYS Makefiles"
  endif
endif

# usage: make -verbose=1
ifeq ($(verbose),1)
	CMAKE_FLAGS+=-DCMAKE_VERBOSE_MAKEFILE=on
else
	CMAKE_FLAGS+=-DCMAKE_VERBOSE_MAKEFILE=off
endif

all: release
release:
	@mkdir -p Release
	cd Release && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/bin .. && make

debug:
	@mkdir -p Debug
	@cd Debug && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Debug .. && make

gprof:
	@mkdir -p Gprof
	@cd Gprof && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Gprof .. && make

run:
	@${Target}

run-debug:
	@gdb ./${Target}D

gprof-view: gprof
	@if [ ! -f gmon.out ]; then ./${Target}G $(RUN_PARAMS); fi
	@gprof -b --demangle ./${Target}G gmon.out | gprof2dot -s | xdot &

# NOTE: perf.data need execute bin-pg with parameters!
perf-view: gprof
	@if [ ! -f perf.data ]; then perf record -g -- ./${Target}G $(RUN_PARAMS); fi
	@perf script | c++filt | gprof2dot -s -f perf | xdot &

install:
	@cd Release && make install

clean: clean-debug clean-release clean-gprof
clean-release:
	@if [ -f Release/Makefile ]; then cd Release && make clean; fi
	@if [ -d Release ]; then rm -rf Release; fi

clean-debug:
	@if [ -f Debug/Makefile ]; then cd Debug && make clean; fi
	@if [ -d Debug ]; then rm -rf Debug; fi

clean-gprof:
	@if [ -f gmon.out ]; then rm gmon.out ; fi
	@if [ -f perf.data ]; then rm perf.data* ; fi
	@if [ -f Gprof/Makefile ]; then cd Gprof && make clean; fi
	@if [ -d Gprof ]; then rm -rf Gprof; fi
