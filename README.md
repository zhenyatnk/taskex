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
