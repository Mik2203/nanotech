#-------------------------------------------------
#
# Project created by QtCreator 2011-08-11T18:58:08
#
#-------------------------------------------------
#QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__


QMAKE_CXXFLAGS_RELEASE += -DEIGEN_NO_DEBUG


QT       += core sql declarative

greaterThan(QT_MAJOR_VERSION, 4) {
    QT *= printsupport quick
}

CODECFORTR  = UTF-8

TARGET = nanotechpro
TEMPLATE = app


SOURCES += main.cpp \
    romembrane.cpp \
    romembranesize.cpp \
    roflow.cpp \
    rosystem.cpp \
    ropass.cpp \
    rostage.cpp \
    qwindowitem.cpp \
    qtoplevelwindow.cpp \
    roapplication.cpp \
    qwheelarea.cpp \
    qrangemodel.cpp \
    romembranesmodel.cpp \
    rosolutemodel.cpp \
    roflowmixer.cpp \
    rosolutemodelio.cpp \
    rowatermodel.cpp \
    rowarning.cpp \
    ropasscontroller.cpp \
    ropassparamsetcontroller.cpp \
    rosystemcontroller.cpp \
    rostagecontroller.cpp \
    rofeedcontroller.cpp \
    rowatertypemodel.cpp \
    roabstractelement.cpp \
    roelement.cpp \
    roelementcontroller.cpp \
    rocase.cpp \
    rounits.cpp \
    roproject.cpp \
    roprojectinfo.cpp \
    roxmlprojectserializer.cpp \
    roreportbuilder.cpp \
    roscalingelement.cpp \
    rodatabase.cpp \
    roabstractprojectserializer.cpp \
    qmlline.cpp \
    rotranslator.cpp \
    rounitstext.cpp \
    rosolutes.cpp \
    romembranesalternativesmodel.cpp \
    rogenericschemesmodel.cpp \
    rocosts.cpp \
    rosystemsolvescheduler.cpp \
    rosystemsolver.cpp \
    roqmlobjectcapturer.cpp \
    roprinter.cpp \
    roprojectmanager.cpp

HEADERS  += \
    romembrane.h \
    romembranesize.h \
    roflow.h \
    rosystem.h \
    ropass.h \
    rostage.h \
    qwindowitem.h \
    qtoplevelwindow.h \
    roapplication.h \
    qwheelarea.h \
    qrangemodel_p.h \
    qrangemodel.h \
    romembranesmodel.h \
    rosolutemodel.h \
    roflowmixer.h \
    rosolutemodelio.h \
    rowatermodel.h \
    rowarning.h \
    ropasscontroller.h \
    ropassparamsetcontroller.h \
    romath.h \
    rosystemcontroller.h \
    rostagecontroller.h \
    rofeedcontroller.h \
    rowatertypemodel.h \
    roabstractelement.h \
    roelement.h \
    roelementcontroller.h \
    rocase.h \
    rounits.h \
    roabstractprojectserializer.h \
    roproject.h \
    roprojectinfo.h \
    roxmlprojectserializer.h \
    roreportbuilder.h \
    roscalingelement.h \
    rodatabase.h \
    roabstractprojectserializer.h \
    qmlline.h \
    rotranslator.h \
    rounitstext.h \
    singleton.h \
    rosolutes.h \
    romembranesalternativesmodel.h \
    rogenericschemesmodel.h \
    rocosts.h \
    rosystemsolvescheduler.h \
    rosystemsolver.h \
    roqmlobjectcapturer.h \
    roprinter.h \
    roprojectmanager.h

