# genSketch 文件、工程模板化创建工具说明文档

2016-04-22

----------------------------------------

## 起源

使用vim编写qt项目是一个麻烦的事情——你需要成对地创建头文件、和实现文件，并且继承自特定的qt类；若仅使用vim的话，比较浪费时间，于是需要一个通用一点的工具。

于是，我编写了genQtSketch工具。

后来，发现这个工具，可以和qt没啥关系，于是干脆改名为genSketch。

----------------------------------------------------------------------

## 使用方法

### 调用工程模板创建工程文件

说白了，就是为你想自动生成的文件，或者“工程”，参考你自己设定的模板文件，并创建在制定的文件夹下。
模板定位于 `$genSKetch/template` 文件夹下。

形如：

```bash
$ genSketch qt/cmake project_name
```

这样的调用，就会使用 `$genSKetch/template/qt/cmake.tpl/` 这个模板——注意，模板，都是以文件夹为单位，分开管理的。并且，该目录名，必须以`.tpl`为结尾。

第二个参数 `project_name` 是你将要在当前工作目录下，生成新文件夹的名字——同时，也是“工程名”；

之后，genSketch工具，就会开始应用一定的规则，在该新建的文件夹下，原样生成各种文件（夹）——文件的属性，也会原样复制。

该处理动作是递归进行的。相当于原样生成目录树。

----------------------------------------------------------------------

具体规则如下：

1. 如果遇到文件夹，则在目标目录下，生成同样的文件夹。至于文件名规则，见规则3；

2. 如果遇到文件，则在目标目录相应子目录下，生成目标文件；至于文件名规则，见规则3

3. 文件（夹）的名字，会按照类似bash-shell变量名的方式，进行替换；替换后，才是真正的名字；
   比如 `${Target}` 会被替换成 `project_name`
   如果，文件的名字以`.tpl`结尾，那么说明文件的内容，也会进行上述替换，然后再写入到目标路径文件中。

   相当于定义 Target=project_name 的环境变量。并且，.tpl文件内部出现，支持两种使用变量的方式： $Target或者${Target}

   另外，某$变量未定义过，会被替换为空串，即删除掉。还有想要输出 $ （dollar英语钱币符号），需要双写（double $，即$$）。

4. 替换所支持的“变量”列表：
        - ${s.timestamp} 替换为时间戳记
        - ${Target} 替换为 genSketch 的第二个参数 —— 默认为“.”，即当前路径。

----------------------------------------------------------------------

## 使用范例

```bash
$ genSketch qt/cmake qt-hello
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

```
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
```

模板源目录树

```
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
```

----------------------------------------------------------------------

调用模板，默认需要提供三个参数，分别是

 - 模板的相对路径（去掉文件夹的.tpl后缀之后路径）
 - 目标的名字——默认当前工作目录的文件夹的名字；
 - 输出文件夹——默认当前当前工作目录

对于第二点，相当于定义了一个“环境变量”，供.tpl脚本使用。如果用户想要更多的自定义变量，怎么办？

本工具，提供了两种方式：

1. 继续命令行；形如 -Dname=value 提供，就能额外提供一些命令行；
2. 使用 xxxx.tpl/.tpl.config.ini 来定义；

方式2，是将需要的变量定义，书写在dosini文件中；放在 section [env-variable] 下面即可；比如

$genSketch/template/qt/qtcreator.tpl/.tpl.config.ini

> ; 给PenvMgr2脚本环境，提供的变量
> [env-variable]
> project_name=${Target}
> cwd=$s.cwd
> debug_dir=${t.(`relpath $src_dir $cwd/Debug`)}
> release_dir=${t.(`relpath $src_dir $cwd/Release`)}
> src_dir=$cwd/src
> pro_path=${t.(`relpath $src_dir $src_dir/$project_name.pro`)}
> pro_user_path=$pro_path.user
> qt_version1=5
> qt_version2=6
> qt_version3=0
> qt_version=${qt_version1}.${qt_version2}.${qt_version3}
> qt_version_short=${qt_version1}${qt_version2}
> 
> ; 特定执行的脚本；可以呼叫应用程序
> [scripts]
> before-script=
> after-script=

两种自定义变量的优先级？

.tpl.config.ini 是定义相对不变，有默认值的变量；变量定义的应用顺序，暂时是按照字母表排序，以后应该会做成从上到下的顺序；
后面定义的重名变量，会覆盖前面的变量。

在应用config变量之后，接着，会应用环境变量所提供的变量——此时，是按照同名后续定义，会覆盖前面的方式。

另外，从变量定义式的范例可以看到，变量可以依赖其他的变量——同时，我说道config中，变量的应用时机，是字符序的——难道不会引起所依赖的变量，还没定义的非法引用的错误？

本系统，的变量定义式，可以看做是是记录一个字符创建规则而已——并且，不是立即生成调用的（参考makefile中=和:=的区别）；

另外，对于形如 ${t.(`relpath $src_dir $cwd/Debug`)} 的式子，还需要额外说明一下。

这是调用shell命令行。并且，如果这个命令行中，出现了$变量，在调用这个命令行之前，还会将用到的变量，作为环境变量，传入这个shell子进程。

具体的 $变量的替换，要在shell中，由shell完成。

#### 关于变量循环引用，和无穷递归

系统内部做了一个深度限制，如果超过500层的递归调用，会直接抛出系统异常。

### 模板应用顺序管理

由于可以内嵌shell脚本；而shell脚本的执行结果，可能依赖于当前文件夹下有多少文件
——要知道，我这个工具，本质目的就是批量创建文件（夹）；于是，如果创建顺序不可控的话，可能会导致shell脚本执行结果不一致。

于是，我额外提供了模板的应用顺序控制机制；通过 xxx.tpl/.tpl.order.txt 文件来控制。

参考： $genQtSketch/template/qt/qtcreator.tpl/.tpl.order.txt

> 98 ./${Target}/src/${Target}.pro.tpl
> 99 ./${Target}/src/${Target}.pro.user.tpl

模板文件夹中，默认的优先级是50；.tpl.order.txt文件中，按照 “优先级数字 空格 相对路径名” 的方式来设定优先级。

优先级应用规则

 - 实际优先级，按照从小到大排序应用。即，优先级最大的，最后被genSketch工具解析并应用。
 - 如果优先级相同，则列表中，后出现的文件，实际优先级更大。
 - 同名文件设定了多次，按出现在后面的所设定的优先级为准。
 - 可以不写表示优先级的数字，此时优先级均为50——当然，不提供优先级手动设定的模板文件，优先级也是50；

----------------------------------------------------------------------

注：获取文件列表，可以用find工具：

> find .

### 帮助系统

具体模板的帮助信息，可以按照 markdown 格式，书写在 xxx.tpl/.tpl.help.md 中；

当使用

> genSketch --help get_command

的时候，会在终端输出该文件中的内容。

如果get_command没有对应的.tpl.help.md文件，则会输出不存在；

最后：

> genSketch --help

会输出 genSketch 本身的帮助信息。

### 新建和编辑模板

> genSketch --edit get_command

如果 get_command 存在，则会输出该模板的全路径。

如果 不存在，则会在 $genSketch/template 下，创建具体的 xxx.tpl 文件夹；并在该文件夹下面，创建

 - .tpl.config.ini
 - .tpl.order.txt
 - .tpl.help.md

这三个文件的模板。

这三个文件本身的模板，可以定义在 $genSketch/下。创建的时候会原样拷贝过去。

### 与vim集成

参见：

https://github.com/sarrow104/genSketch-vim
