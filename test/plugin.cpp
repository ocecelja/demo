/*
 * 
 */

#include <iostream>

#include <linkmanager/module_registry.h>
#include <linkmanager/api/modules.h>

namespace {

class test_link : public linkmanager::api::modules::link
{
public:
  inline explicit test_link()
  {}

  inline bool trialmethod()
  {
    return true;
  }

  virtual std::string name() const final
  {
    return "plugin_test_link#";
  }

  virtual bool configure(nlohmann::json const & config) final
  {
    std::cerr << "Applying configuration: " << config << std::endl;
    return true;
  }

  virtual bool is_active() const final
  {
    return m_active;
  }

  virtual void set_active(bool new_status, activation_callback cb) final
  {
    m_active = new_status;
    cb(*this, new_status);
  }

  virtual device_list devices() const
  {
    return m_devices;
  }

  bool        m_active = false;
  device_list m_devices = {};
};

class test_module : public linkmanager::api::modules::link_module
{
 public: 
  inline test_module()
  {
  }

  virtual std::string name() const final
  {
    return "EM919x Sierra Wireless LTE Module";
  }

  virtual bool configure_link(nlohmann::json const & config) final
  {
    
  }

  virtual bool is_powered_on() const final
  {
    return true;
  }

  virtual bool set_powered_on(bool new_state [[maybe_unused]]) final
  {
    return true;
  }

  virtual link_list links() const final
  {
    return list_test;
  }

  link_list list_test = {};

  char s_DevicePath[256];
  char s_InterfaceName[256];
  char s_SimPin[16];

};

} // anonymous namespace

extern "C" {


void init_link_plugin(void * _reg)
{
  if (!_reg) {
    // Must not happen!
    std::cerr << "Bad registry, aborting." << std::endl;
    return;
  }

  auto mr = reinterpret_cast<linkmanager::module_registry *>(_reg);
  mr->register_module(std::make_shared<test_module>());
}


} // extern "C"
