import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#f0f0f0"
    
    property int count: 0

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: countText
            text: root.count
            font.pixelSize: 48
            color: "#2c3e50"
            Layout.alignment: Qt.AlignHCenter
            
            Behavior on scale {
                NumberAnimation { duration: 100 }
            }
        }

        Button {
            id: incrementButton
            text: "Increment"
            Layout.alignment: Qt.AlignHCenter
            Layout.minimumWidth: 120
            Layout.minimumHeight: 40
            
            background: Rectangle {
                color: incrementButton.pressed ? "#2472a4" : 
                       incrementButton.hovered ? "#2980b9" : "#3498db"
                radius: 5
                
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
            }
            
            contentItem: Text {
                text: incrementButton.text
                color: "white"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            
            onClicked: {
                root.count++
                countText.scale = 1.2
                scaleTimer.restart()
            }
        }
    }
    
    Timer {
        id: scaleTimer
        interval: 100
        onTriggered: countText.scale = 1.0
    }
} 