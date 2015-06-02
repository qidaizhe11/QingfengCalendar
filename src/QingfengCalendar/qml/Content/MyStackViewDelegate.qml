import QtQuick 2.1
import QtQuick.Controls 1.0

StackViewDelegate {
    id: stack_view_delegate

    property bool horizontal : true

    function getTransition(properties)
    {
        return stack_view_delegate["horizontalSlide"][properties.name]
    }

    function transitionFinished(properties)
    {
        properties.exitItem.x = 0
        properties.exitItem.y = 0
    }

    property QtObject horizontalSlide: QtObject {
        property Component pushTransition: StackViewTransition {
            PropertyAnimation {
                target: enterItem
//                    property: "x"
//                    from: target.width
//                    to: 0
//                    duration: 200
                property: "opacity"
                from: 0
                to: 1
                duration: 300
            }
            PropertyAnimation {
                target: exitItem
                property: "x"
                from: 0
                to: -target.width
                duration: 300
//                    property: "opacity"
//                    from: 1
//                    to: 0
//                    duration: 200
            }
        }

        property Component popTransition: StackViewTransition {
            PropertyAnimation {
                target: enterItem
                property: "x"
                from: -target.width
                to: 0
                duration: 300
//                    property: "opacity"
//                    from: 0
//                    to: 1
//                    duration: 200
            }
            PropertyAnimation {
                target: exitItem
//                    property: "x"
//                    from: 0
//                    to: target.width
//                    duration: 200
                property: "opacity"
                from: 1
                to: 0
                duration: 300
            }
        }
        property Component replaceTransition: pushTransition
    }
}
