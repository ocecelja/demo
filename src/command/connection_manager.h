/*
 * 
 */
#ifndef LINKMANAGER_COMMANDS_CONNECTION_MANAGER_H
#define LINKMANAGER_COMMANDS_CONNECTION_MANAGER_H

#include "../commands.h"

namespace linkmanager::command {


class connection_manager : public command_base
{
public:
  virtual ~connection_manager() = default;
  inline connection_manager() {}

  virtual int run(options const & opts, module_registry & registry) final;
};


} // namespace linkmanager::command

#endif // guard
