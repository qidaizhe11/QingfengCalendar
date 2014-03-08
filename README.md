
##Qingfeng Calendar
------------

一个基于 C++/Qt 的日历客户端，

初始版本基于 Qt Widgets，已废弃，
正在基于 Qt Quick/ Qml 重建，

此次重构版本基于 [Qt Organizer](https://qt.gitorious.org/qt/qtpim/) 和 [QtQuick.Controls.Calendar](https://qt.gitorious.org/qt/qtquickcontrols/)，
其中后者会出现在 [Qt 5.3](http://qt-project.org/wiki/New-Features-in-Qt-5.3)，至于前者何时会推到正式版，我真的不知道。
</br>

###当前进度：
-------------------

####正在解决：

- 事件编辑界面（EventEditView）的绘制

####待解决:
- 与 Google Calendar 的同步（双向）
- 农历，节假日，以及国内法定节假日安排
- 周视图
- ......

####已解决:
- 月视图上提醒事件的显示
- 与数据库的交互（直接调用的 Qt Organizer）
- 事件的增删改逻辑
</br>

###构建与运行：
-------------------

程序尚属初始阶段，离安装包还远......

####[Qt 5.2](http://qt-project.org/downloads)

下载安装即可

注：因为一个严重的bug, [QTBUG-35989](https://bugreports.qt-project.org/browse/QTBUG-35989), Linux系统需回退到**Qt 5.1**版本，否则可能会因下拉框，下拉按钮(ContextMenu)等导致崩溃。

####[Qt Organizer](https://qt.gitorious.org/qt/qtpim/)

git clone 后 qmake, make, make install

####Qingfeng Calendar

修改文件 [MyEventModel.cpp](https://github.com/qidaizhe11/QingfengCalendar/blob/master/MyPlugins/MyEventModel.cpp) 中代码：
```
QString file_name = "/home/daizhe/qidaizhe11@gmail.com-2.ics";
```
注释掉此行，并将其上面三行反注释。

qmake, make，应该可以看到当前程序运行效果了。
</br>

---------------
</br>
尚在不断完善中...
