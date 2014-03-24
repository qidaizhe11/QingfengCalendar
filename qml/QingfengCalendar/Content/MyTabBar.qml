import QtQuick 2.1
import QtQuick.Controls 1.1

FocusScope {
    id: tabbar
    height: tabrow.height
    width: tabrow.width

    activeFocusOnTab: true

    Keys.onRightPressed: {
        if (tabView && tabView.currentIndex < tabView.count - 1)
            tabView.currentIndex = tabView.currentIndex + 1
    }
    Keys.onLeftPressed: {
        if (tabView && tabView.currentIndex > 0)
            tabView.currentIndex = tabView.currentIndex - 1
    }

//    onTabViewChanged: parent = tabView
    visible: true

    property var tabView
//    property var style
//    property var styleItem: tabView.__styleItem ? tabView.__styleItem : null

    property bool tabsMovable: false

    property int tabsAlignment: Qt.AlignLeft

    property int tabOverlap: 1

    property int elide: Text.ElideRight

    property real availableWidth: tabbar.width

    property var __selectedTabRect

    function tab(index) {
        for (var i = 0; i < tabrow.children.length; ++i) {
            if (tabrow.children[i].tabindex === index) {
                return tabrow.children[i]
            }
        }
        return null;
    }

    /*! \internal */
    function __isAncestorOf(item, child) {
        //TODO: maybe removed from 5.2 if the function was merged in qtdeclarative
        if (child === item)
            return false;

        while (child) {
            child = child.parent;
            if (child === item)
                return true;
        }
        return false;
    }

    property Component tab_item: Item {
        scale: control.tabPosition === Qt.TopEdge ? 1 : -1

        property int totalOverlap: tabOverlap * (control.count - 1)
        property real maxTabWidth: (styleData.availableWidth + totalOverlap) / control.count

        implicitWidth: Math.round(Math.min(maxTabWidth, textitem.implicitWidth + 20))
        implicitHeight: Math.round(textitem.implicitHeight + 10)

        Item {
            anchors.fill: parent
            anchors.bottomMargin: styleData.selected ? 0 : 2
            BorderImage {
                anchors.fill: parent
                source: styleData.selected ?
                            "../images/tab_selected.png" :
                            "../images/tab.png"
                border.top: 6
                border.bottom: 6
                border.left: 6
                border.right: 6
                anchors.topMargin: styleData.selected ? 0 : 1
            }
        }
        Text {
            id: textitem
            anchors.fill: parent
            anchors.leftMargin: 4
            anchors.rightMargin: 4
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: styleData.title
            elide: Text.ElideMiddle
            renderType: Text.NativeRendering
            scale: control.tabPosition === Qt.TopEdge ? 1 : -1
            color: __syspal.text
            Rectangle {
                anchors.centerIn: parent
                width: textitem.paintedWidth + 6
                height: textitem.paintedHeight + 4
                visible: (styleData.activeFocus && styleData.selected)
                radius: 3
                color: "#224f9fef"
                border.color: "#47b"
            }
        }
    }

    ListView {
        id: tabrow
        objectName: "tabrow"
        Accessible.role: Accessible.PageTabList
        LayoutMirroring.enabled: Qt.application.layoutDirection === Qt.RightToLeft
        spacing: -tabOverlap
        orientation: Qt.Horizontal
        interactive: false
        focus: true

        anchors.fill: parent

        // Note this will silence the binding loop warnings caused by QTBUG-35038
        // and should be removed when this issue is resolved.
        property int contentWidthWorkaround: contentWidth > 0 ? contentWidth: 0
//        width: Math.min(availableWidth, count ? contentWidthWorkaround : availableWidth)
        height: currentItem ? currentItem.height : 0

        highlightMoveDuration: 0
        currentIndex: tabView.currentIndex
        onCurrentIndexChanged: tabrow.positionViewAtIndex(currentIndex, ListView.Contain)

        moveDisplaced: Transition {
            NumberAnimation {
                property: "x"
                duration: 125
                easing.type: Easing.OutQuad
            }
        }

        states: [
            State {
                name: "left"
                when: tabsAlignment === Qt.AlignLeft
                AnchorChanges { target:tabrow ; anchors.left: parent.left }
//                PropertyChanges { target:tabrow ; anchors.leftMargin: leftCorner.width }
            },
            State {
                name: "center"
                when: tabsAlignment === Qt.AlignHCenter
                AnchorChanges { target:tabrow ; anchors.horizontalCenter: tabbar.horizontalCenter }
            },
            State {
                name: "right"
                when: tabsAlignment === Qt.AlignRight
                AnchorChanges { target:tabrow ; anchors.right: parent.right }
//                PropertyChanges { target:tabrow ; anchors.rightMargin: rightCorner.width }
            }
        ]

        model: tabView.__tabs

        delegate: MouseArea {
            id: tabitem
            objectName: "mousearea"
            hoverEnabled: true
            focus: true
            enabled: modelData.enabled

            Binding {
                target: tabbar
                when: selected
                property: "__selectedTabRect"
                value: Qt.rect(x, y, width, height)
            }

//            drag.target: tabsMovable ? tabloader : null
//            drag.axis: Drag.XAxis
//            drag.minimumX: drag.active ? 0 : -Number.MAX_VALUE
//            drag.maximumX: tabrow.width - tabitem.width

            property int tabindex: index
            property bool selected : tabView.currentIndex === index
            property string title: modelData.title
            property bool nextSelected: tabView.currentIndex === index + 1
            property bool previousSelected: tabView.currentIndex === index - 1

//            z: selected ? 1 : -index
            z: selected ? 10 : 1
            implicitWidth: tabloader.implicitWidth
            implicitHeight: tabloader.implicitHeight

            function changeTab() {
                tabView.currentIndex = index;
                var next = tabbar.nextItemInFocusChain(true);
                if (__isAncestorOf(tabView.getTab(index), next))
                    next.forceActiveFocus();
            }

            onClicked: {
                if (tabrow.interactive) {
                    changeTab()
                }
            }
            onPressed: {
                if (!tabrow.interactive) {
                    changeTab()
                }
            }

            Loader {
                id: tabloader

                property Item control: tabView
                property int index: tabindex

                property QtObject styleData: QtObject {
                    readonly property alias index: tabitem.tabindex
                    readonly property alias selected: tabitem.selected
                    readonly property alias title: tabitem.title
                    readonly property alias nextSelected: tabitem.nextSelected
                    readonly property alias previsousSelected: tabitem.previousSelected
                    readonly property alias hovered: tabitem.containsMouse
                    readonly property alias enabled: tabitem.enabled
                    readonly property bool activeFocus: tabbar.activeFocus
                    readonly property real availableWidth: tabbar.availableWidth
                    readonly property real totalWidth: tabrow.contentWidth
                }

                anchors.fill: parent
                sourceComponent: tab_item

//                Drag.keys: "application/x-tabbartab"
//                Drag.active: tabitem.drag.active
//                Drag.source: tabitem

//                property real __prevX: 0
//                property real __dragX: 0
//                onXChanged: {
//                    if (Drag.active) {
//                        // keep track for the snap back animation
//                        __dragX = tabitem.mapFromItem(tabrow, tabloader.x, 0).x

//                        // when moving to the left, the hot spot is the left edge and vice versa
//                        Drag.hotSpot.x = x < __prevX ? 0 : width
//                        __prevX = x
//                    }
//                }

                width: tabitem.width
                state: Drag.active ? "drag" : ""

                transitions: [
                    Transition {
                        to: "drag"
                        PropertyAction { target: tabloader; property: "parent"; value: tabrow }
                    },
                    Transition {
                        from: "drag"
                        SequentialAnimation {
                            PropertyAction { target: tabloader; property: "parent"; value: tabitem }
                            NumberAnimation {
                                target: tabloader
                                duration: 50
                                easing.type: Easing.OutQuad
                                property: "x"
                                from: tabloader.__dragX
                                to: 0
                            }
                        }
                    }
                ]
            }

            Accessible.role: Accessible.PageTab
            Accessible.name: modelData.title
        }
    }
}
