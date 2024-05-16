#include "LeapmotionSpecificSettings.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

template <>
void DataStreamReader::read(const Protocols::LeapmotionSpecificSettings& n)
{
  m_stream << n.serial;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Protocols::LeapmotionSpecificSettings& n)
{
  m_stream >> n.serial;
  checkDelimiter();
}

template <>
void JSONReader::read(const Protocols::LeapmotionSpecificSettings& n)
{
  obj["Serial"] = n.serial;
}

template <>
void JSONWriter::write(Protocols::LeapmotionSpecificSettings& n)
{
  n.serial = obj["Serial"].toString();
}
