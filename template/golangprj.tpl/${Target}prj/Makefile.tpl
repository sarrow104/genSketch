.PHONY: all build install run doc vim vscode

all: build

build:
	go build ${Target}

install:
	go install ${Target}

run:
	go run ${Target}

doc:
	go doc ${Target}

vim:
	. ./addgopath.sh && gvim src/${Target}/${Target}.go

vscode:
	. ./addgopath.sh && code .
