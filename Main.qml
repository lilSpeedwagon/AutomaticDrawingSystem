import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.1

ApplicationWindow {

    id: mainWindow
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("ADS (debug)")

    Connections {
        target: ADSCore

        onSignalLog:   {

        }

        onSignalDraw:   {

        }

    }

    function ready()    {
        ADSCore.ready();
    }

    Canvas  {
        id: canvas
        width: 800
        height: 600
        antialiasing: true
        property var context: null
        property real currentX: 0
        property real currentY: 0

        onPaint: {

        }

        function drawTo(x, y)  {
            context.lineTo(x,y);
            ready();
        }

        function moveTo(x, y)   {
            context.closePath();
            context.moveTo(x,y);
            context.beginPath();
            ready();
        }

        Component.onCompleted: {
            context = getContext("2d");
            context.beginPath();
        }
    }


    Component.onCompleted: {


    }
}
