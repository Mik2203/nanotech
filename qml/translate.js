function autoTr(str) {
    return app.translator.currentLanguage+1>0 ? app.translator.emptyString + qsTr(str) : "";
}
