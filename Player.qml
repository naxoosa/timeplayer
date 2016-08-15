import QtQuick 2.7
import QtQml 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import CustomProgressBar 1.0

Item {
    property bool issimplify : false;
    property var circles: [];    

    Image {
        id:bg
        anchors.fill: parent
        width: 320;
        height: 320;
        source: "contents/background.png"
    }

    CustomProgressBar {
        id: playprogressbar
        objectName: "playprogressbar"
        width: 319;
        height: 319;
    }

    Image {
        width: 298;
        height: 298;
        x: 12
        y: 12
        source: "contents/support.png"

    }

    Image {
        objectName: "btncenter"
        id: btncenter
        width: 126;
        height: 126;
        x: 97
        y: 97
        source: "contents/btncenterup.png"
        property bool pressed: false


        Image {
            id: lblplay
            x:12
            y:38
            property bool pressed: false

            source: "contents/lblplayup.png"

            SequentialAnimation {
                id:fadeplay
                PropertyAnimation {target: lblplay; property: "opacity"; to: 0; duration: 500}
                PropertyAnimation {target: lblplay; property: "opacity"; from: 0; to: 1; duration: 500}
                loops: Animation.Infinite
            }            
        }

        Image {
            id: lblpause
            x:65
            y:38
            property bool pressed: false

            source: "contents/lblpauseup.png"
            SequentialAnimation {
                id:fadepause
                PropertyAnimation {target: lblpause; property: "opacity"; to: 0; duration: 500}
                PropertyAnimation {target: lblpause; property: "opacity"; from: 0; to: 1; duration: 500}
                loops: Animation.Infinite
            }
        }

        MouseArea {
            id: mouseareacenter
            objectName: "btncenterm"
            anchors.fill: btncenter
            property int type: 0
            onPressed: {
                btncenter.source = "contents/btncenterdown.png";
            }
            onReleased: {
                btncenter.source = "contents/btncenterup.png";
            }

            Component.onCompleted: {
                mouseareacenter.clicked.connect(sendClick);
            }

            function sendClick(t){
                var sign = t==1?false:true;
                if(!lblplay.pressed){                   
                    if(lblpause.pressed==true){
                        if(sign)
                        mainwindow.sendmessage({'action':'endpause'});
                        lblpause.pressed = false;
                        fadepause.running = false;
                        lblpause.opacity = 1;
                        lblcafe.source = "contents/cafedown.png"
                        lblcafe.pressed = false;
                        lblrepas.source= "contents/repasdown.png"
                        lblrepas.pressed = false
                        btncenter.enabled=true;
                        btnadd.enabled=true
                        btnleft.enabled=true
                        btnright.enabled=true
                        btnup.enabled=true
                        quit.enabled = true;
                        lblcafe.enabled = true;
                        lblrepas.enabled = true;
                    }
                    else{
                        if(sign)
                        mainwindow.sendmessage({'action':'startreport'});
                    }
                    lblplay.pressed = true;
                    fadeplay.running = true;
                    lblplay.opacity = 0.6;
                    if(player.issimplify)
                    {
                        btncenter.enabled=false;
                        btnadd.enabled=false
                        btnleft.enabled=false
                        //btnright.enabled=false
                        btnup.enabled=false
                    }
                }
                else if(lblplay.pressed){
                    var error = false;
                    if(sign){
                        if(lblcafe.pressed)
                            mainwindow.sendmessage({'action':'startpausecafe'});
                        else if(lblrepas.pressed)
                            mainwindow.sendmessage({'action':'startpauserepas'});
                        else
                            error = true;

                    }
                    if(!error){
                        lblplay.pressed = false;
                        lblpause.pressed = true;
                        fadeplay.running = false;
                        fadepause.running = true;
                        lblplay.opacity = 1;
                        lblpause.opacity = 0.6;
                        btncenter.enabled=false;
                        btnadd.enabled=false
                        btnleft.enabled=false
                        btnright.enabled=false
                        btnup.enabled=false
                        quit.enabled = false;
                        lblcafe.enabled = false;
                        lblrepas.enabled = false;
                    }
                }
                else{
                    mainwindow.sendmessage({'action':'prout'});
                }
            }
        }

    }

    Image {
        id: lblcafe
        x:130
        y:220
        property bool pressed: false

        source: "contents/cafedown.png"
        /*SequentialAnimation {
            id:fadepausecafe
            PropertyAnimation {target: lblcafe; property: "opacity"; to: 0; duration: 500}
            PropertyAnimation {target: lblcafe; property: "opacity"; from: 0; to: 1; duration: 500}
            loops: Animation.Infinite
        }*/

        MouseArea {
            anchors.fill: lblcafe
            onClicked: {
                lblcafe.source = "contents/cafe.png"
                lblcafe.pressed = true;
                lblrepas.source= "contents/repasdown.png"
                lblrepas.pressed = false
            }
        }
    }

    Image {
        id: lblrepas
        x:170
        y:220
        property bool pressed: false

        source: "contents/repasdown.png"
        /*SequentialAnimation {
            id:fadepauserepas
            PropertyAnimation {target: lblrepas; property: "opacity"; to: 0; duration: 500}
            PropertyAnimation {target: lblrepas; property: "opacity"; from: 0; to: 1; duration: 500}
            loops: Animation.Infinite
        }*/

        MouseArea {
            anchors.fill: lblrepas
            onClicked: {
                lblcafe.source = "contents/cafedown.png"
                lblcafe.pressed = false;
                lblrepas.source= "contents/repas.png"
                lblrepas.pressed = true;
            }
        }
    }


    Image {
        id: btnup
        width: 161;
        height: 74;
        x: 80
        y: 26
        source: "contents/btnupup.png"
        property bool pressed: false

        Image {
            id: lblup
            x:65
            y:18
            source: "contents/lblupup.png"
        }


        MouseArea {
            anchors.fill: btnup
            onPressed: {btnup.source = "contents/btnupdown.png";}
            onReleased: {btnup.source = "contents/btnupup.png";}
            onClicked: {
                if(lblplay.pressed==true){
                    addpausedlg.visible = true;
                }
            }
        }
    }

    Image {
        id: btnleft
        width: 74
        height: 161
        x: 27
        y: 80
        source: "contents/btnleftup.png"
        property bool pressed: false

        Image {
            id: lblleft
            x:13
            y:68
            source: "contents/lblleftup.png"
        }

        MouseArea {
            anchors.fill: btnleft
            onPressed: {
                btnleft.source = "contents/btnleftdown.png";
                lblleft.source= "contents/lblleftdown.png";

            }
            onReleased:{
                btnleft.source = "contents/btnleftup.png";
                lblleft.source= "contents/lblleftup.png";
            }
        }
    }

    Image {
        id: btnright
        width: 74;
        height: 161;
        x: 220
        y: 80
        source: "contents/btnrightup.png"
        property bool pressed: false

        Image {
            id: lblright
            x:30
            y:65
            source: "contents/lblrightup.png"
        }


        MouseArea {
            id: mousearearight
            objectName: "btnright"
            anchors.fill: btnright
            onPressed: {
                btnright.source = "contents/btnrightdown.png";

            }
            onReleased:{
                btnright.source = "contents/btnrightup.png";
            }
            Component.onCompleted: {
                mousearearight.clicked.connect(sendStop);
            }

            function sendStop(t){
                var sign = t==1?false:true;
                if(sign)
                mainwindow.sendmessage({'action':'endreport'});
                lblpause.pressed = false;
                lblplay.pressed = false;
                fadeplay.running = false;
                lblplay.opacity = 1
                btncenter.enabled = false;
                btnright.enabled = false;
                btnleft.enabled = false;
            }
        }
    }

    Text {
        //anchors.left: parent.left
        objectName: "clock"
        id: clock
        width : 320
        height: 50
        x: 0
        y: 250
        color:"#4f4947"
        font.bold: true
        font.pointSize: 13
        horizontalAlignment: Text.AlignHCenter

        property date dat: new Date();
        text: {
            return "08:00";
        }

        function timeChanged() {
            dat = new Date();
            var datfin = circle.starttime;
            datfin.setHours(datfin.getHours()+circle.echelle);
            var delta =(datfin.getTime()-dat.getTime())/1000;
            var hours = Math.floor(delta/3600);
            var minutes =Math.floor((delta%3600)/60);
            var secondes = Math.floor(delta%60);
            if(delta<=0){
                hours = 0;
                minutes = 0;
                secondes = 0;
            }

            //clock.text = (hours<10?"0"+hours:hours)+":"+(minutes<10?"0"+minutes:minutes)+":"+(secondes<10?"0"+secondes:secondes);
            clock.text = (hours<10?"0"+hours:hours)+":"+(minutes<10?"0"+minutes:minutes);
        }
    }

    Rectangle {
        objectName: "grpprogressbar"
        id:grpprogressbar
        width : 180
        height: 100
        x: 70
        y: 240
        color: "transparent"
        visible: false;

        ProgressBar {
            objectName: "progressbar"
            id: progressbar
            width : 140
            height: 10
            x: 20
            y: 18
            value: 0
            visible: true;
            style: ProgressBarStyle {
                background: Rectangle {
                    radius: 2
                    color: "lightgray"
                    border.color: "gray"
                    border.width: 1
                    implicitWidth: 200
                    implicitHeight: 24
                }
                progress: Rectangle {
                    color: "#d07f58"
                    border.color: "#b25e35"
                }
            }
        }

        Text {
            x: 0
            y: 0
            text: "0 Min"
            color:"#4f4947"
            font.family: "Arial"
            font.bold: true
            font.pointSize: 11
        }
        Text {
            id:endmin
            objectName: "endmin"
            x: 130
            y: 0
            text: "15 Min"
            color:"#4f4947"
            font.family: "Arial"
            font.bold: true
            font.pointSize: 11
        }

        Text {
            id:currentvalue
            objectName: "currentvalue"
            x: 65
            y: 18
            text: ""
            color:"#4f4947"
            font.family: "Arial"
            font.bold: false
            font.pointSize: 7
        }
    }

    Dialog {
        id: addpausedlg        
        visible: false
        title: "Add pause dialog"

        property var locale: Qt.locale()

        contentItem: Rectangle {
            color: "steelblue"
            implicitWidth: 310
            implicitHeight: 200            

            Text {
                id:lblstarthour
                x:10
                y:15
                text: "Heure de début"
            }

            TextField  {
                id:starthour
                width:100
                height:30;
                x:100
                y:5
                placeholderText: new Date().toLocaleString(addpausedlg.locale,"hh:mm")
                //inputMask: "99:99"
                validator: RegExpValidator {
                    regExp: /[0-9][0-9]\:[0-9][0-9]/
                }

            }

            Text {
                id:lblendhour
                x:10
                y:55
                text: "Heure de fin"
            }

            TextField  {
                id:endhour
                width:100
                height:30;
                x:100
                y:45
                placeholderText: {
                    var dt = new Date();
                    dt.setMinutes(dt.getMinutes()+15);
                    dt.toLocaleString(addpausedlg.locale,"hh:mm")
                }
            }

            Text {
                id:lbltype
                x:10
                y:95
                text: "Type de pause"
            }

            ComboBox {
                id:cmbType
                width:200
                height:30
                x:100
                y:85
                property bool error : false
                model: ListModel {
                    id:list
                    ListElement { itemid:1; text:"Pause de 15m" }
                    ListElement { itemid:2; text: "Pause Déjeuner" }
                }
                style: ComboBoxStyle {
                    id: comboBox
                    background: Rectangle {
                        id: rectCategory
                        radius: 5
                        border.width: 2
                        border.color: control.error?"#ff0000":"#000000"
                        color: "#fff"

                        SequentialAnimation on border.color {
                            id: animateColor
                            running: false
                            ColorAnimation { to: "yellow"; duration: 5000 }
                            ColorAnimation { to: "blue"; duration: 5000 }
                        }
                    }
                    label: Text {
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 15
                        font.family: "Courier"
                        font.capitalization: Font.SmallCaps
                        color: "black"
                        text: control.currentText
                    }
                }
                function prout(){
                    console.log("1o");
                    cmbType.style.textColor = "red";
                }
                onCurrentIndexChanged: {
                    tmAnimate.stop();
                    cmbType.error = false;
                    if(tmAnimate.error30)
                        cmbTypeError30.visible = false;
                    else
                        cmbTypeError15.visible = false;
                }
            }

            Text {
                id:cmbTypeError15
                x:5
                y:120
                text: "Erreur le type de pause inplique une durée égale à 15 minutes"
                color: "red"
                visible: false
            }

            Text {
                id:cmbTypeError30
                x:5
                y:120
                text: "Erreur le type de pause inplique une durée superieure ou égale à 30 minutes"
                color: "red"
                visible: false
            }



            Button {
                id:btnadd
                x:40
                y:150
                text: "Ajouter"
                onClicked: {
                    var start;
                    var end;
                    if(starthour.text=="")
                        start = starthour.placeholderText;
                    else
                        start = starthour.text;

                    if(endhour.text=="")
                        end = endhour.placeholderText;
                    else
                        end = endhour.text;

                    var dtstart = new Date();
                    dtstart.setHours(start.substring(0,start.indexOf(":")))
                    dtstart.setMinutes(start.substr(start.indexOf(":")+1,2));
                    dtstart.setSeconds(0);
                    dtstart.setMilliseconds(0);
                    var dtend = new Date();
                    dtend.setHours(end.substring(0,end.indexOf(":")))
                    dtend.setMinutes(end.substr(end.indexOf(":")+1,2));
                    dtend.setSeconds(0);
                    dtend.setMilliseconds(0);

                    if(list.get(cmbType.currentIndex).itemid==2 && dtend-dtstart<30*60*1000){
                        tmAnimate.start();
                        tmAnimate.error30 = true;
                        cmbTypeError30.visible = true;
                    }
                    else if(list.get(cmbType.currentIndex).itemid==1 && dtend-dtstart!=15*60*1000){
                        tmAnimate.start();
                        tmAnimate.error30 = false;
                        cmbTypeError15.visible = true;
                    }
                    else{
                        addpausedlg.visible=false                        
                        mainwindow.sendmessage({'action':'addpause','startdate':dtstart,"enddate":dtend,"typepause":list.get(cmbType.currentIndex).itemid});
                    }
                }

                function clignote(){
                    if(cmbType.error)
                        cmbType.error = false;
                    else
                        cmbType.error = true;
                }

                Timer {
                    id:tmAnimate
                    property bool error30 : false
                    interval: 500; running: false; repeat: true;
                    onTriggered: {
                        var start;
                        var end;
                        if(starthour.text=="")
                            start = starthour.placeholderText;
                        else
                            start = starthour.text;

                        if(endhour.text=="")
                            end = endhour.placeholderText;
                        else
                            end = endhour.text;

                        var dtstart = new Date();
                        dtstart.setHours(start.substring(0,start.indexOf(":")))
                        dtstart.setMinutes(start.substr(start.indexOf(":")+1,2));
                        var dtend = new Date();
                        dtend.setHours(end.substring(0,end.indexOf(":")))
                        dtend.setMinutes(end.substr(end.indexOf(":")+1,2));

                        if(list.get(cmbType.currentIndex).itemid==2 && dtend-dtstart>=30*60*1000){
                            tmAnimate.stop();
                            cmbType.error = false;
                            if(error30)
                                cmbTypeError30.visible = false;
                            else
                                cmbTypeError15.visible = false;
                        }
                        else if(list.get(cmbType.currentIndex).itemid==0 && dtend-dtstart==15*60*1000){
                            tmAnimate.stop();
                            cmbType.error = false;
                            if(error30)
                                cmbTypeError30.visible = false;
                            else
                                cmbTypeError15.visible = false;
                        }
                        else
                            btnadd.clignote();
                    }
                }

            }

            Button {
                id:btncancel
                x:130
                y:150
                text: "Annuler"
                onClicked: {
                    addpausedlg.visible=false
                }

            }

        }
    }
}
