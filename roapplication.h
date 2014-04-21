#ifndef ROAPPLICATION_H
#define ROAPPLICATION_H

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtDeclarative>

#include <QSettings>

#include "roprojectmanager.h"
#include "rotranslator.h"
//
#include "rounits.h"
#include "rosystemsolvescheduler.h"

#include "roprinter.h"

/* содержит и управл€ет основными сервисами приложени€:
Ц	”правление базой данных (RODatabase)
Ц	”правление файлами проекта (ROProjectManager)
Ц	”правление печатью (ROPrinter)
Ц	”правление единицами измерени€ (ROUnits)
Ц	”правление переводами (ROTranslator)
 */


#define roApp (static_cast<ROApplication *>(qApp))

class ROApplication : public QApplication {
    Q_OBJECT

    Q_PROPERTY(ROProjectManager* projectManager READ projectManager CONSTANT)
    Q_PROPERTY(ROTranslator* translator READ translator CONSTANT)
    Q_PROPERTY(ROPrinter* printer READ printer CONSTANT)
    Q_PROPERTY(int visibleDecimals READ visibleDecimals WRITE setVisibleDecimals NOTIFY visibleDecimalsChanged)
    Q_PROPERTY(bool showHelloMessage READ showHelloMessage WRITE setShowHelloMessage NOTIFY showHelloMessageChanged)

    Q_PROPERTY(int MAX_VISIBLE_DECIMALS READ MAX_VISIBLE_DECIMALS CONSTANT)
    Q_PROPERTY(int MIN_VISIBLE_DECIMALS READ MIN_VISIBLE_DECIMALS CONSTANT)

    Q_PROPERTY(ROUnits* units READ units CONSTANT)



public:
    explicit ROApplication(int & argc, char ** argv);
    ~ROApplication();

    static int MAX_VISIBLE_DECIMALS() { return _MAX_VISIBLE_DECIMALS; }
    static int MIN_VISIBLE_DECIMALS() { return _MIN_VISIBLE_DECIMALS; }


    // UI
    int visibleDecimals() const;
    void setVisibleDecimals(int value);

    bool showHelloMessage() const;
    void setShowHelloMessage(bool value);

    ROProjectManager* const projectManager() const;

    ROUnits* const units() const;

    ROTranslator* const translator() const;

    ROPrinter* const printer() const;

    ROQMLObjectCapturer * const schemeCapturer() const;


private:
    static const int _MAX_VISIBLE_DECIMALS;
    static const int _MIN_VISIBLE_DECIMALS;

    void registerTypes();
    void registerTranslator();
    void registerGUI();
    void registerContextGlobals();

    QDeclarativeEngine *_engine;
    QGraphicsObject *_rootObject;
    QDeclarativeComponent *_component;

    ROQMLObjectCapturer* _schemeCapturer;

    QStringList _errors;
    QSqlDatabase db;

    // UI
    int _visibleDecimals;


    ROProjectManager* _projectManager;

    // Translations
    ROTranslator* const _translator;
    ROPrinter* _printer;

    // Units
    ROUnits* _units;

    // Settings
    QSettings _settings;

    bool _showHelloMessage;

signals:
    void visibleDecimalsChanged();
    void showHelloMessageChanged();

private slots:
    void continueExecute();
    void loadSettings();
    void saveSettings();

public slots:
    // Help
    void help();
};

#endif // ROAPPLICATION_H
