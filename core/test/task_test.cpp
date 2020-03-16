#include <taskex/core/taskex.h>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace taskex;

class task_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(task_test, ctor)
{
    Task<void>([](){});
}
TEST_F(task_test, ret_int)
{
    ASSERT_EQ(1, Task<int>([]{return 1;}).get());
}
TEST_F(task_test, then)
{
    ASSERT_EQ(2, Task<int>([]{return 1;})
              .then<int>([](const auto& el){ return el + 1;})
              .get());
}
TEST_F(task_test, then_in_void)
{
    ASSERT_STREQ("test", Task<int>([]{return 1;})
              .then<std::string>([]{ return "test";})
              .get().c_str());
}
TEST_F(task_test, then_out_void)
{
    int result = 0;
    ASSERT_NO_THROW(Task<int>([]{return 1;})
                    .then<void>([&result](const auto& el){ result = 3;})
              .get());
    ASSERT_EQ(3, result);
}
TEST_F(task_test, then_inout_void)
{
    int result = 0;
    ASSERT_NO_THROW(Task<int>([]{return 1;})
                    .then<void>([&result]{ result = 3;})
              .get());
    ASSERT_EQ(3, result);
}

TEST_F(task_test, then_any_ret)
{
    ASSERT_STREQ("test", Task<int>([]{return 1;})
              .then<std::string>([](const auto&){ return "test";})
              .get().c_str());
}
