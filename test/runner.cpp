/*
 * 
 */
#include <gtest/gtest.h>

char const * cli_name = nullptr;

int main(int argc, char **argv)
{
  cli_name = argv[0];

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
