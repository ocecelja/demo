/*
 * 
 */
#include <linkmanager/module_registry.h>

#include <dlfcn.h>

namespace linkmanager {

void
module_registry::register_module(link_module_ptr module, std::string name_override /* = {} */)
{
  if (!module) {
    throw std::invalid_argument("Cannot register null module.");
  }

  auto name = module->name();
  if (!name_override.empty()) {
    name = name_override;
  }

  auto iter = m_modules.find(name);
  if (iter != m_modules.end()) {
    throw std::invalid_argument("Module already registered: " + name);
  }

  m_modules[name] = module;
}



module_registry::module_map const &
module_registry::modules() const
{
  return m_modules;
}



void
module_registry::load_modules(std::filesystem::path path)
{
  namespace fs = std::filesystem;

  if (!fs::is_directory(path)) {
    throw std::invalid_argument("Path is not a directory!");
  }

  for (auto const & entry : fs::directory_iterator{path}) {
    // We consider regular files and symlinks - if the symlink points to
    // something bad, dlopen() will catch it.
    if (!fs::is_regular_file(entry) && !fs::is_symlink(entry)) {
      continue;
    }

    // Try opening via dlopen - this could fail for a number of reasons.
    auto handle = dlopen(entry.path().c_str(), RTLD_LAZY);
    if (!handle) {
      // Could not dlopen(), silently ignore!
      continue;
    }

    // Find init function
    init_link_plugin_t init_func = reinterpret_cast<init_link_plugin_t>(dlsym(handle, "init_link_plugin"));
    if (!init_func) {
      // No init function; dlclose() and ignore.
      dlclose(handle);
      continue;
    }

    // Call init function. We'll try to catch exceptions, if any, but leave the
    // plugin handle open unless there is anything.
    try {
      init_func(this);
    } catch (...) {
      dlclose(handle);
    }
  }
}

} // namespace linkmanager
