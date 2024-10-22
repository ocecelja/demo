/*
 * 
 */
#ifndef LINKMANAGER_API_MODULES_MODULE_H
#define LINKMANAGER_API_MODULES_MODULE_H

#include <string>
#include <memory>
#include <list>

#include <nlohmann/json.hpp>

namespace linkmanager::api::modules {

/**
 * Forward declarations
 */
class link;

/**
 * The link_module class represents a hardware module and/or its driver. Each
 * hardware module must be managed by exactly one link_module.
 *
 * It does not contain much functionality of its own, except for querying the
 * availability of the hardware itself.
 *
 * Beyond that, it provides access to one or more links. Links represent the
 * physical communications units a link_module manages. For example, a multi-
 * SIM LTE model link_module would provide a link per SIM slot.
 */
class link_module
{
public:
  virtual ~link_module() = default;

  /**
   * Return a name for the link module. Link modules must have unique names.
   */
  virtual std::string name() const = 0;
  // TODO description, e.g. for lsmod?

  /**
   * Query and set power state. When setting the power state, the return value
   * is the new power state. If that is different from the passed parameter,
   * then the operation may have failed.
   */
  virtual bool is_powered_on() const = 0;
  virtual bool set_powered_on(bool new_state) = 0;

  // TODO include some kind of hang detection function here

  // ************************CUSTOM*************************************
  virtual void registerLink();
  // *******************************************************************

  /**
   * Access individual links. If the list is empty the link_module cannot
   * be used.
   * TODO: this may require powering it on, or some similar operation.
   */
  using link_list = std::list<std::shared_ptr<link>>;
  virtual link_list links() const = 0;
};

} // namespace linkmanager::api::modules

#endif // guard
