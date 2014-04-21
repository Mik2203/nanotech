#ifndef ROTRANSLATOR_H
#define ROTRANSLATOR_H

#include <QObject>
#include <QTranslator>

/* �����, ���������� �� ��������� �������� ����� � �������� ��������.
 * emptyString ������������ � QML ��� ���������� ��������
 * (��� ������������� � ��� ������������ ^_^)
 */

class ROTranslator : public QObject {

    Q_OBJECT
    Q_ENUMS(Language)
    Q_PROPERTY(Language currentLanguage READ currentLanguage WRITE setCurrentLanguage NOTIFY currentLanguageChanged)
    Q_PROPERTY(QString emptyString READ emptyString NOTIFY currentLanguageChanged)

public:
    enum Language {EN, RU};

    explicit ROTranslator(QObject *parent = 0);
    ~ROTranslator();

    Language currentLanguage() const;
    void setCurrentLanguage(Language);
    QString emptyString() { return ""; }

signals:
    void currentLanguageChanged();
    
public slots:

private:
    Language _currentLanguage;
    QTranslator* _translator;
    
};

#endif // ROTRANSLATOR_H
