
##Qingfeng Calendar
------------

一个基于 C++/Qt 的日历客户端。

当前主界面（月视图）效果：
![ScreenShot](/resources/screenshot-monthview.jpg)

UI界面采用 Qt Quick/ Qml 绘制

程序整体基于 [Qt Organizer](https://qt.gitorious.org/qt/qtpim/) 和 [QtQuick.Controls.Calendar](https://qt.gitorious.org/qt/qtquickcontrols/)，皆为 Qt 未正式面世的模块，后者会在 [Qt 5.3](http://qt-project.org/wiki/New-Features-in-Qt-5.3)面世，至于前者，似乎还远。
</br>

##当前进度：
------------

####正在解决：
- 周视图
- 与 Google Calendar 的同步

####待解决:
- 底层 SQLite 数据库的重新启用
- 农历，节假日
- 桌面透明工具
- 配置界面
- ......

####已解决:
- 月视图
- Qt Organizer 代理层
- 事件编辑界面（EventEditView）
</br>

##构建与运行：
------------

程序尚在快速开发阶段，暂不考虑安装包

####[Qt 5.2](http://qt-project.org/downloads)

下载安装即可

####[Qt Organizer](https://qt.gitorious.org/qt/qtpim/)

git clone 后 qmake, make, make install

####Qingfeng Calendar

注释掉文件 [main.qml](https://github.com/qidaizhe11/QingfengCalendar/blob/master/qml/QingfengCalendar/main.qml) 中 FileDialog 下代码：
```
visible: false
```
并将其上面一行反注释。

注释掉文件 [MyEventModel.cpp](https://github.com/qidaizhe11/QingfengCalendar/blob/master/MyPlugins/MyEventModel.cpp) 中代码：
```
importEvents(QUrl("file:///home/daizhe/qidaizhe11@gmail.com-2.ics"));
```

另外，出于个人开发需要，程序 qml 中引用的模块与 [main.cpp](https://github.com/qidaizhe11/QingfengCalendar/blob/master/main.cpp) 中注册的名称可能略有不同，类似 **MyCalendar2.Events** 与 **MyCalendar.Events** 的区别，需自行调整一致。

qmake, make，应该可以看到当前程序运行效果了。
</br>

##其它
------------

当前周视图界面效果：
![ScreenShot](/resources/screenshot-weekview.jpg)

当前编辑界面效果：
![ScreenShot](/resources/screenshot-edit-view.jpg)

尚在不断完善中...
