#ifndef ROXMLPROJECTSERIALIZER_H
#define ROXMLPROJECTSERIALIZER_H

#include "roabstractprojectserializer.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class ROXMLProjectSerializer : public ROAbstractProjectSerializer {
    Q_OBJECT
public:
    explicit ROXMLProjectSerializer(QObject *parent = 0);
    
private:
    QXmlStreamReader _istream;
    QXmlStreamWriter _ostream;

    void beginWrite(QTextStream& stream);
    void writeStartElement(const QString& name);
    void writeEndElement();
    void writeElement(const QString& name, const QString& data);
    bool endWrite();

    void beginRead(QTextStream& stream);
    bool readElement();
    bool endRead();

signals:
    
public slots:
    
};

#endif // ROXMLPROJECTSERIALIZER_H
