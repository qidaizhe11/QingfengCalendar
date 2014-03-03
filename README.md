
##Qingfeng Calendar
------------

一个基于 C++/Qt 的日历客户端，

初始版本基于 Qt Widgets，已废弃，
正在基于 Qt Quick/ Qml 重建，

此次重构版本基于 [Qt Organizer](https://qt.gitorious.org/qt/qtpim/) 和 [QtQuick.Controls.Calendar](https://qt.gitorious.org/qt/qtquickcontrols/)，
其中后者会出现在 [Qt 5.3](http://qt-project.org/wiki/New-Features-in-Qt-5.3)，至于前者何时会推到正式版，我真的不知道。
</br>

-------------------

###当前进度：

####正在解决：

- 事件编辑界面（EventEditView）的绘制
- 事件的创建，修改，保存（在EditView上操作）

####待解决:
- 与 Google Calendar 的同步（双向）
- 农历，节假日，以及国内法定节假日安排
- 周视图

####已解决:
- 月视图上提醒事件的显示
- 与数据库的交互（直接简单调用 Qt Organizer 了）
</br>

-----------
###构建与运行：

程序尚属开发初始阶段，安装包还远......

####[Qt 5.2](http://qt-project.org/downloads)

下载安装即可

####[Qt Organizer](https://qt.gitorious.org/qt/qtpim/)

git clone 后 qmake, make, make install

####Qingfeng Calendar

修改文件 [MyEventModel.cpp](https://github.com/qidaizhe11/QingfengCalendar/blob/master/MyPlugins/MyEventModel.cpp) 中代码：
```
QString file_name = "/home/daizhe/qidaizhe11@gmail.com-2.ics";
```
自行指定吧，应该基础的ics文件都能处理？这是Qt Organizer的事情，留待以后测试。

qmake, make，应该可以看到当前程序运行效果了。
</br>

---------------


尚在完善中...
