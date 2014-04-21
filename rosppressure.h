#ifndef ROSPPRESSURE_H
#define ROSPPRESSURE_H
#include "rodoublesystemparameter.h"

class ROSPPressure : public RODoubleSystemParameter {
    Q_OBJECT
public:
    ROSPPressure();
    ROSPPressure(const QString& name, const double& value = 0.0,
                 const QString& description = QString());
    const double value(const QString& units = QString()) const;
    const QString name() const;
    const QString description() const;
    const QList<QString> units() const;

    inline void setName(const QString& name) { _name = name; }
    inline void setValue(const double& value, const QString& units = QString()) { _value = value; }
    inline void setDescription(const QString& description) { _description = description; }
    inline void setUnits(const QStringList& units) { _units = units; }

private:
    QString _name;
    double _value;
    QString _description;
    QStringList _units;
public:

};

#endif // ROSPPRESSURE_H
