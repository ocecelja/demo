/*
 * 
 */
#ifndef LINKMANAGER_COMMANDS_DAEMON_H
#define LINKMANAGER_COMMANDS_DAEMON_H

#include "../commands.h"

namespace linkmanager::command {


class daemon : public command_base
{
public:
  virtual ~daemon() = default;
  inline daemon() {}

  virtual int run(options const & opts, module_registry & registry) final;
};


} // namespace linkmanager::command

#endif // guard
