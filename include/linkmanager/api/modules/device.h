/*
 * 
 */
#ifndef LINKMANAGER_API_MODULES_DEVICE_H
#define LINKMANAGER_API_MODULES_DEVICE_H

#include <string>
#include <functional>

#include <liberate/net/network.h>

namespace linkmanager::api::modules {


/**
 * The device class represents a network device ready for networking.
 *
 * Some virtual links may provide more than one network device, e.g.
 * for Ethernet or IP emulation, etc.
 *
 * Devices can be up or down, and if up, contain network configuration
 * information. TODO currently, mostly IP-based network addresses and
 * routing information is available.
 */
class device
{
public:
  virtual ~device() = default;

  /**
   * Return a name for the device. If possible, this should be the Linux
   * network device.
   */
  virtual std::string name() const = 0;


  /**
   * Get or set the up status.
   */
  virtual bool is_up() const = 0;
  inline bool is_down() const
  {
    return !is_up();
  }

  virtual void set_updown(bool new_state) = 0;


  /**
   * Retrieve the device networks. A device may have multiple networks
   * associated with it. A network instance's local_address() typically
   * represents the device's own address, the rest the remaining network
   * configuration.
   */
  using network_list = std::list<liberate::net::network>;
  virtual network_list get_networks() const = 0;

  // TODO need more here? Yes, something for reporting metrics!
};

} // namespace linkmanager::api::modules

#endif // guard
