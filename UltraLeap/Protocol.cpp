#include "Protocol.hpp"

#include <ossia/detail/config.hpp>

#include <ossia/detail/logger.hpp>
#include <ossia/network/base/protocol.hpp>
#include <ossia/network/common/complex_type.hpp>
#include <ossia/network/context.hpp>
#include <ossia/network/oscquery/oscquery_server.hpp>

#include <LeapC.h>
namespace ossia
{
leapmotion_protocol::leapmotion_protocol(
    ossia::net::network_context_ptr ptr, std::string serial)
    : protocol_base{flags{SupportsMultiplex}}
    , m_context{std::move(ptr)}
    , m_instance{ul::instance<ul::leap_manager>()}
    , m_serial{std::move(serial)}
{
}

struct hand
{
  ossia::net::node_base& root;
  std::string kind;
  ossia::net::parameter_base& active{
      *ossia::create_parameter(root, "/" + kind + "/active", "bool")};

  ossia::net::parameter_base& thumb_distal_begin{
      *ossia::create_parameter(root, "/" + kind + "/thumb/distal/begin", "xyz")};
  ossia::net::parameter_base& index_distal_begin{
      *ossia::create_parameter(root, "/" + kind + "/index/distal/begin", "xyz")};
  ossia::net::parameter_base& middle_distal_begin{
      *ossia::create_parameter(root, "/" + kind + "/middle/distal/begin", "xyz")};
  ossia::net::parameter_base& ring_distal_begin{
      *ossia::create_parameter(root, "/" + kind + "/ring/distal/begin", "xyz")};
  ossia::net::parameter_base& pinky_distal_begin{
      *ossia::create_parameter(root, "/" + kind + "/pinky/distal/begin", "xyz")};

  ossia::net::parameter_base& thumb_distal_end{
      *ossia::create_parameter(root, "/" + kind + "/thumb/distal/end", "xyz")};
  ossia::net::parameter_base& index_distal_end{
      *ossia::create_parameter(root, "/" + kind + "/index/distal/end", "xyz")};
  ossia::net::parameter_base& middle_distal_end{
      *ossia::create_parameter(root, "/" + kind + "/middle/distal/end", "xyz")};
  ossia::net::parameter_base& ring_distal_end{
      *ossia::create_parameter(root, "/" + kind + "/ring/distal/end", "xyz")};
  ossia::net::parameter_base& pinky_distal_end{
      *ossia::create_parameter(root, "/" + kind + "/pinky/distal/end", "xyz")};

  ossia::net::parameter_base& thumb_orientation{*ossia::create_parameter(
      root, "/" + kind + "/thumb/orientation", "orientation.quaternion")};
  ossia::net::parameter_base& index_orientation{*ossia::create_parameter(
      root, "/" + kind + "/index/orientation", "orientation.quaternion")};
  ossia::net::parameter_base& middle_orientation{*ossia::create_parameter(
      root, "/" + kind + "/middle/orientation", "orientation.quaternion")};
  ossia::net::parameter_base& ring_orientation{*ossia::create_parameter(
      root, "/" + kind + "/ring/orientation", "orientation.quaternion")};
  ossia::net::parameter_base& pinky_orientation{*ossia::create_parameter(
      root, "/" + kind + "/pinky/orientation", "orientation.quaternion")};

  ossia::net::parameter_base& thumb_width{
      *ossia::create_parameter(root, "/" + kind + "/thumb/width", "float")};
  ossia::net::parameter_base& index_width{
      *ossia::create_parameter(root, "/" + kind + "/index/width", "float")};
  ossia::net::parameter_base& middle_width{
      *ossia::create_parameter(root, "/" + kind + "/middle/width", "float")};
  ossia::net::parameter_base& ring_width{
      *ossia::create_parameter(root, "/" + kind + "/ring/width", "float")};
  ossia::net::parameter_base& pinky_width{
      *ossia::create_parameter(root, "/" + kind + "/pinky/width", "float")};

