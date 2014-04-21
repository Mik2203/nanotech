#include "roprojectinfo.h"

ROProjectInfo::ROProjectInfo(const QString& name, const QString& author, const QString& company, const QString& notes, QObject *parent) :
    QObject(parent),
    _name(name),
    _author(author),
    _company(company),
    _notes(notes) {
    connect(this, SIGNAL(nameChanged()), this, SIGNAL(inputChanged()));
    connect(this, SIGNAL(authorChanged()), this, SIGNAL(inputChanged()));
    connect(this, SIGNAL(companyChanged()), this, SIGNAL(inputChanged()));
    connect(this, SIGNAL(notesChanged()), this, SIGNAL(inputChanged()));
}

const QString& ROProjectInfo::name() const { return _name; }
const QString& ROProjectInfo::author() const { return _author; }
const QString& ROProjectInfo::company() const { return _company; }
const QString& ROProjectInfo::notes() const { return _notes; }

void ROProjectInfo::setName(const QString &name) { _name = name; Q_EMIT nameChanged(); }
void ROProjectInfo::setAuthor(const QString &author) { _author = author; Q_EMIT authorChanged(); }
void ROProjectInfo::setCompany(const QString &company) { _company = company; Q_EMIT companyChanged(); }
void ROProjectInfo::setNotes(const QString &notes) { _notes = notes; Q_EMIT notesChanged(); }




void ROProjectInfo::reset() {
    setName("");
    setAuthor("");
    setCompany("");
    setNotes("");
}
