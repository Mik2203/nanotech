#include "roproject.h"

const int ROProject::_MAX_CASES_COUNT = 4;
const int ROProject::_MIN_CASES_COUNT = 1;

ROProject::ROProject(QObject *parent) :
    QObject(parent), _info(new ROProjectInfo())
{
    ROCase* firstCase = new ROCase();
    _cases.append(firstCase);

    connect(this, SIGNAL(caseCountChanged()), this, SIGNAL(inputChanged()));
    connect(firstCase, SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
    connect(_info, SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
}


ROProject::~ROProject() {
    delete _info;
    qDeleteAll(_cases); _cases.clear();
}

ROProjectInfo* const ROProject::info() const { return _info; }

int ROProject::caseCount() const { return _cases.count(); }

ROCase* ROProject::case_(int caseIndex) const { return _cases.value(caseIndex); }

ROCase *ROProject::addCase(int copyFromCaseIndex) {
    if (_cases.count() < _MAX_CASES_COUNT) {
        ROCase* newCase = new ROCase();
        if (0 <= copyFromCaseIndex && copyFromCaseIndex < _cases.count()) {
            newCase->copyDataFrom(case_(copyFromCaseIndex));
        }
        connect(newCase, SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
        _cases.append(newCase);
        Q_EMIT caseCountChanged();
        Q_EMIT lastCaseChanged();
        return newCase;
    }
    return nullptr;
}

int ROProject::caseIndex(ROCase* sysCase) const { return _cases.indexOf(sysCase); }

bool ROProject::setCaseCount(int caseCount) {
    if (_MIN_CASES_COUNT <= caseCount && caseCount <= _MAX_CASES_COUNT) {
        while (caseCount > _cases.count()) addCase();
        while (caseCount < _cases.count()) removeCase();
        return true;
    }
    return false;
}

ROCase *const ROProject::firstCase() const { return _cases.first(); }
ROCase *const ROProject::lastCase() const { return _cases.last(); }

bool ROProject::removeCase(int caseIndex) {
    if (caseIndex == -1) caseIndex = _cases.count()-1; // last
    if (_cases.count() > _MIN_CASES_COUNT && (0 <= caseIndex && caseIndex < _cases.count())) {
        ROCase* removingCase = _cases.takeAt(caseIndex);
        disconnect(removingCase, SIGNAL(inputChanged()), this, SIGNAL(inputChanged()));
        delete removingCase;
        Q_EMIT caseCountChanged();
        if (caseIndex == 0) Q_EMIT firstCaseChanged();
        if (caseIndex == _cases.count()) Q_EMIT lastCaseChanged();
        return true;
    }
    return false;
}


void ROProject::reset() {
    info()->reset();
    setCaseCount(1);
    firstCase()->reset();
}
