# genSketch 文件、工程模板化创建工具说明文档

2016-04-22

----------------------------------------

## 起源

使用vim编写qt项目是一个麻烦的事情——你需要成对地创建头文件、和实现文件，并且继承自特定的qt类；若仅使用vim的话，比较浪费时间，于是需要一个通用一点的工具。

于是，我编写了genQtSketch工具。

后来，发现这个工具，可以和qt没啥关系，于是干脆改名为genSketch。

----------------------------------------------------------------------

## 使用方法

说白了，就是为你想自动生成的文件，或者“工程”，创建模板。模板定位于 `$genQtSKetch/template` 文件夹下。

形如：

```bash
$ genQtSketch qt/cmake project_name
```

这样的调用，就会使用 `$genQtSKetch/template/qt/cmake.tpl/` 这个模板——注意，模板，都是以文件夹为单位，分开管理的。并且，该目录名，必须以`.tpl`为结尾。

第二个参数 `project_name` 是你将要在当前工作目录下，生成新文件夹的名字——同时，也是“工程名”；

之后，genQtSketch工具，就会开始应用一定的规则，在该新建的文件夹下，原样生成各种文件（夹）——文件的属性，也会原样复制。

该处理动作是递归进行的。相当于原样生成目录树。

----------------------------------------------------------------------

具体规则如下：

1. 如果遇到文件夹，则在目标目录下，生成同样的文件夹。至于文件名规则，见规则3；

2. 如果遇到文件，则在目标目录相应子目录下，生成目标文件；至于文件名规则，见规则3

3. 文件（夹）的名字，会按照类似bash-shell变量名的方式，进行替换；替换后，才是真正的名字；
   比如 `${Target}` 会被替换成 `project_name`
   如果，文件的名字以`.tpl`结尾，那么说明文件的内容，也会进行上述替换，然后再写入到目标路径文件中。

4. 替换所支持的“变量”列表：
	- ${s.timestamp} 替换为时间戳记
	- ${Target} 替换为 genQtSketch 的第二个参数 —— 默认为“.”，即当前路径。

----------------------------------------------------------------------

## 使用范例

```bash
$ genQtSketch qt/cmake qt-hello
get_command = qt/cmake
target = qt-hello
dir = /home/sarrow/project/qt5
get_command = /home/sarrow/project/genQtSketch/template/qt/cmake.tpl
qt-hello/ <- `mkdir -p`
qt-hello/readme.txt <- copy
qt-hello/runQtCreator <- copy
qt-hello/Debug/ <- `mkdir -p`
qt-hello/Release/ <- `mkdir -p`
qt-hello/src/ <- `mkdir -p`
qt-hello/src/CMakeLists.txt <- template
qt-hello/src/Makefile <- copy
qt-hello/src/mainwindow.h <- copy
qt-hello/src/CMakeLists.txt.user <- template
qt-hello/src/mainwindow.cpp <- copy
qt-hello/src/main.cpp <- copy
```

目标目录树：

	qt-hello
	├── Debug
	├── readme.txt
	├── Release
	├── runQtCreator
	└── src
	    ├── CMakeLists.txt
	    ├── CMakeLists.txt.user
	    ├── main.cpp
	    ├── mainwindow.cpp
	    ├── mainwindow.h
	    └── Makefile

	3 directories, 8 files

模板源目录树

	template/qt/cmake.tpl/
	└── ${Target}
	    ├── Debug
	    ├── readme.txt
	    ├── Release
	    ├── runQtCreator
	    └── src
		├── CMakeLists.txt.tpl
		├── CMakeLists.txt.user.tpl
		├── main.cpp
		├── mainwindow.cpp
		├── mainwindow.h
		└── Makefile

	4 directories, 8 files
