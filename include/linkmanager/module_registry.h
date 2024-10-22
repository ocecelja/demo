/*
 * 
 */
#ifndef LINKMANAGER_MODULE_REGISTRY_H
#define LINKMANAGER_MODULE_REGISTRY_H

#include <map>
#include <string>
#include <memory>
#include <exception>
#include <filesystem>

#include <linkmanager/api/modules.h>

namespace linkmanager {

/**
 * Link module registry
 */
class module_registry
{
public:
  using link_module_ptr = std::shared_ptr<linkmanager::api::modules::link_module>;
  using module_map = std::map<
    std::string,
    link_module_ptr
  >;

  /**
   * Register link module.
   *
   * May raise exceptions. When a module with the same name is already
   * registered, std::invalid_argument is raised.
   */
  void register_module(link_module_ptr module, std::string name_override = {});

  /**
   * Return information about registered modules.
   */
  module_map const & modules() const;

  inline std::size_t size() const
  {
    return modules().size();
  }

  inline bool empty() const
  {
    return modules().empty();
  }

  /**
   * Load DSO modules from the following path. Any file will be considered,
   * but the convention is to name them <somethign>_plugin.so
   */
  void load_modules(std::filesystem::path path);


private:
  module_map  m_modules;
};


} // namespace linkmanager

#endif // guard
