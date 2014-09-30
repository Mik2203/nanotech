#ifndef ROPASSCONTROLLER_H
#define ROPASSCONTROLLER_H

#include <QObject>

#include "ropass.h"
#include "ropassparamsetcontroller.h"
#include "rostagecontroller.h"
#include "rosystemcontroller.h"
#include "rowarning.h"

/* содержит логику выявления и сигналирования об ошибках и предупреждениях,
 * возникающих на ступени. Предупреждения представлены классом ROWarning.
 */

class ROPassController : public QObject {
    Q_OBJECT

    Q_PROPERTY(ROPassParamSetController* paramSetC READ paramSetC CONSTANT)
    Q_PROPERTY(ROPass* pass READ pass CONSTANT)

    Q_PROPERTY(ROSystemController* sysC READ sysC CONSTANT)

    // WARNINGS
    Q_PROPERTY(ROWarning* fillFlowData READ fillFlowData CONSTANT)
    Q_PROPERTY(ROWarning* checkRecovery READ checkRecovery CONSTANT)


    Q_PROPERTY(bool hasAnyCriticalWarnings READ hasAnyCriticalWarnings NOTIFY hasAnyCriticalWarningsChanged)
    Q_PROPERTY(bool hasAnyCautionWarnings READ hasAnyCautionWarnings NOTIFY hasAnyCautionWarningsChanged)


public:
    explicit ROPassController(ROPass* pass, ROSystemController* sysC);
    explicit ROPassController(); // for QML
    ~ROPassController();

    void copyDataFrom(const ROPassController* const from);

    Q_INVOKABLE ROStageController * stageC(int stageCIndex) const;
    Q_INVOKABLE ROStageController * stageC(ROStage * stage) const;

    ROPassParamSetController* const paramSetC() const;
    ROPass * const pass() const;

    ROSystemController* const sysC() const;


    // WARNINGS
    ROWarning* const fillFlowData() const; // CRITICAL
    ROWarning* const checkRecovery() const; // CRITICAL

    bool hasAnyCriticalWarnings() const;
    bool hasAnyCautionWarnings() const;

    QStringList allWarningsMessages(ROWarning::WarningType type = ROWarning::WarningAll) const;

signals:
    void hasAnyCriticalWarningsChanged();
    void hasAnyCautionWarningsChanged();
    void inputChanged();


public slots:
    void updateWarnings();
    void reset();
    void updateBlend();




private slots:
//    void updateFeed();
    void processRawWaterBeforeChanged();
    void processRawWaterAfterChanged();
    void updateFlowParams();
    void updateStages();
    void updateBlendPermeate();
    void updateRawWater();

private:
     ROPass* const _pass;
     ROPassParamSetController* const _paramSetC;
     QList<ROStageController*> _stageControllers;


     ROFlowMixer * _aboutBlend_R;  // см. updateBlend()
     ROFlowMixer * _toTotalProduct_RSTP;
     ROFlowMixer * _toBlending_ST;
     ROFlowMixer * _toFeed_ST;

     ROFlow * const _incomingRecycles_R;


     ROSystemController* const _sysC;

     bool _solved;
     bool _calculated;


     // WARNINGS
     ROWarning* _fillFlowData;
     ROWarning* _checkRecovery;
};

#endif // ROPASSCONTROLLER_H
