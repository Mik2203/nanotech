#ifndef ROMEMBRANESIZE_H
#define ROMEMBRANESIZE_H
#include <QString>

class ROMembraneSize {
public:
    ROMembraneSize(const double& diameter, const double& length);
    ROMembraneSize(const ROMembraneSize&);
    ROMembraneSize& operator= (const ROMembraneSize&);
    const double diameter() const;
    const double length() const;
    const QString toString() const;
private:
    double _diameter;
    double _length;
};

#endif // ROMEMBRANESIZE_H