OTHER_FILES += \
    changes_and_additions.txt \
    ParametersBar.qml \
    ROSchemeStage.qml \
    ROSchemePass.qml \
    ROSchemeSystem.qml \
    ROConfSystem.qml \
    ROSolutesTableView.qml \
    ROResults.qml \
    ROResultCell.qml \
    ROResultText.qml \
    ROConfPass.qml \
    ROConfStage.qml \
    widgets/ComboBoxButton.qml \
    widgets/ScrollBar.qml \
    widgets/ScrollAreaHelper.qml \
    widgets/ScrollArea.qml \
    widgets/ROListView.qml \
    widgets/ComboBox.qml \
    widgets/PopupField.qml \
    widgets/Button.qml \
    widgets/ROMembranesTable.qml \
    widgets/DoubleInput.qml \
    ROConfFeed.qml \
    widgets/TextInput.qml \
    qml/ROSolutesTableView.qml \
    qml/ROSchemeSystem.qml \
    qml/ROSchemeStage.qml \
    qml/ROSchemePass.qml \
    qml/ROResultText.qml \
    qml/ROResults.qml \
    qml/ROResultFeedText.qml \
    qml/ROResultCell.qml \
    qml/ROConfSystem.qml \
    qml/ROConfStage.qml \
    qml/ROConfPass.qml \
    qml/ROConfFeed.qml \
    qml/ParametersBar.qml \
    qml/FeedWaterPage.qml \
    qml/FeedBar.qml \
    qml/widgets/TextInput.qml \
    qml/widgets/TabFrame.qml \
    qml/widgets/Tab.qml \
    qml/widgets/ScrollBar.qml \
    qml/widgets/ScrollAreaHelper.qml \
    qml/widgets/ScrollArea.qml \
    qml/widgets/PopupField.qml \
    qml/widgets/DoubleInput.qml \
    qml/widgets/ComboBoxButton.qml \
    qml/widgets/CheckBox.qml \
    qml/widgets/ScrollableListView.qml \
    qml/widgets/WarningPopup.qml \
    qml/ROResultsStageData.qml \
    qml/ROResultsPassData.qml \
    qml/ROResultsTab.qml \
    qml/WarningResult.qml \
    qml/ROResultsWarnings.qml \
    qml/ROResultsHeaderButton.qml \
    qml/ROResultsElementData.qml \
    qml/MenuPanel/MenuPanel.qml \
    qml/MenuPanel/UnitsGrid.qml \
    qml/MenuPanel/ProjectButtonsGrid.qml \
    qml/MenuPanel/PrintPanel.qml \
    qml/ROProjectInfoWindow.qml \
    qml/widgets/TextEdit.qml \
    qml/widgets/DefaultListViewDelegate.qml \
    qml/widgets/ComboBox.qml \
    qml/widgets/Button.qml \
    qml/widgets/BasicButton.qml \
    qml/ROResultsSysData.qml \
    qml/MainWindow.qml \
    qml/ScalingPanel.qml \
    qml/widgets/TabBar.qml \
    qml/ScalingTable.qml \
    qml/widgets/InternalWindow.qml \
    qml/widgets/ExternalWindow.qml \
    qml/ROMembraneParametersTable.qml \
    qml/ROMembraneParametersCell.qml \
    qml/ROMembraneAlternativesTable.qml \
    qml/widgets/SimpleListView.qml \
    qml/ROSolutesTreeView.qml \
    qml/ROGenericSchemesTreeView.qml \
    qml/ROCosts.qml \
    qml/ROCostsRowHeader.qml \
    qml/ROCostsColHeader.qml \
    qml/ROCostsEditableCell.qml \
    qml/ROCostsReadonlyCell.qml \
    qml/ROSchemeWarnings.qml \
    qml/widgets/Slider.qml \
    qml/translate.js \
    qml/HelloWindow.qml \
    qml/AboutWindow.qml \
    app_icon.rc \
    qml/widgets/BorderPanel.qml \
    qml/widgets/BorderText.qml \
    qml/results-page/Data.qml \
    qml/results-page/Warnings.qml \
    qml/results-page/Tab.qml \
    qml/results-page/system/Common.qml \
    qml/results-page/system/Data.qml \
    qml/results-page/util/TableRowTitle.qml \
    qml/results-page/util/TableRowUnits.qml \
    qml/results-page/util/TableRowValue.qml \
    qml/results-page/system/Hydrodynamics.qml \
    qml/results-page/common/FlowHydrodynamics.qml \
    qml/results-page/common/FlowSolubility.qml \
    qml/results-page/common/FlowScaling.qml \
    qml/results-page/util/getFlows.js \
    qml/results-page/pass/Data.qml \
    qml/results-page/common/Solubility.qml \
    qml/results-page/common/Scaling.qml \
    qml/results-page/stage/Data.qml \
    qml/results-page/element/Data.qml \
    qml/results-page/util/SectionTitle.qml \
    qml/results-page/util/ElementTitle.qml \
    qml/results-page/pass/CommonHeader.qml \
    qml/results-page/pass/CommonData.qml \
    qml/results-page/pass/FlowsHeader.qml \
    qml/results-page/pass/FlowsData.qml \
    qml/results-page/stage/CommonHeader.qml \
    qml/results-page/stage/CommonData.qml \
    qml/results-page/stage/FlowsData.qml \
    qml/results-page/stage/FlowsHeader.qml \
    qml/results-page/element/CommonData.qml \
    qml/results-page/element/CommonHeader.qml

