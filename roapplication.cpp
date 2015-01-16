#include "roapplication.h"

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShellAPI.h>
#endif


#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


#include "qwindowitem.h"
#include "qwheelarea.h"
#include "qrangemodel.h"
#include "qmlline.h"

#include "rodatabase.h"

#include "roelement.h"
#include "ropass.h"
#include "rocosts.h"
#include "ropassparamsetcontroller.h"
#include "rostage.h"
#include "roflow.h"
#include "rosolutemodel.h"
#include "rowatertypemodel.h"
#include "romembranesalternativesmodel.h"
#include "rogenericschemesmodel.h"
#include "romembrane.h"
#include "roscalingelement.h"

#include "roqmlobjectcapturer.h"

#include "rosolutemodelio.h"
#include "rounitstext.h"

#include "rosystemcontroller.h"
#include "ropasscontroller.h"
#include "rostagecontroller.h"
#include "rofeedcontroller.h"

#include "rowarning.h"



#include "rosystemsolver.h"
#include "rosystemsolvescheduler.h"
#include "roreportbuilder.h"


const int ROApplication::_MAX_VISIBLE_DECIMALS = 10;
const int ROApplication::_MIN_VISIBLE_DECIMALS = 0;


ROApplication::ROApplication(int & argc, char ** argv) : QApplication(argc, argv),
    _engine(new QDeclarativeEngine(this)),
    _translator(new ROTranslator()),
    _visibleDecimals(2),
    _showHelloMessage(true),
    _settings(QSettings::IniFormat, QSettings::UserScope,
              "RM Nanotech", "Nanotech PRO"),
    _units(ROUnits::instance()) {

    _schemeCapturer = new ROQMLObjectCapturer(_engine);

    // TODO CHECK !!!
    if (!RODatabase::setDatabase("nano.db")) this->exit();
    registerTypes();
    registerTranslator();


    _projectManager = new ROProjectManager();

    _printer = new ROPrinter();

    registerContextGlobals();
    registerGUI();

    loadSettings();
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(saveSettings()));
}

ROApplication::~ROApplication() {
    RODatabase::free();
    ROUnits::free();

    delete _translator;
    delete _schemeCapturer;

    // TODO DELETE ENGINE ETC... ?
}

ROQMLObjectCapturer * const ROApplication::schemeCapturer() const { return _schemeCapturer; }

void ROApplication::registerContextGlobals() {
    _engine->rootContext()->setContextProperty("app", this);
    _engine->rootContext()->setContextProperty("db", RODatabase::instance());
    _engine->rootContext()->setContextProperty("unitsText", ROUnitsText::instance());
    _engine->rootContext()->setContextProperty("schemeCapturer", schemeCapturer());

    ROSoluteModelIO* solutesIO = new ROSoluteModelIO(this);
    _engine->rootContext()->setContextProperty("solutesIO", solutesIO);

}

void ROApplication::registerGUI() {
#ifdef QT_DEBUG
    _component = new QDeclarativeComponent(_engine, QUrl("qml/MainWindow.qml"));
#else
    _component = new QDeclarativeComponent(_engine, QUrl("qrc:/qml/MainWindow.qml"));
#endif
    if (!_component->isLoading()) {
        continueExecute();
    } else {
        connect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(continueExecute()));
    }
}

void ROApplication::registerTypes() {
    qmlRegisterType<QWindowItem>("Window", 0, 1, "Window");
    qmlRegisterType<QWheelArea>("WheelArea", 0, 1, "WheelArea");
    qmlRegisterType<QRangeModel>("RangeModel", 0, 1, "RangeModel");
    qmlRegisterType<qmlLine>("Line", 1, 0, "Line");
    qmlRegisterType<ROPrinter>("ROPrinter", 1, 0, "ROPrinter");

    qmlRegisterType<ROSystemController>("ROSystemController", 1, 0, "ROSystemController");
    qmlRegisterType<ROPassController>("ROPassController", 1, 0, "ROPassController");
    qmlRegisterType<ROStageController>("ROStageController", 1, 0, "ROStageController");
    qmlRegisterType<ROFeedController>("ROFeedController", 1, 0, "ROFeedController");
    qmlRegisterType<ROElementController>("ROElementController", 1, 0, "ROElementController");
    qmlRegisterType<ROPassParamSetController>("ROPassParamSetController", 1, 0, "ROPassParamSetController");

    qmlRegisterType<ROProject>("ROProject", 1, 0, "ROProject");
    qmlRegisterType<ROProjectManager>("ROProjectManager", 1, 0, "ROProjectManager");
    qmlRegisterType<ROTranslator>("ROTranslator", 1, 0, "ROTranslator");
    qmlRegisterType<ROProjectInfo>("ROProjectInfo", 1, 0, "ROProjectInfo");
    qmlRegisterType<ROCase>("ROCase", 1, 0, "ROCase");
    qmlRegisterType<ROCosts>("ROCosts", 1, 0, "ROCosts");
    qmlRegisterType<ROSystem>("ROSystem", 1, 0, "ROSystem");
    qmlRegisterType<ROSystemSolveScheduler>("ROSystemSolveScheduler", 1, 0, "ROSystemSolveScheduler");
    qmlRegisterType<ROSystemSolver>("ROSystemSolver", 1, 0, "ROSystemSolver");
    qmlRegisterType<ROPass>("ROPass", 1, 0, "ROPass");
    qmlRegisterType<ROElement>("ROElement", 1, 0, "ROElement");
    qmlRegisterType<ROScalingElement>("ROScalingElement", 1, 0, "ROScalingElement");
    qmlRegisterType<ROStage>("ROStage", 1, 0, "ROStage");
    qmlRegisterType<ROFlow>("ROFlow", 1, 0, "ROFlow");
    qmlRegisterType<ROMembrane>("ROMembrane", 1, 0, "ROMembrane");
    qmlRegisterType<ROSolutes>("ROSolutes", 1, 0, "ROSolutes");
    qmlRegisterType<ROSoluteModel>("ROSoluteModel", 1, 0, "ROSoluteModel");
    qmlRegisterType<ROWaterTypeModel>("ROWaterTypeModel", 1, 0, "ROWaterTypeModel");
    qmlRegisterType<ROMembranesModel>("ROMembranesModel", 1, 0, "ROMembranesModel");
    qmlRegisterType<ROMembranesAlternativesModel>("ROMembranesAlternativesModel", 1, 0, "ROMembranesAlternativesModel");
    qmlRegisterType<ROGenericSchemesModel>("ROGenericSchemesModel", 1, 0, "ROGenericSchemesModel");
    qmlRegisterType<ROReportBuilder>("ROReportBuilder", 1, 0, "ROReportBuilder");

    qmlRegisterType<ROWarning>("ROWarning", 1, 0, "ROWarning");

    qmlRegisterType<ROUnits>("ROUnits", 1, 0, "ROUnits");
    qmlRegisterType<ROUnitsText>("ROUnitsText", 1, 0, "ROUnitsText");

    // IO
    qmlRegisterType<ROWaterModel>("ROWaterModel", 1, 0, "ROWaterModel");


}

