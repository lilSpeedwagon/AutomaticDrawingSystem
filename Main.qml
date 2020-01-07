import QtQml 2.3
import QtQuick 2.12
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

    }

    Connections {
        target: VirtualDrawer

        onSignalDrawTo:   {
            console.log("draw");
            canvas.currentX = x;
            canvas.currentY = y;
            canvas.currentDraw = true;
            canvas.drawCurrentPoint();
        }

        onSignalMoveTo: {
            console.log("move");
            canvas.currentX = x;
            canvas.currentY = y;
            canvas.currentDraw = false;
            canvas.drawCurrentPoint();
        }
    }

    function ready()    {
        VirtualDrawer.ready();
    }

    /*PathView    {
        width: 800
        height: 600
        property real lastX: 0
        property real lastY: 0
        property Path lastPath: null

        function moveTo(x, y)   {
            lastPath = new Path;
            path.startX = x;
            path.startY = y;
        }

        function drawTo(x, y)   {
            var line = new PathLine(x ,y);
            lastPath.pathElements

        }
    }*/

    Canvas  {
        id: canvas
        width: 800
        height: 600
        contextType: qsTr("")
        visible: true
        anchors.fill: parent
        antialiasing: true
        renderStrategy: Canvas.Cooperative
        renderTarget: Canvas.FramebufferObject
        property real currentX: 0
        property real currentY: 0
        property bool currentDraw: false
        property bool pathOpened: false

        function drawCurrentPoint()    {
            if (canvas.available)   {
                var ctx = canvas.getContext('2d');
                ctx.lineWidth = 2;
                ctx.strokeStyle = '#8080F0';

                if (!currentDraw)   {
                    moveTo(ctx, currentX, currentY);
                }   else    {
                    drawTo(ctx, currentX, currentY);
                }
                requestPaint();
            }   else    {
                console.log("canvas is unavaliable");
            }
        }

        function drawTo(ctx, x, y)  {
            ctx.lineTo(x,y);
            ctx.stroke();
        }

        function moveTo(ctx, x, y)   {
            if (pathOpened) {
                ctx.closePath();
                pathOpened = false;
            }
            ctx.beginPath();
            pathOpened = true;
            ctx.moveTo(x,y);
        }

        Component.onCompleted: {

        }
    }

    Button {
        id: button
        x: 8
        y: 482
        text: qsTr("Button")

        onClicked: {
            ADSCore.testProcess();
        }
    }


    Component.onCompleted: {

    }
}
