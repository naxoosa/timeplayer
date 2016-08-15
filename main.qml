import QtQuick 2.7
import QtQuick.Window 2.2
import QtWinExtras 1.0

Window {
    id: mainwindow
    visible: true
    flags: Qt.FramelessWindowHint |
           Qt.WindowMinimizeButtonHint | Qt.Window
    width: 320
    height: 320
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2
    color: "transparent"
    title: qsTr("TimePlayer")    

    Player {
        id: player;
        objectName: "player"
        anchors.fill: parent
        width: 320;
        height: 320;
        property var parentplayer: mainwindow;
    }

    Image {
        id: detail
        width:20
        height:20
        source: "contents/load-icon.png"
        x:225
        y:8

        MouseArea {
            anchors.fill: detail
            onClicked: {
                mainwindow.opendetail(0);
            }

        }
    }

    Image {
        id: move
        width:20
        height:20
        source: "contents/move-icon.png"
        x:260
        y:30
    }

    MouseArea {
        anchors.fill: move;

        property variant clickPos: "1,1"
        onPressed: {
            clickPos = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            mainwindow.x += delta.x;
            mainwindow.y += delta.y;
        }
    }

    Image {
        id: minimize
        width:20
        height:20
        source: "contents/minimize.png"
        x:283
        y:58

        MouseArea {
            anchors.fill: minimize
            onClicked: {
                mainwindow.visibility = Window.Minimized
            }
        }
    }

    Image {
        id: quit
        width:20
        height:20
        source: "contents/close-icon.png"
        x:299
        y:90

        MouseArea {
            anchors.fill: quit
            onClicked: {
                //if(lblpause.pressed != true){
                    //beforequit();
                    Qt.quit();
                //}
            }
        }
    }

    /*MouseArea {
            anchors.fill: parent
            onClicked: startReport.sendMessage({ 'type': 'pause', 'action': 'add'})
    }*/

    WorkerScript {
           id: startReport
           source: "js/report.js"

           onMessage: {
               console.log(messageObject.type);
               sendmessage(messageObject);
           }
    }

    signal sendmessage(var message);
    signal opendetail(var val);
}
