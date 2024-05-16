#include "LeapmotionDevice.hpp"

#include "LeapmotionSpecificSettings.hpp"
#include "Protocol.hpp"

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

#include <score/document/DocumentContext.hpp>

#include <ossia/network/generic/generic_device.hpp>

#include <ossia-qt/invoke.hpp>

#include <QLabel>
#include <QProgressDialog>

#include <wobjectimpl.h>

W_OBJECT_IMPL(Protocols::LeapmotionDevice)

namespace Protocols
{

LeapmotionDevice::LeapmotionDevice(
    const Device::DeviceSettings& settings, const ossia::net::network_context_ptr& ctx)
    : OwningDeviceInterface{settings}
    , m_ctx{ctx}
{
  m_capas.canRefreshTree = true;
  m_capas.canAddNode = false;
  m_capas.canRemoveNode = false;
  m_capas.canRenameNode = false;
  m_capas.canSetProperties = false;
  m_capas.canSerialize = false;
}

LeapmotionDevice::~LeapmotionDevice() { }

bool LeapmotionDevice::reconnect()
{
  disconnect();
  try
  {
    const auto& stgs
        = settings().deviceSpecificSettings.value<LeapmotionSpecificSettings>();
    auto addr = std::make_unique<ossia::net::generic_device>(
        std::make_unique<ossia::leapmotion_protocol>(m_ctx, stgs.serial.toStdString()),
        settings().name.toStdString());

    m_dev = std::move(addr);
    deviceChanged(nullptr, m_dev.get());
  }
  catch(...)
  {
    SCORE_TODO;
  }

  return connected();
}

void LeapmotionDevice::disconnect()
{
  OwningDeviceInterface::disconnect();
}

}
