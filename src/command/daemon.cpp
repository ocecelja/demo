/*
 * 
 */
#include "../build-config.h"

#include <signal.h>

#include <iostream>

#include <linkmanager/module_registry.h>

#include "daemon.h"

#include <liberate/logging.h>

namespace linkmanager::command {
namespace {

static volatile bool keep_running = true;

void signal_handler(int sig)
{
  if (sig == SIGINT) {
    keep_running = false;
  }
}


inline void install_sigint_handler()
{
  auto res = ::signal(SIGINT, signal_handler);
  if (res == SIG_ERR) {
    LIBLOG_ERROR("Could not install signal handler; this will lead to unclean shutdowns.");
  }
}

} // anonymous namespace


int
daemon::run(options const & opts, module_registry & registry)
{
  // Set the log level again to the chosen one.
  LIBLOG_INFO("Log level is: " << spdlog::level::to_string_view(opts.log_level).data());
  spdlog::set_level(opts.log_level);

  // Log version informationd and status line
  LIBLOG_INFO("This is " LINKMANAGER_PACKAGE_NAME " version " LINKMANAGER_PACKAGE_VERSION);

  // TODO

  LIBLOG_INFO("Initialized, entering run loop.");

  // Run loop
  install_sigint_handler();
  while (keep_running) {
    // TODO
  }
  LIBLOG_INFO("Run loop ended.");

  return 0;
}

} // namespace linkmanager::command
