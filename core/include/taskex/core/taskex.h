#pragma once

#include <vector>
#include <future>
#include <functional>

namespace taskex {
  
template<typename RetType>
class Task final
{
public:
    template<typename TypeFunc>
    explicit Task(TypeFunc&& thenFunc) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc)] () mutable {return _thenFunc();});
    }
    template<typename TaskParms>
    Task(std::function<RetType(void)> thenFunc, std::future<TaskParms>&& params) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc), parm = std::move(params)]() mutable
        {
            parm.get();
            return _thenFunc();
        });
    }
    template<typename TaskParms>
    Task(std::function<RetType(TaskParms)> thenFunc, std::future<TaskParms>&& params) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc), parm = std::move(params)]() mutable
        {
            return _thenFunc(parm.get());
        });
    }
    RetType get() const
    {
        if(m_future.valid())
           m_value = m_future.get();
        return m_value;
    }

    template<typename RetThen>
    auto then(std::function<RetThen(RetType)> thenFunc)
    {
        return Task<RetThen>(std::move(thenFunc), std::move(m_future));
    }
    template<typename RetThen>
    auto then(std::function<RetThen(void)> thenFunc)
    {
        return Task<RetThen>(std::move(thenFunc), std::move(m_future));
    }
    
protected:
    mutable std::future<RetType> m_future;
    mutable RetType m_value = {};
};
template<>
class Task<void> final
{
public:
    template<typename TypeFunc>
    explicit Task(TypeFunc&& thenFunc) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc)] () mutable {return _thenFunc();});
    }
    template<typename TaskParms>
    Task(std::function<void(void)> thenFunc, std::future<TaskParms>&& params) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc), parm = std::move(params)]() mutable
        {
            parm.get();
            return _thenFunc();
        });
    }
    template<typename TaskParms>
    Task(std::function<void(TaskParms)> thenFunc, std::future<TaskParms>&& params) noexcept
    {
        m_future = std::async(std::launch::async, [_thenFunc = std::move(thenFunc), parm = std::move(params)]() mutable
        {
            return _thenFunc(parm.get());
        });
    }
    void get() const
    {
        if(m_future.valid())
            m_future.get();
    }

    template<typename RetThen>
    auto then(const std::function<RetThen(void)>& thenFunc)
    {
        return Task<RetThen>(thenFunc, std::move(m_future));
    }
    
protected:
    mutable std::future<void> m_future;
};

template<typename RetType, typename ParamType>
std::vector<Task<RetType>> CreateCollection(const std::function<RetType(ParamType)>& func, const std::vector<ParamType>& params)
{
    std::vector<Task<RetType>> ret;
    ret.reserve(params.size());
    for(const auto& param: params)
        ret.push_back(Task<RetType>([param, func](){ return func(param);}));
    return ret;
}
template<typename ParamType>
std::vector<Task<void>> CreateCollection(const std::function<void(ParamType)>& func, const std::vector<ParamType>& params)
{
    std::vector<Task<void>> ret;
    ret.reserve(params.size());
    for(const auto& param: params)
        ret.push_back(Task<void>([param, func](){ func(param);}));
    return ret;
}

template<typename RetType>
Task<std::vector<RetType>> ContainTasks(std::vector<Task<RetType>>&& tasks)
{
    return Task<std::vector<RetType>>{[_tasks = std::move(tasks)] () mutable
        {
            std::vector<RetType> ret;
            ret.reserve(_tasks.size());
            for(const auto& task: _tasks)
                ret.push_back(task.get());
            return ret;
        }};
}
Task<void> ContainTasks(std::vector<Task<void>>&& tasks)
{
    return Task<void>{[_tasks = std::move(tasks)] () mutable
        {
            for(const auto& task: _tasks)
                task.get();
        }};
}

}
