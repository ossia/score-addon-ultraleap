#include "score_addon_ultraleap.hpp"

#include <score/plugins/FactorySetup.hpp>
#include <score/widgets/MessageBox.hpp>

#include <QTimer>

#include <UltraLeap/LeapmotionProtocolFactory.hpp>
#include <UltraLeap/Protocol.hpp>

score_addon_ultraleap::score_addon_ultraleap()
{
  try
  {
    m_hasleapmotion = ul::libleapc::instance().available;
  }
  catch(...)
  {
  }

  if(!m_hasleapmotion)
  {
    qDebug() << "UltraLeap not found. Make sure LeapC.dll / dylib is in the right "
                "path.";
    return;
  }
  //qRegisterMetaType<leapmotion::OutputSettings>();
  static auto inst = ul::instance<ul::leap_manager>();
  qDebug() << "UltraLeap library loaded.";
}

score_addon_ultraleap::~score_addon_ultraleap() { }

std::vector<score::InterfaceBase*> score_addon_ultraleap::factories(
    const score::ApplicationContext& ctx, const score::InterfaceKey& key) const
{
  if(!m_hasleapmotion)
    return {};

  return instantiate_factories<
      score::ApplicationContext,
      FW<Device::ProtocolFactory, Protocols::LeapmotionProtocolFactory>>(ctx, key);
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_ultraleap)
