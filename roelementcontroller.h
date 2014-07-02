#ifndef ROVESSELCONTROLLER_H
#define ROVESSELCONTROLLER_H

#include <QObject>
#include <QStringList>
#include "roelement.h"
#include "rowarning.h"
//#include "rostagecontroller.h"

/* содержит логику выявления и сигналирования об ошибках и предупреждениях,
 * возникающих на элементе. Предупреждения представлены классом ROWarning.
 */

class ROStageController;
class ROElementController : public QObject {
    Q_OBJECT

    Q_PROPERTY(ROWarning* checkMaxFeedPressure READ checkMaxFeedPressure CONSTANT)
    Q_PROPERTY(ROWarning* checkMaxPressureDrop READ checkMaxPressureDrop CONSTANT)
    Q_PROPERTY(ROWarning* checkMaxFeedRate READ checkMaxFeedRate CONSTANT)
    Q_PROPERTY(ROWarning* checkMinConcentrate READ checkMinConcentrate CONSTANT)
    Q_PROPERTY(ROWarning* checkFlux READ checkFlux CONSTANT)
    Q_PROPERTY(ROWarning* checkMaxRecovery READ checkMaxRecovery CONSTANT)
    Q_PROPERTY(ROWarning* checkLSI READ checkLSI CONSTANT)
    Q_PROPERTY(ROWarning* checkSDSI READ checkSDSI CONSTANT)

    Q_PROPERTY(bool hasAnyCriticalWarnings READ hasAnyCriticalWarnings NOTIFY hasAnyCriticalWarningsChanged)
    Q_PROPERTY(bool hasAnyCautionWarnings READ hasAnyCautionWarnings NOTIFY hasAnyCautionWarningsChanged)
public:
    explicit ROElementController(ROElement *element, ROStageController* stageC);
    explicit ROElementController(); // for QML

    ~ROElementController();

    // WARNINGS
    ROWarning* const checkMaxFeedPressure() const; // CAUTION
    ROWarning* const checkMaxPressureDrop() const; // CAUTION
    ROWarning* const checkMaxFeedRate() const; // CAUTION
    ROWarning* const checkMinConcentrate() const; // CAUTION
    ROWarning* const checkFlux() const; // CAUTION
    //ROWarning* const checkMaxFlux() const; // CAUTION
    ROWarning* const checkMaxRecovery() const; // CAUTION
    ROWarning* const checkLSI() const; // CAUTION
    ROWarning* const checkSDSI() const; // CAUTION
    Q_INVOKABLE ROWarning* checkSaturation(int compound) const; // CAUTION

    bool hasAnyCriticalWarnings() const;
    bool hasAnyCautionWarnings() const;

    Q_INVOKABLE QStringList allWarningsMessages(ROWarning::WarningType type = ROWarning::WarningAll) const;


    ROElement* const element() const;
    ROStageController* const stageC() const;

signals:
    
    void hasAnyCriticalWarningsChanged();
    void hasAnyCautionWarningsChanged();

public slots:
    void updateWarnings();

private:
    ROElement* const _element;
    ROStageController* const _stageC;

    // WARNINGS
    ROWarning* _checkMaxFeedPressure;
    ROWarning* _checkMaxPressureDrop;
    ROWarning* _checkMaxFeedRate;
    ROWarning* _checkMinConcentrate;
    ROWarning* _checkFlux;
    ROWarning* _checkMaxRecovery;
    ROWarning* _checkLSI;
    ROWarning* _checkSDSI;
    QVector<ROWarning*> _checkSaturation;

};

#endif // ROVESSELCONTROLLER_H
