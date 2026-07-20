import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import Snake

Window {
    id: mainWindowId
    width: 640
    height: 640
    visible: true
    Material.theme: "Dark"
    title: qsTr("Snake Game")


    SnakeManager{
        id: snakeModelId
        Component.onCompleted: {
        }
    }

    Connections {
            target: snakeModelId
            function onMessageChanged() {
                if (snakeModelId.message !== "") {
                    messageId.open()
                }
            }
    }



    MessageDialog{
        id: messageId
        text: snakeModelId.message
        // anchors.centerIn: parent
        visible: false
        buttons: MessageDialog.Close | MessageDialog.Ok
        onAccepted:{
            snakeModelId.message = ""
            snakeModelId.reset()
        }

        onRejected: {
            snakeModelId.requestQuit()



        }
    }

    Rectangle{
        anchors.fill: parent
        color: "#202020"
        focus: true
        Keys.onPressed: (event) => {
                            snakeModelId.changeDirection(event.key);
                        }

        Repeater {
            model: snakeModelId
            Rectangle {
                x: position.x
                y: position.y
                width: 20; height: 20
                color: "yellow"
                border.color: "#202020"
            }
        }


        Rectangle {
            x: snakeModelId.food.x
            y: snakeModelId.food.y
            width: 20; height: 20
            radius: 10
            color: "red"
            Component.onCompleted: {
                // console.log(snakeModelId.food.x)
            }
        }



    }
}
