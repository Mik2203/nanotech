import QtQuick 1.1
import "../widgets" as ROWidgets

Item {
    id: tabbar

    function selectNextTab() {
        if (tabFrame && tabFrame.current < tabFrame.count - 1) {
            var index = tabFrame.current + 1
            if (index == tabFrame.count - 1 && !tabFrame.tabs[index].active) return false;
            while (!tabFrame.tabs[index].active && index < tabFrame.count) ++index;
            if (index < tabFrame.count) {
                tabFrame.current = index
                return true;
            }
        }
        return false;
    }

    function selectPrevTab() {
        if (tabFrame && tabFrame.current > 0) {
            var index = tabFrame.current - 1
            while (!tabFrame.tabs[index].active && index >= 0) --index;
            if (index >= 0) {
                tabFrame.current = index
                return true;
            }
        }
        return false;
    }

    Keys.onRightPressed: selectNextTab()
    Keys.onLeftPressed: selectPrevTab()


    height: tabrow.height
    width: tabrow.width

    property Item tabFrame
    onTabFrameChanged: parent = tabFrame

    function tab(index) {
        for (var i = 0; i < tabrow.children.length; ++i) {
            if (tabrow.children[i].tabindex == index) {
                return tabrow.children[i]
            }
        }
        return null;
    }

    Row {
        id: tabrow
        height: 25
        x: 5
        //spacing: -3

        Repeater {
            id:repeater
            focus:true
            model: tabFrame ? tabFrame.tabs.length : null
            delegate: ROWidgets.Button {
                id:tab

                property int tabindex: index
                property bool selected: tabFrame.current == index
                active: tabFrame.tabs[index].active
                height: tabrow.height
                text:  tabFrame.tabs[index].title
                onClicked: tabFrame.current = index
                toggled: selected
                textFont.pixelSize: selected ? 13 : 11
                textFont.bold: selected
                textHorizontalAlignment: (img.status == Image.Ready) ? Qt.AlignRight : Qt.AlignHCenter
                textRightMargin: (img.status == Image.Ready) ? 5 : 0
                width: textWidth + 20 + (img.status == Image.Ready ? img.width - 5 : 0)
                radius: 0

                Image {
                    id: img
                    width: tabFrame.tabs[index].imageWidth
                    height: tabFrame.tabs[index].imageHeight
                    smooth: true
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    source: tabFrame.tabs[index].imageSource
                }

                Connections {
                    target: tabFrame.tabs[index]
                    onActiveChanged: if (selected && !tabFrame.tabs[index].active) if (!selectNextTab()) selectPrevTab()
                }
            }
        }
    }
}
