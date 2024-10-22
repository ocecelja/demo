/*
 * 
 */
#ifndef LINKMANAGER_API_MODULES_PLUGIN_H
#define LINKMANAGER_API_MODULES_PLUGIN_H

/**
 * The plugin architecture aims at simplicity and flexibility. In order to
 * bridge the gap between C and C++, a simple C function for initialising
 * a plugin is provided.
 *
 * The module_registry will call this function passing a pointer to itself
 * as the only argument. This pointer is guaranteed to be non-null. The plugin
 * may perform any kind of initialisation in this function, but is generally
 * expected to call the registry's register_module() function with any
 * link_module implementations it provides.
 *
 * Plugins are *not* unloaded. This would require managing the lifetime of
 * objects created during the init phase together with the lifetime of the
 * plugin - since that can get complicated, a plugin once loaded stays loaded.
 *
 * It is partially for this reason that the link_module class contains
 * functionality for e.g. resetting hardware.
 *
 * In a plugin, provide an extern "C" function matching the typedef below with
 * the symbol name "init_link_plugin".
 *
 * Any other symbols should be hidden, in unique or anonymous namespaces.
 */
typedef void (*init_link_plugin_t)(void *);

#endif // guard
