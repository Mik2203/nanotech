#ifndef ROPROJECT_H
#define ROPROJECT_H

#include <QObject>
#include "rosystem.h"
#include "rocase.h"
#include "roprojectinfo.h"

/* содержит все данные по проекту.
 * Внутри состоит из набора вариантов (ROCase),
 * а также включает в себя информацию о проекте (ROProjectInfo)
 */

class ROProject : public QObject {

    Q_OBJECT

    Q_PROPERTY(ROCase* firstCase READ firstCase NOTIFY firstCaseChanged)
    Q_PROPERTY(ROCase* lastCase READ lastCase NOTIFY lastCaseChanged)
    Q_PROPERTY(int caseCount READ caseCount NOTIFY caseCountChanged)

    Q_PROPERTY(ROProjectInfo* info READ info CONSTANT)

    Q_PROPERTY(int MAX_CASES_COUNT READ MAX_CASES_COUNT CONSTANT)
    Q_PROPERTY(int MIN_CASES_COUNT READ MIN_CASES_COUNT CONSTANT)
public:
    explicit ROProject(QObject *parent = 0);
    ~ROProject();


    // CASES
    static int MAX_CASES_COUNT() { return _MAX_CASES_COUNT; }
    static int MIN_CASES_COUNT() { return _MIN_CASES_COUNT; }
    int caseCount() const;
    Q_INVOKABLE ROCase* case_(int caseIndex) const;
    Q_INVOKABLE ROCase* addCase(int copyFromCaseIndex = -1);
    Q_INVOKABLE bool removeCase(int caseIndex = -1);
    Q_INVOKABLE int caseIndex(ROCase* case_) const;
    bool setCaseCount(int caseCount);
    ROCase* const firstCase() const;
    ROCase* const lastCase() const;
    
    // INFO
    ROProjectInfo* const info() const;

private:
    static const int _MAX_CASES_COUNT;
    static const int _MIN_CASES_COUNT;

    ROProjectInfo* _info;
    QList<ROCase*> _cases;

signals:
    void caseCountChanged();
    void firstCaseChanged();
    void lastCaseChanged();

    void inputChanged();
    
public slots:
    void reset();
    
};

#endif // ROPROJECT_H
