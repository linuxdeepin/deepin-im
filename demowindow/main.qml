import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 300
    height: 300
    visible: true
    title: qsTr("Hello World")

    TextInput {
        id: input
        anchors.fill: parent
        font.pixelSize: 20
        text: "Hello World"
        focus: true
    }
}
