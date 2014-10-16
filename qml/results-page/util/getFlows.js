function forElement(element, type) {
    if (element == sys) {
        if (type == "hydrodynamics")
            return [
                        { title: app.translator.emptyString + qsTr("Feed"), flow: sys.feed },
                        { title: app.translator.emptyString + qsTr("Stage 1 feed"), flow: sys.firstPass.firstStage.feed },
                        { title: app.translator.emptyString + qsTr("Permeate"), flow: sys.permeate },
                        { title: app.translator.emptyString + qsTr("Concentrate"), flow: sys.concentrate }
                    ]
        return [
                    { title: app.translator.emptyString + qsTr("Feed"), flow: sys.feed },
                    { title: app.translator.emptyString + qsTr("Adj. feed"), flow: sys.adjustedFeed },
                    { title: app.translator.emptyString + qsTr("Stage 1 feed"), flow: sys.firstPass.firstStage.feed },
                    { title: app.translator.emptyString + qsTr("Permeate"), flow: sys.permeate },
                    { title: app.translator.emptyString + qsTr("Concentrate"), flow: sys.concentrate }
                ]
    }
    return [
                { title: app.translator.emptyString + qsTr("Feed"), flow: element.feed },
                { title: app.translator.emptyString + qsTr("Permeate"), flow: element.permeate },
                { title: app.translator.emptyString + qsTr("Concentrate"), flow: element.concentrate }
            ]
}
