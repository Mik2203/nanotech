#include "roprojectmanager.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "roxmlprojectserializer.h"

ROProjectManager::ROProjectManager(QObject *parent) :
    QObject(parent),
    _proj(new ROProject()),
    _projWasAltered(false) {
    connect(_proj, SIGNAL(inputChanged()), this, SLOT(setProjWasAltered()));
}

ROProjectManager::~ROProjectManager() {
    _projFile.close();
    delete _proj;
}

ROProject* const ROProjectManager::proj() const { return _proj; }

QString ROProjectManager::projFileName() const { return _projFile.fileName(); }
bool ROProjectManager::projWasAltered() const { return _projWasAltered; }

void ROProjectManager::newProject() {
    if (maybeSave()) {
        _projFile.close();
        _projFile.setFileName("");
        Q_EMIT projFileNameChanged();

        disconnect(_proj, SIGNAL(inputChanged()), this, SLOT(setProjWasAltered()));

//        delete _proj;
//        _proj = new ROProject();
        _proj->reset();

        connect(_proj, SIGNAL(inputChanged()), this, SLOT(setProjWasAltered()));

        Q_EMIT projChanged();
        if (_projWasAltered) {
            _projWasAltered = false;
            Q_EMIT projWasAlteredChanged();
        }
    }
}

bool ROProjectManager::saveProject() {
    if (_projFile.isOpen()) {
        return saveProjectToFile();
    } else {
        return saveProjectAs();
    }
}

bool ROProjectManager::saveProjectAs() {
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save Project"), "projects/",
                                                    tr("Nanotech Project Files (*.ntp)"));
    if (!fileName.isEmpty()) {
        _projFile.close();
        _projFile.setFileName(fileName);
        _projFile.open(QFile::ReadWrite);
        if (_projFile.isOpen() && saveProjectToFile()) {
            Q_EMIT projFileNameChanged();
            return true;
        }
    }

    return false;


}

void ROProjectManager::openProject() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(0, tr("Open Project"), "",
                                                        tr("Nanotech Project Files (*.ntp)"));
        if (!fileName.isEmpty() /*&& fileName != _projFile.fileName()*/) {
            _projFile.close();
            _projFile.setFileName(fileName);
            _projFile.open(QFile::ReadWrite);
            if (_projFile.isOpen()) {
                loadProjectFromFile();
                Q_EMIT projChanged();
                Q_EMIT projFileNameChanged();
            }
        }

    }
}

bool ROProjectManager::saveProjectToFile() {
    if (!_projFile.isOpen() || !_projFile.isWritable()) {
        QMessageBox::warning(0, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(_projFile.fileName())
                             .arg(_projFile.errorString()));
        return false;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    // Clear file
    _projFile.reset();
    _projFile.resize(_projFile.pos());

    // Write data
    ROXMLProjectSerializer ps;
    ps.serialize(_proj, QTextStream(&_projFile));

    // Update file
    _projFile.flush();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (_projWasAltered) {
        _projWasAltered = false;
        Q_EMIT projWasAlteredChanged();
    }
    return true;

}


void ROProjectManager::setProjWasAltered() {
    if (!_projWasAltered) {
        _projWasAltered = true;
        Q_EMIT projWasAlteredChanged();
    }
}

bool ROProjectManager::maybeSave() {
    if (_projWasAltered) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(0, tr("Application"),
                                   tr("The project has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveProject();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void ROProjectManager::loadProjectFromFile() {
    if (!_projFile.isOpen()) {
        QMessageBox::warning(0, tr("Application"),
                             tr("Cannot read project file %1:\n%2.")
                             .arg(_projFile.fileName())
                             .arg(_projFile.errorString()));
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    ROXMLProjectSerializer ps;
    _proj->reset();
    ps.deserialize(_proj, QTextStream(&_projFile));

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (_projWasAltered) {
        _projWasAltered = false;
        Q_EMIT projWasAlteredChanged();
    }
}