  ossia::net::parameter_base& thumb_extended{
      *ossia::create_parameter(root, "/" + kind + "/thumb/extended", "float")};
  ossia::net::parameter_base& index_extended{
      *ossia::create_parameter(root, "/" + kind + "/index/extended", "float")};
  ossia::net::parameter_base& middle_extended{
      *ossia::create_parameter(root, "/" + kind + "/middle/extended", "float")};
  ossia::net::parameter_base& ring_extended{
      *ossia::create_parameter(root, "/" + kind + "/ring/extended", "float")};
  ossia::net::parameter_base& pinky_extended{
      *ossia::create_parameter(root, "/" + kind + "/pinky/extended", "float")};

  ossia::net::parameter_base& palm_velocity{
      *ossia::create_parameter(root, "/" + kind + "/palm/velocity", "xyz")};
  ossia::net::parameter_base& palm_position{
      *ossia::create_parameter(root, "/" + kind + "/palm/position", "xyz")};
  ossia::net::parameter_base& palm_normal{
      *ossia::create_parameter(root, "/" + kind + "/palm/normal", "xyz")};
  ossia::net::parameter_base& palm_direction{
      *ossia::create_parameter(root, "/" + kind + "/palm/direction", "xyz")};
  ossia::net::parameter_base& palm_orientation{*ossia::create_parameter(
      root, "/" + kind + "/palm/orientation", "orientation.quaternion")};

  ossia::net::parameter_base& pinch{
      *ossia::create_parameter(root, "/" + kind + "/pinch", "float")};
  ossia::net::parameter_base& grab{
      *ossia::create_parameter(root, "/" + kind + "/grab", "float")};

  void init()
  {
    ossia::vecf_domain<3> box_domain;
    box_domain.min[0] = -200;
    box_domain.min[1] = 0;
    box_domain.min[2] = -200;
    box_domain.max[0] = 200;
    box_domain.max[1] = 500;
    box_domain.max[2] = 200;

    thumb_distal_begin.set_domain(box_domain);
    thumb_distal_end.set_domain(box_domain);
    index_distal_begin.set_domain(box_domain);
    index_distal_end.set_domain(box_domain);
    middle_distal_begin.set_domain(box_domain);
    middle_distal_end.set_domain(box_domain);
    ring_distal_begin.set_domain(box_domain);
    ring_distal_end.set_domain(box_domain);
    pinky_distal_begin.set_domain(box_domain);
    pinky_distal_end.set_domain(box_domain);

    ossia::domain domain_speed = ossia::make_domain(-1000., 1000.f);
    ossia::domain domain_01 = ossia::make_domain(0.f, 1.f);
    ossia::domain domain_minus1_1 = ossia::make_domain(-1.f, 1.f);
    palm_velocity.set_domain(domain_speed);
    palm_position.set_domain(box_domain);
    palm_normal.set_domain(domain_minus1_1);
    palm_direction.set_domain(domain_minus1_1);

    grab.set_domain(domain_01);
    pinch.set_domain(domain_01);

    ossia::domain_base<float> angle_domain;
    angle_domain.min = 0;
    angle_domain.max = 360;

    palm_orientation.set_domain(angle_domain);
  }
};

struct leap_device_parameters
{
  ossia::net::device_base& dev;
  hand left{dev.get_root_node(), "left"};
  hand right{dev.get_root_node(), "right"};

