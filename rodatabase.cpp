#include "rodatabase.h"
#include <QDebug>

#include <QFile>
#include <QFileInfo>
#include <QSqlError>

RODatabase::RODatabase(): QObject(0), _errorText("") {}
RODatabase::~RODatabase() {
    delete _waterTypeModel;
    delete _membranesModel;
    delete _membraneAlternativesModel;
    delete _genericSchemesModel;
}

ROWaterTypeModel *const RODatabase::waterTypes() { return _waterTypeModel; }
ROMembranesModel *const RODatabase::membranes() { return _membranesModel; }
ROMembranesAlternativesModel* const RODatabase::membraneAlternatives() { return _membraneAlternativesModel; }
ROGenericSchemesModel *const RODatabase::genericSchemes() { return _genericSchemesModel; }
const QString &RODatabase::errorText() const { return _errorText; }


bool RODatabase::setDatabase(const QString& dbname) { return instance()->setInstanceDatabase(dbname); }
bool RODatabase::setInstanceDatabase(const QString &dbname) {
    QFile dbFile(dbname);
    if (!dbFile.exists()) {
        _errorText = tr("Database error!\nDatabase file does not exists.\n File path: %1\n")
                .arg(QFileInfo(dbFile).absoluteFilePath());
        return false;
    }

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbname);


    // try open
    if (!_db.open()) {
        _errorText = tr("Database error!\nCannot open database.\n Database error: %1\n Driver error: %2\n")
                       .arg(_db.lastError().databaseText(),
                            _db.lastError().driverText());
        return false;
    }

    _membranesModel = new ROMembranesModel(this);
//    _membranesModel->populate();

    _waterTypeModel = new ROWaterTypeModel(_db, this);
    _waterTypeModel->populate();

    _membraneAlternativesModel = new ROMembranesAlternativesModel(this);
    _genericSchemesModel = new ROGenericSchemesModel(this);

    return true;
}
