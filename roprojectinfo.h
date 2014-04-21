#ifndef ROPROJECTINFO_H
#define ROPROJECTINFO_H

#include <QObject>


// содержит информацию о проекте (имя, автора и др.).

class ROProjectInfo : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString company READ company WRITE setCompany NOTIFY companyChanged)
    Q_PROPERTY(QString notes READ notes WRITE setNotes NOTIFY notesChanged)


public:
    explicit ROProjectInfo(const QString& name = "", const QString& author = "", const QString& company = "", const QString& notes = "", QObject *parent = 0);
    const QString& name() const;
    const QString& author() const;
    const QString& company() const;
    const QString& notes() const;


    void setName(const QString& name);
    void setAuthor(const QString& author);
    void setCompany(const QString& company);
    void setNotes(const QString& notes);

private:
    QString _name;
    QString _author;
    QString _company;
    QString _notes;

signals:
    void nameChanged();
    void authorChanged();
    void companyChanged();
    void notesChanged();

    void inputChanged();
    
public slots:
    void reset();
    
};

#endif // ROPROJECTINFO_H
