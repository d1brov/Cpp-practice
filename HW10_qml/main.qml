import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt.labs.folderlistmodel

ApplicationWindow {
    id: window
    title: "QtQuick Image Viewer"
    width: 800
    height: 600
    visible: true

    menuBar: MenuBar {
            Menu {
                title: qsTr("Folder")
                MenuItem {
                    text: qsTr("Open...")
                    onTriggered: folderModel.folder=folderDialog.open()
                }
            }
            Menu {
                title: qsTr("View")
                MenuItem {
                    text: qsTr("List")
                    onTriggered: {
                        hideAllViews()
                        listViewRect.visible = true
                    }
                }
                MenuItem {
                    text: qsTr("Grid")
                    onTriggered: {
                        hideAllViews()
                        gridViewRect.visible = true;
                    }
                }
                MenuItem {
                text: qsTr("PathView")
                onTriggered: {
                    hideAllViews()
                    pathViewRect.visible = true;
                }
            }
            }
        }

    FolderListModel {
        id: folderModel
        showDirs: false
        nameFilters: [ "*.png", "*.jpeg", "*.jpg"]
    }

    FolderDialog {
        id: folderDialog
        onAccepted: {
            folderModel.folder = folderDialog.selectedFolder;
        }
    }

    function hideAllViews() {
        listViewRect.visible = false;
        gridViewRect.visible = false;
        pathViewRect.visible = false;
    }

    ///////////////////////////////////////////////////////////////////////////
    // LIST VIEW
    Rectangle {
        id: listViewRect
        anchors.fill: parent
        color: Qt.rgba(200/255, 80/255, 80/255, 1)
        visible: false

        ListView {
            id: listView
            anchors.fill: listViewRect
            spacing: 5
            anchors.topMargin: listView.spacing
            anchors.leftMargin: listView.spacing

            model: folderModel
            delegate: Rectangle {   // Row just does not work here
                id: listViewRow
                width: listView.width - listView.spacing
                height: listView.height / 6
                color: Qt.rgba(1, 150/255, 150/255, .9)

                MouseArea {
                    anchors.fill: listViewRow;
                    onReleased: showFullscreenImage(listViewItemImage);
                }

                Image {
                    id: listViewItemImage
                    width: listViewRow.height
                    height: listViewRow.height
                    anchors.left: listViewRow.left
                    fillMode: Image.PreserveAspectFit
                    source: fileUrl
                }

                Text {
                    id: listViewItemText
                    anchors.left: listViewItemImage.right
                    anchors.verticalCenter: listViewItemImage.verticalCenter
                    anchors.leftMargin: listView.spacing
                    height: listViewItemImage.height * 0.1
                    text: fileName
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // GRID VIEW
    Rectangle {
        id: gridViewRect
        anchors.fill: parent
        color: Qt.rgba(40/255, 120/255, 40/255, 1)
        visible: true

        GridView {
            id: gridView
            anchors.fill: gridViewRect
            cellWidth: (800 - cellSpacing) / 5
            cellHeight: cellWidth
            anchors.topMargin: gridView.cellSpacing
            anchors.leftMargin: gridView.cellSpacing

            property int cellSpacing: 5
            property int textHeight: 10

            model: folderModel
            delegate: Rectangle {
                id: gridCell
                width: gridView.cellWidth - gridView.cellSpacing * 2
                height: gridView.cellHeight - gridView.cellSpacing * 2
                color: Qt.rgba(50/255, 180/255, 50/255, 0.7)

                MouseArea {
                    anchors.fill: gridCell;
                    onReleased: showFullscreenImage(gridViewItemImage);
                }

                Image {
                    id: gridViewItemImage
                    width: gridCell.width - gridView.cellSpacing * 2
                    height: gridCell.height - gridView.textHeight - gridView.cellSpacing * 4

                    anchors.horizontalCenter: gridCell.horizontalCenter
                    anchors.top: gridCell.top
                    anchors.topMargin: gridView.cellSpacing

                    fillMode: Image.PreserveAspectFit
                    source: fileUrl
                }

                Text {
                    anchors.top: gridViewItemImage.bottom
                    anchors.topMargin: gridView.cellSpacing
                    anchors.horizontalCenter: gridViewItemImage.horizontalCenter
                    height: gridView.textHeight

                    // IDK eliding does not work with centering
                    //width: gridCell.width - gridView.cellSpacing * 2
                    //elide: Text.ElideRight

                    text: fileName
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // PATH VIEW
    Rectangle {
        id: pathViewRect
        anchors.fill: parent
        color: Qt.rgba(50/255, 50/255, 160/255, .7)
        visible: false

        property int imgSize: pathViewRect.width/3

        Component {
            id: delegate
            Rectangle {
                id: pathViewCell
                opacity: PathView.isCurrentItem ? 1 : 0.33

                Image {
                    id: pathViewImage
                    width: pathViewRect.imgSize
                    height: pathViewRect.imgSize

                    fillMode: Image.PreserveAspectFit
                    source: fileUrl

                    MouseArea {
                        anchors.fill: parent;
                        onReleased: showFullscreenImage(pathViewImage);
                    }
                }
                Text {
                    anchors.top: pathViewImage.bottom
                    anchors.horizontalCenter: pathViewImage.horizontalCenter
                    text: fileName
                }
            }
        }

        PathView {
            id: pathView
            anchors.fill: parent
            pathItemCount: 5

            focus: true
            Keys.onLeftPressed: decrementCurrentIndex()
            Keys.onRightPressed: incrementCurrentIndex()


            highlightRangeMode: PathView.StrictlyEnforceRange
            preferredHighlightBegin: .5
            preferredHighlightEnd: .5


            model: folderModel
            delegate: delegate
            path: Path
            {
                startX: -pathViewRect.imgSize/2
                startY: 0
                PathQuad {
                    x: pathView.width
                    y: 0
                    controlX: pathView.width/2 - pathViewRect.imgSize
                    controlY: pathView.height*0.75
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // MAXIMIZED IMAGE
    Image {
        id: fullscreenImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        visible: false
        MouseArea {
            anchors.fill: parent
            onReleased: hideFullscreenImage();
        }
    }

    function showFullscreenImage(img) {
        fullscreenImage.source = img.source
        fullscreenImage.visible = true;
    }

    function hideFullscreenImage() {
        fullscreenImage.visible = false;
    }
}
