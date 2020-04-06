# TASKEX

[![Build Status](https://travis-ci.org/zhenyatnk/taskex.svg?branch=master)](https://travis-ci.org/zhenyatnk/taskex)
[![Build status](https://ci.appveyor.com/api/projects/status/x12noh5yjnlxi2hv?svg=true)](https://ci.appveyor.com/project/zhenyatnk/taskex)

Header only library for programming using tasks.

## Setting Up the Environment

Requered C++14.

## Build and Run

Just make build folder, run CMake and then edit/build the project:

```
md .build
cd .build
cmake ..
cmake --build .
```

## How to add library in project
This is header only library, for using need include header file.
``` cpp
#include <taskex/core/taskex.h>
```

## How to use
For use need create and manange task or group tasks.

### Step 1: Create task
Create task with callable function in parameter template use function return type.
``` cpp
   auto task = taskex::Task<int>([]{return 1;})
```

### Step 2: Wait result
For wait and get result function use method `get`
``` cpp
   int a = task.get(); // a = 1
```

## Dependencies
The start of a task may depend on the completion of another task. For such a dependence use method `then` for task.
Method `then` have template argument this is return type for function.
``` cpp
   auto task = taskex::Task<int>([]{return 1;})
   auto new_task = task.then<int>([](const auto& el){ return ++el;});
   auto result = new_task.get(); // a = 2
```

## Usage Samples

### Convert return type from first task
``` cpp
int main()
{
   std::cout << taskex::Task<int>([]{return 1;})
                .then<std::string>([](const auto& el){ return std::to_string(el);})
                .get(); // in display 1
   return 0;
}
```
