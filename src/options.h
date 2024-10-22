/*
 * 
 */
#ifndef LINKMANAGER_OPTIONS_H
#define LINKMANAGER_OPTIONS_H

#include <string>
#include <vector>

#include <liberate/logging.h>

#include <nlohmann/json.hpp>

namespace linkmanager {

/**
 * Commands implemented in the CLI
 */
enum commands : uint8_t
{
  CMD_LSMOD   = 0,
  CMD_DAEMON  = 10,
  CMD_CONNECTION_MANAGER = 20,
};


/**
 * CLI options
 */
struct options
{
  commands                  cmd;
  spdlog::level::level_enum log_level = spdlog::level::info;

  // Configuration as loaded from a file.
  nlohmann::json            config = "{}"_json;

  // Command line options.
  std::vector<std::string>  plugin_path;
};

} // namespace linkmanager

#endif // guard
