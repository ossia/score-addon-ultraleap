#pragma once
#include <ossia/network/base/protocol.hpp>
#include <ossia/network/context_functions.hpp>

#include <UltraLeap/../3rdparty/ultraleap/src/DataTypes.hpp>
#include <UltraLeap/../3rdparty/ultraleap/src/Leap.hpp>
namespace ul
{
struct leap_manager;
}
namespace ossia
{

struct leap_device_parameters;
class OSSIA_EXPORT leapmotion_protocol final : public ossia::net::protocol_base
{
public:
  explicit leapmotion_protocol(ossia::net::network_context_ptr, std::string serial);
  ~leapmotion_protocol();

private:
  void set_device(ossia::net::device_base& dev) override;
  bool pull(net::parameter_base&) override;
  bool push(const net::parameter_base&, const ossia::value& v) override;
  bool push_raw(const net::full_parameter_data&) override;
  bool observe(net::parameter_base&, bool) override;
  bool update(net::node_base& node_base) override;

  void on_message(const ul::tracking_message& m, leap_device_parameters& params);

  ossia::net::network_context_ptr m_context;
  std::shared_ptr<ul::leap_manager> m_instance;
  std::string m_serial;
  ul::subscriber_handle m_handle;
  std::chrono::steady_clock::time_point prev_frame_time{};
  ul::Fingers prev_left_hand;
  ul::Fingers prev_right_hand;
};
}