TRANSLATIONS += nano_ru.ts # for update utilite
win32:RC_FILE = app_icon.rc # for windows icon

release {
    RESOURCES += resources.qrc
}

#for Windows
win32:debug {
    #in Windows, you can use & to separate commands
    #CONFIG += console
#    copyfiles.commands += @echo $${TARGET} &
    copyfiles.commands += @call xcopy /S /Y ..\\$${TARGET}\\qml .\\$${DESTDIR}\\qml &
    #copyfiles.commands += @call xcopy /S /Y ..\\$${TARGET}\\images .\\$${DESTDIR}\\images &
}
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles







# UNCOMMENT TO USE INTEL MKL
#DEFINES += "EIGEN_USE_MKL_ALL"
#win32 {
#    INCLUDEPATH += "C:/Program Files (x86)/Intel/Composer XE 2013/mkl/include"
#    DEPENDPATH += "C:/Program Files (x86)/Intel/Composer XE 2013/mkl/include"

#    INCLUDEPATH += "c:/Program Files (x86)/Intel/Composer XE 2013/compiler/include"
#    DEPENDPATH += "c:/Program Files (x86)/Intel/Composer XE 2013/compiler/include"

#    LIBS += -L"C:/Program Files (x86)/Intel/Composer XE 2013/mkl/lib/ia32/" -lmkl_core -lmkl_intel_thread -lmkl_intel_c
#    LIBS += -L"c:/Program Files (x86)/Intel/Composer XE 2013/compiler/lib/ia32/" -llibiomp5md

#    PRE_TARGETDEPS += "C:/Program Files (x86)/Intel/Composer XE 2013/mkl/lib/ia32/mkl_core.lib"
#    PRE_TARGETDEPS += "C:/Program Files (x86)/Intel/Composer XE 2013/mkl/lib/ia32/mkl_intel_thread.lib"
#    PRE_TARGETDEPS += "C:/Program Files (x86)/Intel/Composer XE 2013/mkl/lib/ia32/mkl_intel_c.lib"
#    PRE_TARGETDEPS += "c:/Program Files (x86)/Intel/Composer XE 2013/compiler/lib/ia32/libiomp5md.lib"
#}


#UNCOMMENT TO USE OPENMP
win32 {
    QMAKE_CXXFLAGS += /openmp
    CONFIG(release, debug|release): LIBS += -L$$PWD/omp/ -lvcomp
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/omp/ -lvcompd
    INCLUDEPATH += $$PWD/omp
    DEPENDPATH += $$PWD/omp
    CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/omp/vcomp.lib
    else:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/omp/vcompd.lib
}

win32 {
    LIBS += -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/" -lshell32
    INCLUDEPATH += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include"
    DEPENDPATH += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include"
    PRE_TARGETDEPS += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/shell32.lib"
}
