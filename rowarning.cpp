#include "rowarning.h"

ROWarning::ROWarning(std::function<bool()> updateFunc, WarningType type, std::function<QString()> updateWhatFunc, QObject *parent):
    _updateFunc(updateFunc), _type(type), _what(""), _updateWhatFunc(updateWhatFunc), _enabled(false), QObject(parent) {
    _enabled = _updateFunc();
    _what = _updateWhatFunc();
}

ROWarning::ROWarning(QObject *parent):
    _type(ROWarning::WarningCritical), _what(""), _enabled(false), QObject(parent) {}

bool ROWarning::enabled() const { return _enabled; }
QString ROWarning::what() const { return _what; }
ROWarning::WarningType ROWarning::type() const { return _type; }

void ROWarning::update() {
    _enabled = _updateFunc();
    if (_enabled) {
        _what = _updateWhatFunc();
        emit whatChanged();
    }
    emit enabledChanged();
}
