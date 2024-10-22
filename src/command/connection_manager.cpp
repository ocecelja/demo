/*
 * 
 */

#include <iostream>

#include <linkmanager/module_registry.h>

#include "connection_manager.h"

namespace linkmanager::command {

int
connection_manager::run(options const & opts, module_registry & registry)
{
  std::cout << "Available link modules:" << std::endl;
  for (auto [name, _testPointer] : registry.modules()) {
    // std::cout << "  " << name << std::endl;
    std::cout << "  " << _testPointer->name() << std::endl;
    // std::cout << "  " << _testPointer->links() << std::endl;
  }

  return 0;
}

} // namespace linkmanager::command
