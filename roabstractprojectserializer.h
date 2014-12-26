#ifndef ROSYSTEMSERIALIZER_H
#define ROSYSTEMSERIALIZER_H

#include <QObject>
#include <QTextStream>

#include "roproject.h"

// класс для сериализации и десериализации проекта
class ROAbstractProjectSerializer : public QObject
{
    Q_OBJECT
public:
    explicit ROAbstractProjectSerializer(QObject *parent = nullptr);

    void deserialize(ROProject* const proj, QTextStream& ostream);
    bool serialize(const ROProject* const proj, QTextStream& istream);

protected:
    enum ElementType { NoToken, Invalid, StartDocument, EndDocument, StartElement, EndElement, TextElement };

    virtual void beginWrite(QTextStream& stream) =0;
    virtual void writeStartElement(const QString& name) =0;
    virtual void writeEndElement() =0;
    virtual void writeElement(const QString& name, const QString& data) =0;
    virtual bool endWrite() =0;

    virtual void beginRead(QTextStream& stream) =0;
    virtual bool readElement() =0;
    virtual bool endRead() =0;

    ElementType _curElementType;
    QString _curText;

private:
    bool endOfElement() const;
    bool readCheckNestedText(const QString& name);
    bool readNestedText();
    bool readRequiredElement(const QString& name);
    bool readElementStart();
    bool readElementEnd();

    
public slots:
    
};

#endif // ROSYSTEMSERIALIZER_H