void ROApplication::registerTranslator() {
    QTextCodec *unicodeCodec = QTextCodec::codecForName("UTF-8");
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(unicodeCodec);
    QTextCodec::setCodecForCStrings(unicodeCodec);
#endif
    QTextCodec::setCodecForLocale(unicodeCodec);

    connect(translator(), SIGNAL(currentLanguageChanged()), ROUnitsText::instance(), SLOT(update()));
}

void ROApplication::continueExecute()
{
    disconnect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(continueExecute()));

    if (_component->isError()) {
        QList<QDeclarativeError> errorList = _component->errors();
        foreach (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        return;
    }

    QObject *obj = _component->create();
    obj->setParent(_engine);

    if(_component->isError()) {
        QList<QDeclarativeError> errorList = _component->errors();
        Q_FOREACH (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        return;
    }

    _rootObject = qobject_cast<QGraphicsObject*>(obj);
    Q_ASSERT(_rootObject);
    this->setWindowIcon(QIcon(":/images/logo.png"));
}

void ROApplication::loadSettings() {
    _showHelloMessage = _settings.value("showHelloMessage", true).toBool();
    _translator->setCurrentLanguage(static_cast<ROTranslator::Language>(_settings.value("language", ROTranslator::EN).toInt()));
    _settings.beginGroup("units");
    _units->setFlowUnits(static_cast<ROUnits::FlowUnits>(_settings.value("flow", ROUnits::m3h).toInt()));
    _units->setFluxUnits(static_cast<ROUnits::FluxUnits>(_settings.value("flux", ROUnits::lmh).toInt()));
    _units->setPressureUnits(static_cast<ROUnits::PressureUnits>(_settings.value("pressure", ROUnits::bar).toInt()));
    _units->setTemperatureUnits(static_cast<ROUnits::TemperatureUnits>(_settings.value("temperature", ROUnits::Celsius).toInt()));
    _settings.endGroup();
}

void ROApplication::saveSettings() {
    _settings.setValue("showHelloMessage", _showHelloMessage);
    _settings.setValue("language", _translator->currentLanguage());
    _settings.beginGroup("units");
    _settings.setValue("flow", _units->flowUnits());
    _settings.setValue("flux", _units->fluxUnits());
    _settings.setValue("pressure", _units->pressureUnits());
    _settings.setValue("temperature", _units->temperatureUnits());
    _settings.endGroup();
}



int ROApplication::visibleDecimals() const { return _visibleDecimals; }

void ROApplication::setVisibleDecimals(int value) {
    if (_visibleDecimals != value) {
        _visibleDecimals = qBound(_MIN_VISIBLE_DECIMALS, value, _MAX_VISIBLE_DECIMALS);
        Q_EMIT visibleDecimalsChanged();
    }
}

ROUnits *const ROApplication::units() const { return _units; }



ROTranslator* const ROApplication::translator() const { return _translator; }

ROPrinter *const ROApplication::printer() const { return _printer; }

bool ROApplication::showHelloMessage() const { return _showHelloMessage; }

void ROApplication::setShowHelloMessage(bool value) {
    _showHelloMessage = value;
    Q_EMIT showHelloMessageChanged();
}

ROProjectManager *const ROApplication::projectManager() const { return _projectManager; }





void ROApplication::help() {
#ifdef Q_OS_WIN
    // вызов без консоли.
    ShellExecute(NULL, L"open", L"help\\doc.pdf", NULL, NULL, SW_SHOW );
#else
    system("start help/doc.pdf");
#endif
}




