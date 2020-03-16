#pragma once

#include <future>
#include <functional>

namespace taskex {
  
template<typename RetType>
class Task final
{
public:
    explicit Task(const std::function<RetType(void)>& thenFunc)
    {
        m_future = std::async(std::launch::async, [=](){return thenFunc();});
    }
    template<typename TaskParms>
    Task(const std::function<RetType(void)>& thenFunc, std::future<TaskParms>&& params)
    {
        m_future = std::async(std::launch::async, [thenFunc, parm = std::move(params)]() mutable
        {
            parm.get();
            return thenFunc();
        });
    }
    template<typename TaskParms>
    Task(const std::function<RetType(TaskParms)>& thenFunc, std::future<TaskParms>&& params)
    {
        m_future = std::async(std::launch::async, [thenFunc, parm = std::move(params)]() mutable
        {
            return thenFunc(parm.get());
        });
    }
    RetType get()
    {
        if(m_future.valid())
           return m_future.get();
        return m_value;
    }

    template<typename RetThen>
    auto then(const std::function<RetThen(RetType)>& thenFunc)
    {
        return Task<RetThen>(thenFunc, std::move(m_future));
    }
    template<typename RetThen>
    auto then(const std::function<RetThen(void)>& thenFunc)
    {
        return Task<RetThen>(thenFunc, std::move(m_future));
    }
    
protected:
    std::future<RetType> m_future;
    RetType m_value = {};
};
template<>
class Task<void> final
{
public:
    explicit Task(const std::function<void(void)>& thenFunc)
    {
        m_future = std::async(std::launch::async, [=](){return thenFunc();});
    }
    template<typename TaskParms>
    Task(const std::function<void(void)>& thenFunc, std::future<TaskParms>&& params)
    {
        m_future = std::async(std::launch::async, [thenFunc, parm = std::move(params)]() mutable
        {
            parm.get();
            return thenFunc();
        });
    }
    template<typename TaskParms>
    Task(const std::function<void(TaskParms)>& thenFunc, std::future<TaskParms>&& params)
    {
        m_future = std::async(std::launch::async, [thenFunc, parm = std::move(params)]() mutable
        {
            return thenFunc(parm.get());
        });
    }
    void get()
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
    std::future<void> m_future;
};

}
