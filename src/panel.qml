import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    flags: Qt.Tool | Qt.FramelessWindowHint

    width: 640
    height: 40
    visible: true
    title: qsTr("Hello World")

    Component {
        id: contactDelegate

        Row {
            Text {
                id: label
                text: modelData.label
            }
            Text {
                id: text
                text: modelData.text
            }
        }
    }

    Flow {
        Repeater{
            model: kimpanel.lookupTable
            delegate: contactDelegate
        }
    }
}
