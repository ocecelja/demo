/*
 * 
 */
#ifndef LINKMANAGER_COMMANDS_H
#define LINKMANAGER_COMMANDS_H

#include "options.h"

#include <linkmanager/module_registry.h>

namespace linkmanager {

/**
 * Command interface
 */
class command_base
{
public:
  virtual ~command_base() = default;
  virtual int run(options const & opts, module_registry & registry) = 0;
};


/**
 * Registered commands.
 */
command_base * get_command(commands cmd);

} // namespace linkmanager

#endif // guard
