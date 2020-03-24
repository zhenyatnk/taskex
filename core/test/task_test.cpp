#include <taskex/core/taskex.h>

#include <gtest/gtest.h>
#include <atomic>

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

TEST_F(task_test, CreateCollection_void)
{
    std::atomic<int> sum;
    sum = 0;
    {
        CreateCollection<int>([&sum](int a){sum += a;}, {1,2,3,4,5});
    }
    ASSERT_EQ(sum, 15);
}

TEST_F(task_test, CreateCollection_notvoid)
{
    std::atomic<int> sum;
    sum = 0;
    {
        CreateCollection<int, int>([&sum](int a){return sum += a;}, {1,2,3,4,5});
    }
    ASSERT_EQ(sum, 15);
}

TEST_F(task_test, ContainTasks_empty)
{
    ASSERT_NO_THROW(ContainTasks({}).get());
}

TEST_F(task_test, ContainTasks_void_notempty)
{
    int a1= 0;
    int a2 = 0;
    std::vector<Task<void>> tasks;
    tasks.push_back(Task<void>([&a1](){a1 = 1;}));
    tasks.push_back(Task<void>([&a2](){a2 = 2;}));
    ContainTasks(std::move(tasks)).get();
    ASSERT_EQ(a1, 1);
    ASSERT_EQ(a2, 2);
}
TEST_F(task_test, ContainTasks_notvoid_notempty)
{
    int a1= 0;
    int a2 = 0;
    std::vector<Task<int>> tasks;
    tasks.push_back(Task<int>([&a1](){return 1;}));
    tasks.push_back(Task<int>([&a2](){return 2;}));
    auto ret = ContainTasks(std::move(tasks)).get();
    ASSERT_EQ(ret[0], 1);
    ASSERT_EQ(ret[1], 2);
}

TEST_F(task_test, CreateCollection_ContainTasks)
{
    auto task = ContainTasks(CreateCollection<int, int>([](int a){ return a*a;}, {1,2,3,4,5}))
                              .then<int>([](const auto& elements)
                              {
                                  int a = 0;
                                  for(const auto& el:elements)
                                      a += el;
                                  return a;
                              });
    ASSERT_EQ(task.get(), 55);
}
