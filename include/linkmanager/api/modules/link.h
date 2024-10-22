/*
 * 
 */
#ifndef LINKMANAGER_API_MODULES_LINK_H
#define LINKMANAGER_API_MODULES_LINK_H

#include <string>
#include <list>
#include <functional>

#include <nlohmann/json.hpp>

namespace linkmanager::api::modules {

/**
 * Forward
 */
class device;


/**
 * The link class represents a logical link as provided by a link_module.
 *
 * hardware module and/or its driver. Each
 * hardware module must be managed by exactly one link_module.
 *
 * It does not contain much functionality of its own, except for querying the
 * availability of the hardware itself.
 *
 * Beyond that, it provides access to one or more links. Links represent the
 * physical communications units a link_module manages. For example, a multi-
 * SIM LTE model link_module would provide a link per SIM slot.
 */
class link
{
public:
  virtual ~link() = default;

  /**
   * Return a name for the link. Link names are unique to their enclosing
   * link_module.
   */
  virtual std::string name() const = 0;


  /**
   * Logical links can be configured with configuration data. The precise
   * structure cannot be determined for all types of link in advance. For
   * that reason, we provide a JSON structure here as a convenience. The
   * data type works nicely as a flexible structure in native code, and it
   * can be taken e.g. from a configuration file section of the link manager.
   *
   * Return true if the configuration has been accepted, false otherwise.
   */
  virtual bool configure(nlohmann::json const & config) = 0;


  /**
   * Get or set activation status of the virtual link. This may apply the
   * configuration above. Activating a link may take a while. It is preferrable
   * if this function returns immediately, but in case that is not possible,
   * an activation callback function must be provided.
   */
  using activation_callback = std::function<void (link &, bool)>;

  virtual bool is_active() const = 0;
  virtual void set_active(bool new_status, activation_callback callback) = 0;

  // ************************CUSTOM*************************************
  virtual void registerDevice();
  // *******************************************************************

  /**
   * Access individual devices.
   *
   * A virtual link may provide one or more devices, equivalent to or directly
   * represented by Linux network devices. Devices are the object the Scheduler
   * uses.
   *
   * The list may be empty for inactive links.
   */
  using device_list = std::list<std::shared_ptr<device>>;
  virtual device_list devices() const = 0;
};

} // namespace linkmanager::api::modules

#endif // guard
