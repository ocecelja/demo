/*
 * 
 */
#include "commands.h"

#include <map>

#include "command/lsmod.h"
#include "command/daemon.h"
#include "command/connection_manager.h"

namespace linkmanager {
namespace {


static std::map<commands, command_base *> the_commands = {
  { CMD_LSMOD, new command::lsmod{} },
  { CMD_DAEMON, new command::daemon{} },
  { CMD_CONNECTION_MANAGER, new command::connection_manager{} },
};

} // anonymous namespace


command_base *
get_command(commands cmd)
{
  auto iter = the_commands.find(cmd);
  if (iter == the_commands.end()) {
    return nullptr;
  }
  return iter->second;
}

} // namespace linkmanager
