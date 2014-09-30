#ifndef ROSTAGECONTROLLER_H
#define ROSTAGECONTROLLER_H

#include <QObject>
#include "rostage.h"
#include "rowarning.h"
#include "ropasscontroller.h"
#include "roelementcontroller.h"

/* содержит логику выявления и сигналирования об ошибках и предупреждениях,
 * возникающих на стадии. Предупреждения представлены классом ROWarning.
 */

class ROStageController : public QObject {
    Q_OBJECT


    Q_PROPERTY(ROStage* stage READ stage CONSTANT)

    Q_PROPERTY(ROWarning* membraneChosen READ membraneChosen CONSTANT)


    Q_PROPERTY(ROPassController* passC READ passC CONSTANT)

    Q_PROPERTY(bool hasAnyCriticalWarnings READ hasAnyCriticalWarnings NOTIFY hasAnyCriticalWarningsChanged)
    Q_PROPERTY(bool hasAnyCautionWarnings READ hasAnyCautionWarnings NOTIFY hasAnyCautionWarningsChanged)
public:
    explicit ROStageController(ROStage* stage, ROPassController* passC);
    explicit ROStageController(); // for QML
    ~ROStageController();

    Q_INVOKABLE ROElementController* elementC(int elementCIndex) const;
    Q_INVOKABLE ROElementController* elementC(ROElement* element) const;

    // WARNINGS
    ROWarning* const membraneChosen() const; // CRITICAL

    bool hasAnyCriticalWarnings() const;
    bool hasAnyCautionWarnings() const;

    Q_INVOKABLE QStringList allWarningsMessages(ROWarning::WarningType type = ROWarning::WarningAll) const;


    ROStage* const stage() const;
    ROPassController* const passC() const;

signals:

    void hasAnyCriticalWarningsChanged();
    void hasAnyCautionWarningsChanged();

    void inputChanged();
    
public slots:
    void updateWarnings();

private slots:
    void updateElements();
    void updateRawWater();


private:
    ROStage* const _stage;
    ROPassController* const _passC;

    ROFlowMixer* const _rawWaterToFeed_SRT;

    QList<ROElementController*> _elementControllers;

    // WARNINGS
    ROWarning* _membraneChosen;


    
};

#endif // ROSTAGECONTROLLER_H