  ossia::net::parameter_base& num_hands{
      *ossia::create_parameter(dev.get_root_node(), "/num_hands", "int")};
};

void leapmotion_protocol::set_device(net::device_base& dev)
{
  using namespace ul;
  subscriber_options opt;
  opt.on_tracking_event
      = [this, params = leap_device_parameters{dev}](const tracking_message& m) mutable {
    this->on_message(m, params);
  };
  if(!m_serial.empty())
  {
    opt.tracked_device_serial = m_serial;
  }
  else
  {
    opt.tracked_device = 1;
  }
  m_handle = m_instance->subscribe(std::move(opt));
}

leapmotion_protocol::~leapmotion_protocol()
{
  if(m_instance && m_handle)
    m_instance->unsubscribe(m_handle);
}

static double distance_conversion_factor(std::string_view unit)
{
  if(unit == "m")
  {
    return 0.001;
  }
  else if(unit == "mm")
  {
    return 1.;
  }
  else
  {
    return 1.;
  }
}

void leapmotion_protocol::on_message(
    const ul::tracking_message& msg, leap_device_parameters& params)
{
  using namespace ul;
  const auto frame_time = std::chrono::steady_clock::now();
  const auto frame_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(
                              frame_time - prev_frame_time)
                              .count();
  const auto inv_frame_rate
      = (frame_diff > 1e9 || frame_diff <= 1e3) ? 0. : 1e9 / frame_diff;
  const auto factor = distance_conversion_factor("mm");

  const int Nhand = msg.hands.size();
  bool leftHandTracked = false;
  bool rightHandTracked = false;

  for(int i = 0; i < Nhand; i++)
  {
    const auto& ih = msg.hands[i];
    if(ih.type == eLeapHandType_Left)
      leftHandTracked = true;
    else
      rightHandTracked = true;
  }

  params.left.active.push_value(leftHandTracked);
  params.right.active.push_value(rightHandTracked);
  if(!leftHandTracked)
    prev_left_hand.hand_id = -1;
  if(!rightHandTracked)
    prev_right_hand.hand_id = -1;

  params.num_hands.push_value(Nhand);

  auto update_speed = [=](Fingers& prev, int hand, int finger, FingerInfo& of) {
    if(prev.hand_id == hand && inv_frame_rate > 0.)
    {
      const auto& pf = prev.fingers[finger];

      of.vx = (of.px - pf.px) * inv_frame_rate;
      of.vy = (of.py - pf.py) * inv_frame_rate;
      of.vz = (of.pz - pf.pz) * inv_frame_rate;
    }
    prev.fingers[finger] = of;
  };

  for(int i = 0; i < Nhand; i++)
  {
    const auto& ih = msg.hands[i];
    hand& h
        = (ih.type == eLeapHandType::eLeapHandType_Left) ? params.left : params.right;

    float px = ih.palm.position.x * factor;
    float py = ih.palm.position.y * factor;
    float pz = ih.palm.position.z * factor;

    float vx = ih.palm.velocity.x * factor;
    float vy = ih.palm.velocity.y * factor;
    float vz = ih.palm.velocity.z * factor;

    float nx = ih.palm.normal.x;
    float ny = ih.palm.normal.y;
    float nz = ih.palm.normal.z;

    float dx = ih.palm.direction.x;
    float dy = ih.palm.direction.y;
    float dz = ih.palm.direction.z;

    float o1 = ih.palm.orientation.x;
    float o2 = ih.palm.orientation.y;
    float o3 = ih.palm.orientation.z;
    float o4 = ih.palm.orientation.w;

    //oh.radius = ih.palm.width;
    auto pinch = ih.pinch_strength;
    auto grab = ih.grab_strength;

    auto time = ih.visible_time * 0.000001;

    {
      h.palm_position.push_value(ossia::vec3f{px, py, pz});
      h.palm_velocity.push_value(ossia::vec3f{vx, vy, vz});
      h.palm_direction.push_value(ossia::vec3f{dx, dy, dz});
      h.palm_normal.push_value(ossia::vec3f{nx, ny, nz});
      h.palm_orientation.push_value(ossia::vec4f{o1, o2, o3, o4});
      h.pinch.push_value(pinch);
      h.grab.push_value(grab);
    }

    ossia::net::parameter_base* fingers_begin[5]{
        &h.thumb_distal_begin, &h.index_distal_begin, &h.middle_distal_begin,
        &h.ring_distal_begin, &h.pinky_distal_begin};
    ossia::net::parameter_base* fingers_end[5]{
        &h.thumb_distal_end, &h.index_distal_end, &h.middle_distal_end,
        &h.ring_distal_end, &h.pinky_distal_end};
    ossia::net::parameter_base* fingers_orient[5]{
        &h.thumb_orientation, &h.index_orientation, &h.middle_orientation,
        &h.ring_orientation, &h.pinky_orientation};
    ossia::net::parameter_base* fingers_width[5]{
        &h.thumb_width, &h.index_width, &h.middle_width, &h.ring_width, &h.pinky_width};
    ossia::net::parameter_base* fingers_ext[5]{
        &h.thumb_extended, &h.index_extended, &h.middle_extended, &h.ring_extended,
        &h.pinky_extended};

    int finger_index = 0;
    for(int idx = 0; idx < 5; idx++)
    {
      auto& finger = ih.digits[idx];
      auto pbegin = fingers_begin[idx];
      auto pend = fingers_end[idx];
      auto orient = fingers_orient[idx];
      auto width = fingers_width[idx];
      auto ext = fingers_ext[idx];

      float pbx = finger.distal.prev_joint.x * factor;
      float pby = finger.distal.prev_joint.y * factor;
      float pbz = finger.distal.prev_joint.z * factor;
      float pex = finger.distal.next_joint.x * factor;
      float pey = finger.distal.next_joint.y * factor;
      float pez = finger.distal.next_joint.z * factor;

      auto o1 = finger.distal.rotation.x;
      auto o2 = finger.distal.rotation.y;
      auto o3 = finger.distal.rotation.z;
      auto o4 = finger.distal.rotation.w;

      pbegin->push_value(ossia::vec3f{pbx, pby, pbz});
      pend->push_value(ossia::vec3f{pex, pey, pez});
      orient->push_value(ossia::vec4f{o1, o2, o3, o4});
      width->push_value(finger.distal.width);
      ext->push_value(finger.is_extended ? 1 : 0);

      //of.dx = finger.distal.next_joint.x - finger.distal.prev_joint.x;
      //of.dy = finger.distal.next_joint.y - finger.distal.prev_joint.y;
      //of.dz = finger.distal.next_joint.z - finger.distal.prev_joint.z;

      // Updated later
      // of.vx = 0.; // ih.palm.velocity.x;
      // of.vy = 0.; // ih.palm.velocity.y;
      // of.vz = 0.; // ih.palm.velocity.z;

      //of.width = 0.;
      // of.length = 0.;
      /*
      BoneInfo ob;
      int boneID = 0;

      for(auto& bone : finger.bones)
      {
        auto len = std::hypot(
            bone.next_joint.x - bone.prev_joint.x, bone.next_joint.y - bone.prev_joint.y,
            bone.next_joint.z - bone.prev_joint.z);
        of.length += len * factor;

        ob.fid = of.id;

        ob.bid = boneID;
        boneID += 1;

        ob.ppx = bone.prev_joint.x * factor;
        ob.ppy = bone.prev_joint.y * factor;
        ob.ppz = bone.prev_joint.z * factor;

        ob.o1 = bone.rotation.x;
        ob.o2 = bone.rotation.y;
        ob.o3 = bone.rotation.z;
        ob.o4 = bone.rotation.w;

        ob.pnx = bone.next_joint.x * factor;
        ob.pny = bone.next_joint.y * factor;
        ob.pnz = bone.next_joint.z * factor;

        ob.w = bone.width * factor;
        ob.l = len * factor;

        /// * if(ih.type == eLeapHandType::eLeapHandType_Left)
        /// *   outputs.bone_l(ob);
        /// * else
        /// *   outputs.bone_r(ob);
      }
*/
      /*
      if(ih.type == eLeapHandType::eLeapHandType_Left)
      {
        update_speed(prev_left_hand, ih.id, finger_index, of);
        /// * outputs.finger_l(of);
      }
      else
      {
        update_speed(prev_right_hand, ih.id, finger_index, of);
        /// * outputs.finger_r(of);
      }
      */
      ++finger_index;
    }

#if 0
    // For speed computation on the next turn
    if(ih.type == eLeapHandType::eLeapHandType_Left)
      prev_left_hand.hand_id = ih.id;
    else
      prev_right_hand.hand_id = ih.id;
#endif
  }

  prev_frame_time = frame_time;
}

bool leapmotion_protocol::pull(ossia::net::parameter_base&)
{
  return false;
}

bool leapmotion_protocol::push(const ossia::net::parameter_base&, const ossia::value& v)
{
  return false;
}

bool leapmotion_protocol::push_raw(const ossia::net::full_parameter_data&)
{
  return false;
}

bool leapmotion_protocol::observe(ossia::net::parameter_base&, bool)
{
  return false;
}

bool leapmotion_protocol::update(ossia::net::node_base& node_base)
{
  return false;
}
}
