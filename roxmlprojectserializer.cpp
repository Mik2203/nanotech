#include "roxmlprojectserializer.h"

#include <QDebug>


ROXMLProjectSerializer::ROXMLProjectSerializer(QObject *parent) :
    ROAbstractProjectSerializer(parent)
{
}

void ROXMLProjectSerializer::beginWrite(QTextStream& stream) {
    _ostream.setDevice(stream.device());
#ifdef QT_DEBUG
    _ostream.setAutoFormatting(true);
#endif
    _ostream.writeStartDocument();
}

bool ROXMLProjectSerializer::endWrite() {
    _ostream.writeEndDocument();
    bool ok = !_ostream.hasError();
    _ostream.setDevice(0);
    return ok;
}

void ROXMLProjectSerializer::beginRead(QTextStream &stream) {
    _istream.setDevice(stream.device());
    readElement();
}

bool ROXMLProjectSerializer::readElement() {
    if (_curElementType == EndDocument) return false;
    _istream.readNext();
    while (_istream.isWhitespace()) _istream.readNext();

    // set current element type
    switch(_istream.tokenType()) {
    case QXmlStreamReader::NoToken: _curElementType = NoToken; break;
    case QXmlStreamReader::StartDocument: _curElementType = StartDocument; break;
    case QXmlStreamReader::EndDocument: _curElementType = EndDocument; break;
    case QXmlStreamReader::StartElement: _curElementType = StartElement; break;
    case QXmlStreamReader::EndElement: _curElementType = EndElement; break;
    case QXmlStreamReader::Characters: _curElementType = TextElement; break;
    default: _curElementType = Invalid; break;
    }

    _curText = _istream.isCharacters() ? _istream.text().toString() : _istream.name().toString();
    return true;
}

bool ROXMLProjectSerializer::endRead() {
    bool ok = !_istream.hasError();
    _istream.setDevice(0);
    return ok;
}

void ROXMLProjectSerializer::writeStartElement(const QString& name) {
    _ostream.writeStartElement(name);
}

void ROXMLProjectSerializer::writeEndElement() {
    _ostream.writeEndElement();
}

void ROXMLProjectSerializer::writeElement(const QString& name, const QString& data) {
    _ostream.writeTextElement(name, data);
}
