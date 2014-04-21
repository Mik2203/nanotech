#ifndef ROPROJECTMANAGER_H
#define ROPROJECTMANAGER_H

#include <QObject>
#include <QFile>

#include "roproject.h"

/* класс, обрабатывающий манипуляции с файлом проекта
 * (сохранение, загрузка, создание нового и др.).
 * Содержит указатель на сам проект (ROProject).
 */

class ROProjectManager : public QObject {

    Q_OBJECT

    Q_PROPERTY(ROProject* proj READ proj NOTIFY projChanged)
    Q_PROPERTY(QString projFileName READ projFileName NOTIFY projFileNameChanged)
    Q_PROPERTY(bool projWasAltered READ projWasAltered NOTIFY projWasAlteredChanged)
public:
    explicit ROProjectManager(QObject *parent = 0);
    ~ROProjectManager();

    ROProject* const proj() const;
    QString projFileName() const;
    bool projWasAltered() const;
    
signals:
    void projChanged();
    void projFileNameChanged();
    void projWasAlteredChanged();
    
public slots:
    bool maybeSave();
    void newProject();
    bool saveProject();
    bool saveProjectAs();
    void openProject();

private:
    bool _projWasAltered;
    ROProject* _proj;

    // File
    QFile _projFile;
    bool saveProjectToFile();
    void loadProjectFromFile();

private slots:
    void setProjWasAltered();
    
};

#endif // ROPROJECTMANAGER_H
