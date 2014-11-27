import QtQuick 1.1

Row {
    CommonHeader {}
    Repeater {
        model: sys.passCount
        CommonData { pass: sys.pass(index) }
    }
}
