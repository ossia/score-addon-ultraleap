#pragma once

#include <verdigris>

namespace Protocols
{

struct LeapmotionSpecificSettings
{
  QString serial;
};

}

Q_DECLARE_METATYPE(Protocols::LeapmotionSpecificSettings)
W_REGISTER_ARGTYPE(Protocols::LeapmotionSpecificSettings)
