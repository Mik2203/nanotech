#ifndef ROPASSPARAMSETCONTROLLER_H
#define ROPASSPARAMSETCONTROLLER_H

#include <QObject>


/* �������� �� ������������ ������� ������� �����/���/����� ��������� �������,
 * ����� ���� ������� �� ����� ���� ������������.
 */

class ROPassParamSetController : public QObject
{
    Q_OBJECT

    Q_ENUMS(ParamSetState)

    //SET STATES
    Q_PROPERTY(ParamSetState feedSetState     READ feedSetState     WRITE setFeedSetState     NOTIFY feedSetStateChanged)
    Q_PROPERTY(ParamSetState recoverySetState READ recoverySetState WRITE setRecoverySetState NOTIFY recoverySetStateChanged)
    Q_PROPERTY(ParamSetState permeateSetState READ permeateSetState WRITE setPermeateSetState NOTIFY permeateSetStateChanged)

public:
    enum ParamSetState { ParamSetUndefined, ParamSetExplicit, ParamSetImplicit, ParamSetAuto };

    explicit ROPassParamSetController(QObject *parent = 0);
    void copyDataFrom(const ROPassParamSetController* const from);

    //Parameters set states
    ParamSetState feedSetState() const;
    ParamSetState recoverySetState() const;
    ParamSetState permeateSetState() const;

    void setFeedSetState(ParamSetState state);
    void setRecoverySetState(ParamSetState state);
    void setPermeateSetState(ParamSetState state);

    void setNextController(ROPassParamSetController* const next);
    void setPrevController(ROPassParamSetController* const prev);

    bool allSet() const;

signals:

    void feedSetStateChanged();
    void recoverySetStateChanged();
    void permeateSetStateChanged();
    void anySetStateChanged();
    
public slots:
    void reset();

private:
    // ��������� ��� feedflow, recovery � permeateflow - ��� ����������� �����������,
    // ��������� ������ 2 �� 3 ����� ���� ����������� ������������.
    ParamSetState _feedSetState;
    ParamSetState _recoverySetState;
    ParamSetState _permeateSetState;
    bool _statesChanging;

    ROPassParamSetController* _prev;
    ROPassParamSetController* _next;
    
};

#endif // ROPASSPARAMSETCONTROLLER_H
