/*
 * 
 */

#include <iostream>

#include <linkmanager/module_registry.h>

#include "lsmod.h"

namespace linkmanager::command {

int
lsmod::run(options const & opts, module_registry & registry)
{
  std::cout << "Available link modules:" << std::endl;
  for (auto [name, _testPointer] : registry.modules()) {
    std::cout << "  " << name << std::endl;
  }

  return 0;
}

} // namespace linkmanager::command
