* ģ�崴��C++��

date:2016-03-19

======================================================================

/home/sarrow/project/genQtSketch/main.cpp|1

qt creator �Ĵ���������unit����ã����ǣ�����ڶ�����ʮ�����ڹ�������qt������˵
�������Ե÷�����

�Ҿ��ø����е��ǣ��������з�ʽ�Ĵ�������vim��¼��������ʷ����һ������������һ��
���������������С��������ͨ�����޸ģ��������������ļ�sketch������

����qt�����࣬��ѡ�Ļ����У�

  - QObject
  - QWidget
  - QMainWindow
  - QDeclarativeItem
  - QQuickItem

ֻ�п����õ�ͷ�ļ���������棬����һ����

  - QSharedData

���⣬�����������ļ�����������Ŀ¼����QtCreator��Ĭ���ǰ�ͷ�ļ���Դ�ļ�����ͬһ
��Ŀ¼��

��ô�죿��Ŀ¼�İ취��Щ��

> qt/QObject:MyObject

���ʾ�����ɼ̳���QObject��class MyObject��

��ʵ���������url���ӣ��ֱ��Ǹ������࣬�Լ��Զ������֣��������ɵ�·��������ڵ�
��������λ�ã�

���������ǵ���ȫ�����أ�����url ���ǿ���дΪ��

> qt/QObject MyObject [path]

���⣬�����Զ���xml�������Ա��ϸ�ʵ����ɶ��������ñ���ҳ����
���ڷ���

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

** ���������̵���ӣ�

private:
	-

protected:
	#

public:
	+

----------------------------------------------------------------------

** �̳й�ϵ������

�ο�UML�еı�ǣ�
http://www.cnblogs.com/alex-blog/articles/2704214.html

�̳У�

	����
	  ^
	  |
	����

�̳У�ʵ�֣�������������࣬����ʵ�ֽӿ�

	����
	  ^
	  .
	  .
	  .
	����

���������磬A::+depend(B b)->void����˵��A�����depend������������B��������
����ʵ�֣��Բ�����ʽ����

������

	A::-m_b -> B;

	Aʹ����B���͵ĳ�Ա������ʹ����B���͵�ȫ�ֱ�����

�ۺϣ�

	�ۺ��ǹ�����ϵ��һ�������������ֵ��������벿�֡�ӵ�еĹ�ϵ����has-a�Ĺ�
	ϵ����ʱ�����벿��֮���ǿɷ���ģ����ǿ��Ծ��и��Ե��������ڣ����ֿ�����
	�ڶ���������Ҳ����Ϊ����������������������CPU����˾��Ա����
	��ϵ�ȣ������ڴ�����棬�͹�����ϵ��һ�µģ�ֻ�ܴ����弶�������֣�

	Family <> 0..1 -----------------> 0..* Child

���

	���Ҳ�ǹ�����ϵ��һ�������������ֵ���һ��contains-a�Ĺ�ϵ�����ֹ�ϵ�Ⱦ�
	�ϸ�ǿ��Ҳ��Ϊǿ�ۺϣ���ͬ�����������벿�ּ�Ĺ�ϵ������ʱ�����벿���ǲ�
	�ɷֵģ�������������ڽ���Ҳ����ζ�Ų��ֵ��������ڽ��������������Ĵ���
	�������ڴ�����棬�͹�����ϵ��һ�µģ�ֻ�ܴ����弶�������֣�

	Person <> 0..1 -----------------> 0..* Brain

----------------------------------------------------------------------

���ڼ̳С�ʵ�������ֹ�ϵû�������ʣ��������ֵ���һ�������ࡢ��������ӿڼ������
��ϵ�����������߹�ϵ�����ֵ��������ࡢ��������ӿڼ�����á������ϵ���ǱȽ�����
�ֵģ��кܶ������Ĺ�ϵҪ��׼����λ�Ǻ��ѵģ�ǰ��Ҳ�ᵽ���⼸�ֹ�ϵ�������弶��
�ģ����ԴӴ�����沢������ȫ���ָ��ֹ�ϵ��

���ܵ���˵�����ֹ�ϵ�����ֵ�ǿ���̶�����Ϊ�����>�ۺ�>����>������

----------------------------------------------------------------------

�ۺϸ������ʵ�����ڹ��� ֻ������������������Ĺ��� ��Ϊ����ͬ���� ��������֮��
�����������֮�� ����������һ������һ������֮���кβ�ͬ

�ۺ�����ϵĸ������Ų������ڴ�׸����Ҿ��Ѿ��˽��� ����ֱ��������

����ʦ�ġ��󻰡���ٴ��Ǹ���������Ӻ����� �ڴ��Ҿͽ���һ�� ����ϲ�����ֺ��¹¶�
��������һֱ����Ⱥ�ӵ����� ������������Ⱥ ÿһֻ���㶼���Լ�����Ⱥ ÿ����Ⱥ����
�ö���� ��������Ⱥ�����ֹ�ϵ�Ϳ��Գ�֮Ϊ�ۺ� ����ÿֻ���㶼����ֻ��� ��������
��Ĺ�ϵ�ͽ������ �д˿ɼ� �ۺϵĹ�ϵ����û����Ͻ��� ���㲻����Ϊ���ǵ�Ⱥ����
��Ⱥ��ɢ���޷����� �������޷����������������桪����Ϲ�ϵ���������ͬ������
����

�ۺϹ�ϵͼ��

	��Ⱥ <>---------------- > ����

��Ϲ�ϵͼ��

	���� <>---------------- > ���

----------------------------------------------------------------------

** ͨ�õĴ����̳�

ö�� Q5/include�μ�Ŀ¼������Ϊ���࣬��Ҫ���û������Զ����������ⷽ����ȡ��

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

�Ⲣ����ȡ����Ϊʵ�������������Щ�ļ��е����棬���Ҳ�ֹһ����һ��56���ļ��У�ÿ
���ļ������棬���賣�õ�����������ô��������������100+�����tab�Զ�ѡȡ��ʱ��
ֻ��ͨ��ǰ׺�����ң���ô�����ˣ��ð���������

���ԣ���ʱ���׵İ취�����Ƕඨ��ģ��ɣ�

