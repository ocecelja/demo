/*
 * 
 */
#include "../build-config.h"

#include <iostream>
#include <fstream>

#include <clipp.h>

#include <liberate/logging.h>

#include <linkmanager/module_registry.h>

#include "options.h"
#include "commands.h"
#include "em919xManagement.h"

namespace linkmanager {

namespace {
static constexpr char const * versions =
  "   " LINKMANAGER_PACKAGE_NAME " version " LINKMANAGER_PACKAGE_VERSION
;

static constexpr char const * intro =
  "   TODO introduction"
;


std::string
parse_cli(options & opts, int argc, char **argv)
{
  using namespace clipp;

  bool help = false;
  std::string config;
  auto set_level = [&opts](std::string const & val) -> void
  {
    opts.log_level = spdlog::level::from_str(val);
  };

  auto cli = (
      (option("--log-level") & value("LEVEL") >> set_level)
        .doc("One of 'trace', 'debug', 'info', 'warn', 'error', critical', "
             "'off' (default: 'info')."),

      option("--help", "-?")
        .set(help)
        .doc("Display this help."),


      (option("--config", "-c") & value("FILENAME", config))
        .doc("Path of a configuration file."),


      repeatable(option("--plugins", "-p") & values("PATH", opts.plugin_path))
        .doc("Path for loading link module plugins."),

      (
        command("lsmod")
          .set(opts.cmd, CMD_LSMOD)
          .doc("List available link modules.")

      | command("daemon")
          .set(opts.cmd, CMD_DAEMON)
          .doc("Run the link manager daemon. The executable does not go into "
                "the background, but stays manageable e.g. via runit, etc.")
          
      | command("connection_manager")
          .set(opts.cmd, CMD_CONNECTION_MANAGER)
          .doc("Run the link connection_manager")
      )
  );

  auto result = parse(argc, argv, cli);
  if (!result || help) {
#if 0
    // Parameter debugging
    auto doc_label = [](parameter const & p) {
        if (!p.flags().empty()) {
          return p.flags().front();
        }
        if (!p.label().empty()) {
          return p.label();
        }
        return doc_string{"<?>"};
    };

    for (auto const & m : result) {
      std::cout << "#" << m.index() << " " << m.arg() << " -> ";
      auto p = m.param();
      if (p) {
        std::cout << doc_label(*p) << " \t";
        if (m.repeat() > 0) {
          std::cout << (m.bad_repeat() ? "[bad repeat " : "[repeat ")
                    << m.repeat() << "]";
        }
        if (m.blocked()) {
          std::cout << " [blocked]";
        }
        if (m.conflict()) {
          std::cout << " [conflict]";
        }
        std::cout << std::endl;
      }
      else {
        std::cout << " [unmapped]";
      }
    }
//cout << "missing parameters:\n";
//for(const auto& m : result.missing()) {
//    auto p = m.param();
//    if(p) {
//        os << doc_label(*p) << " \t";
//        os << " [missing after " << m.after_index() << "]\n";
//    }
//}
#endif

    auto fmt = doc_formatting{}
        .first_column(3)
        .last_column(79);
    std::cerr << make_man_page(cli, argv[0], fmt)
        .prepend_section("INTRODUCTION", intro)
        .prepend_section("VERSION INFORMATION", versions)
      ;

    exit(1);
  }

  return config;
}

void
parse_config(std::string const & filename, options & opts)
{
  if (filename.empty()) {
    return;
  }

  std::ifstream i(filename);
  i >> opts.config;
}

} // anonymous namespace
} // namespace linkmanager



int main(int argc, char **argv)
{
  using namespace linkmanager;

  // First things first: set log level to trace, so we can see all *initial*
  // log output until the CLI is fully processed.
  spdlog::set_level(spdlog::level::trace);

  // Parse command line
  options opts;
  auto config_filename = parse_cli(opts, argc, argv);

  // Parse configuration
  parse_config(config_filename, opts);

  // Initialise plugin registry. We can treat plugin paths given on the command
  // line as *additional* to those given in a config file, if any.
  linkmanager::module_registry registry;
  if (opts.config.contains("plugin_path")) {
    auto pp = opts.config["plugin_path"];
    if (pp.is_string()) {
      registry.load_modules(pp.get<std::string>());
    }
    else if (pp.is_array()) {
      for (auto & p : pp) {
        registry.load_modules(p.get<std::string>());
      }
    }
    else {
      std::cerr << "Invalid plugin_path format in config file: " << config_filename << std::endl;
      exit(2);
    }
  }
  for (auto const & p : opts.plugin_path) {
    registry.load_modules(p);
  }

  for (auto [name, mod] : registry.modules()) {
    // std::cout << mod->is_powered_on() << "\n";
    // std::cout << mod->name() << "\n";
    // std::cout << mod->links().size() << "\n";
    std::cout << configure_link(opts.config) << "\n";
    // mod->configure_link(opts.config);
    // std::cout << mod->links().size() << "\n";

  }

  // // Run the specified command.
  // auto cmd = get_command(opts.cmd);
  // if (!cmd) {
  //   std::cerr << "Internal error: command not implemented." << std::endl;
  //   exit(-1);
  // }
  // return cmd->run(opts, registry);
}
