/*
 * 
 */

#include <linkmanager/module_registry.h>

#include <gtest/gtest.h>

using namespace linkmanager;

namespace {

class test_module : public linkmanager::api::modules::link_module
{
  virtual std::string name() const final
  {
    return "test_module";
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
    return {};
  }
};

} // anonymous namespace


TEST(ModuleRegistry, register_null_module_failure)
{
  auto mod = std::shared_ptr<test_module>();
  module_registry mr;

  ASSERT_TRUE(mr.empty());
  ASSERT_THROW(mr.register_module(mod), std::invalid_argument);
  ASSERT_TRUE(mr.empty());
}



TEST(ModuleRegistry, register_test_module_success)
{
  auto mod = std::make_shared<test_module>();
  module_registry mr;

  ASSERT_TRUE(mr.empty());
  ASSERT_NO_THROW(mr.register_module(mod));
  ASSERT_FALSE(mr.empty());
}



TEST(ModuleRegistry, register_test_module_double_name_failure)
{
  auto mod = std::make_shared<test_module>();
  module_registry mr;

  ASSERT_TRUE(mr.empty());
  ASSERT_NO_THROW(mr.register_module(mod));
  ASSERT_FALSE(mr.empty());

  ASSERT_THROW(mr.register_module(mod), std::invalid_argument);
}



TEST(ModuleRegistry, register_test_module_name_override)
{
  auto mod = std::make_shared<test_module>();
  module_registry mr;

  ASSERT_EQ(0, mr.size());
  ASSERT_NO_THROW(mr.register_module(mod));
  ASSERT_EQ(1, mr.size());

  ASSERT_NO_THROW(mr.register_module(mod, "override"));
  ASSERT_EQ(2, mr.size());
}

// See runner.cpp for how accessing the command line works.
extern char const * cli_name;

TEST(ModuleRegistry, register_plugins)
{
  auto dir = std::filesystem::path(cli_name).parent_path();
  auto abs = std::filesystem::absolute(dir);

  module_registry mr;

  ASSERT_EQ(0, mr.size());
  ASSERT_NO_THROW(mr.load_modules(abs));
  ASSERT_EQ(1, mr.size());
}
