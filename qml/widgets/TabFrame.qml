import QtQuick 1.1

Item {
    id: tabWidget
    width: 100
    height: 100
    property int current: 0
    property int count: stack.children.length
    property bool frame: true
    default property alias tabs : stack.children
    property Item tabBar: tabbarItem

    onCurrentChanged: __setOpacities()
    Component.onCompleted: __setOpacities()

    function __setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].enabled = stack.children[i].visible = (i == current ? true : false)
        }
    }

    Component {
        id: tabcomp
        Tab {}
    }

    function addTab(component, title) {
        var tab = tabcomp.createObject(this);
        component.createObject(tab)
        tab.parent = stack
        tab.title = title
        __setOpacities()
        return tab
    }

    function removeTab(id) {
        var tab = tabs[id]
        tab.destroy()
        if (current > 0)
            current-=1
    }

    Rectangle { //
        id: frameitem
        //anchors.fill: parent
        anchors.top: tabbarItem.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        property int frameWidth: frame ? 1 : 0
        border.color: "grey"
        border.width: frameWidth
        color: "transparent"
//        radius: 4


        Item {
            id: stack
            anchors.fill: parent
            anchors.margins: frameitem.frameWidth
        }


    }

    TabBar {
        id: tabbarItem
        tabFrame: tabWidget
        anchors.top: tabWidget.top
        anchors.left: tabWidget.left
        anchors.right: tabWidget.right
    }
}
