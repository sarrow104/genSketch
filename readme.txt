* 模板创建C++类

date:2016-03-19

======================================================================

/home/sarrow/project/genQtSketch/main.cpp|1

qt creator 的创建具名的unit类好用；但是，这对于动辄几十个窗口工具条的qt工程来说
，还是显得繁琐；

我觉得更可行的是，用命令行方式的创建——vim记录了命令历史；下一个命令行与上一个
命令的区别，往往很小；这样，通过简单修改，就能批量创建文件sketch出来。

对于qt界面类，可选的基类有：

  - QObject
  - QWidget
  - QMainWindow
  - QDeclarativeItem
  - QQuickItem

只有可启用的头文件，则比上面，多了一个：

  - QSharedData

此外，就是类名、文件名、创建的目录——QtCreator，默认是把头文件、源文件，放同一
层目录。

怎么办？用目录的办法好些：

> qt/QObject:MyObject

这表示，生成继承自QObject的class MyObject；

其实，这很像是url链接！分别是根，基类，以及自定义名字；至于生成的路径，则放在第
二个参数位置；

不过，考虑到补全的因素，上述url 还是可以写为：

> qt/QObject MyObject [path]

另外，还可以定义xml描述，以便更细质地生成东西——好比网页……
至于方法

QOBject

> #ifndef MYTEST_H
> #define MYTEST_H
>
> #include <QObject>
>
> class MyTest : public QObject
> {
>     Q_OBJECT
> public:
>     explicit MyTest(QObject *parent = 0);
>
> signals:
>
> public slots:
> };
>
> #endif // MYTEST_H
>
>
> #include "mytest.h"
>
> MyTest::MyTest(QObject *parent) : QObject(parent)
> {
>
> }

QWidget

> #ifndef MYWIDGET_H
> #define MYWIDGET_H
>
> #include <QWidget>
>
> class MyWidget : public QWidget
> {
>     Q_OBJECT
> public:
>     explicit MyWidget(QWidget *parent = 0);
>
> signals:
>
> public slots:
> };
>
> #endif // MYWIDGET_H


> #include "mywidget.h"
>
> MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
> {
>
> }

QMainWindow

> #ifndef MAINWINDOW_H
> #define MAINWINDOW_H
>
> #include <QMainWindow>
>
> class MainWindow : public QMainWindow
> {
>     Q_OBJECT
> public:
>     explicit MainWindow(QWidget *parent = 0);
>
> signals:
>
> public slots:
> };
>
> #endif // MAINWINDOW_H


> #include "mainwindow.h"
>
> MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
> {
>
> }

----------------------------------------------------------------------

** 方法，过程的添加：

private:
	-

protected:
	#

public:
	+

----------------------------------------------------------------------

** 继承关系、包含

参考UML中的标记：
http://www.cnblogs.com/alex-blog/articles/2704214.html

继承：

	父类
	  ^
	  |
	子类

继承（实现）——即，虚基类，子类实现接口

	父类
	  ^
	  .
	  .
	  .
	子类

依赖：比如，A::+depend(B b)->void；就说，A的这个depend方法，依赖于B——依赖
于其实现（以参数形式）；

关联：

	A::-m_b -> B;

	A使用了B类型的成员，或者使用了B类型的全局变量；

聚合：

	聚合是关联关系的一种特例，他体现的是整体与部分、拥有的关系，即has-a的关
	系，此时整体与部分之间是可分离的，他们可以具有各自的生命周期，部分可以属
	于多个整体对象，也可以为多个整体对象共享；比如计算机与CPU、公司与员工的
	关系等；表现在代码层面，和关联关系是一致的，只能从语义级别来区分；

	Family <> 0..1 -----------------> 0..* Child

组合

	组合也是关联关系的一种特例，他体现的是一种contains-a的关系，这种关系比聚
	合更强，也称为强聚合；他同样体现整体与部分间的关系，但此时整体与部分是不
	可分的，整体的生命周期结束也就意味着部分的生命周期结束；比如你和你的大脑
	；表现在代码层面，和关联关系是一致的，只能从语义级别来区分；

	Person <> 0..1 -----------------> 0..* Brain

----------------------------------------------------------------------

对于继承、实现这两种关系没多少疑问，他们体现的是一种类与类、或者类与接口间的纵向
关系；其他的四者关系则体现的是类与类、或者类与接口间的引用、横向关系，是比较难区
分的，有很多事物间的关系要想准备定位是很难的，前面也提到，这几种关系都是语义级别
的，所以从代码层面并不能完全区分各种关系；

但总的来说，后几种关系所表现的强弱程度依次为：组合>聚合>关联>依赖；

----------------------------------------------------------------------

聚合跟组合其实都属于关联 只不过它们是两种特殊的关联 因为本是同根生 所以它们之间
难免会有相似之处 下面让我们一起来看一下它们之间有何不同

聚合与组合的概念相信不用我在此赘述大家就已经了解了 下面直接上例子

程老师的《大话》里举大那个大雁的例子很贴切 在此我就借用一下 大雁喜欢热闹害怕孤独
所以它们一直过着群居的生活 这样就有了雁群 每一只大雁都有自己的雁群 每个雁群都有
好多大雁 大雁与雁群的这种关系就可以称之为聚合 另外每只大雁都有两只翅膀 大雁与雁
翅的关系就叫做组合 有此可见 聚合的关系明显没有组合紧密 大雁不会因为它们的群主将
雁群解散而无法生存 而雁翅就无法脱离大雁而单独生存——组合关系的类具有相同的生命
周期

聚合关系图：

	雁群 <>---------------- > 大雁

组合关系图：

	大雁 <>---------------- > 翅膀

----------------------------------------------------------------------

** 通用的创建继承

枚举 Q5/include次级目录名，作为基类，再要求用户输入自定义类名；这方法可取吗？

 Qt3DCore/
 Qt3DInput/
 Qt3DLogic/
 Qt3DQuick/
 Qt3DQuickInput/
 Qt3DQuickRender/
 Qt3DRender/
 QtBluetooth/
 QtCLucene/
 QtConcurrent/
 QtCore/
 QtDBus/
 QtDesigner/
 QtDesignerComponents/
 QtGui/
 QtHelp/
 QtLabsControls/
 QtLabsTemplates/
 QtLocation/
 QtMultimedia/
 QtMultimediaQuick_p/
 QtMultimediaWidgets/
 QtNetwork/
 QtNfc/
 QtOpenGL/
 QtOpenGLExtensions/
 QtPlatformHeaders/
 QtPlatformSupport/
 QtPositioning/
 QtPrintSupport/
 QtQml/
 QtQmlDevTools/
 QtQuick/
 QtQuickParticles/
 QtQuickTest/
 QtQuickWidgets/
 QtScript/
 QtScriptTools/
 QtSensors/
 QtSerialBus/
 QtSerialPort/
 QtSql/
 QtSvg/
 QtTest/
 QtUiPlugin/
 QtUiTools/
 QtWebChannel/
 QtWebEngine/
 QtWebEngineCore/
 QtWebEngineWidgets/
 QtWebSockets/
 QtWebView/
 QtWidgets/
 QtX11Extras/
 QtXml/
 QtXmlPatterns/

----------------------------------------------------------------------

这并不可取；因为实体类基本都在这些文件夹的下面，而且不止一个！一共56个文件夹；每
个文件夹下面，假设常用的有两个；那么，加起来，就是100+；如果tab自动选取的时候，
只能通过前缀来查找，那么死定了，得把人找死！

所以，暂时靠谱的办法，还是多定义模板吧！

