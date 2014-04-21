#include "rotranslator.h"


#include <QDebug>
#include "roapplication.h"

ROTranslator::ROTranslator(QObject *parent) :
    QObject(parent),
    _translator(new QTranslator()),
    _currentLanguage(EN){
    ROApplication::installTranslator(_translator);
}

ROTranslator::~ROTranslator() { delete _translator; }

ROTranslator::Language ROTranslator::currentLanguage() const { return _currentLanguage; }

void ROTranslator::setCurrentLanguage(ROTranslator::Language lang) {
    if (_currentLanguage != lang) {
        switch(lang) {
        case RU: _translator->load(":/translations/nano_ru.qm"); break;
        case EN: _translator->load("nano_en.qm");
        }
        _currentLanguage = lang;
    }
    Q_EMIT currentLanguageChanged();
}
