/*
 * 
 */
#ifndef LINKMANAGER_COMMANDS_LSMOD_H
#define LINKMANAGER_COMMANDS_LSMOD_H

#include "../commands.h"

namespace linkmanager::command {


class lsmod : public command_base
{
public:
  virtual ~lsmod() = default;
  inline lsmod() {}

  virtual int run(options const & opts, module_registry & registry) final;
};


} // namespace linkmanager::command

#endif // guard
