
#include <iostream>

#include <linkmanager/module_registry.h>
#include <linkmanager/api/modules.h>

namespace {

class test_link : public linkmanager::api::modules::link
{

  test_link(){}

  virtual std::string name() const final
  {
    return "plugin_test_link";
  }

  virtual bool is_active() const final
  {
    return true;
  }

  virtual void set_active(bool new_status, activation_callback callback) final
  {
    return;
  }

  virtual device_list devices() const final
  {
    return {};
  }
};
}
