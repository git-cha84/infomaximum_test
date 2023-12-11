

#ifndef TEST_TASK_POOLWORKTHREADS_H
#define TEST_TASK_POOLWORKTHREADS_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

#include "SafeThreadQueue.h"
#include "Request.h"
#include "GlobalConfig.h"
#include "StopListener.h"

class PoolWorkThreads: public StopListener {
private:
    std::vector<std::thread> work_threads_;
    std::atomic<bool> stop_all_threads_{false};
    std::atomic<int> thr_count_{0};
    std::atomic<int> processed_req_count_{0};
    SafeThreadQueue<std::shared_ptr<Request*>> & tasks_queue_;
    std::function<void(Request*) throw()> process_req_;

    auto threadProc() -> void;
    auto stopProcession() -> void;

public:
    explicit PoolWorkThreads(SafeThreadQueue<std::shared_ptr<Request*>> & queueTasks);
    PoolWorkThreads() = delete;
    PoolWorkThreads(const PoolWorkThreads&) = delete;
    PoolWorkThreads(PoolWorkThreads&&) = delete;
    PoolWorkThreads& operator=(const PoolWorkThreads&) = delete;
    PoolWorkThreads& operator=(PoolWorkThreads&&) = delete;

    virtual ~PoolWorkThreads();

    auto runProcessQueue() ->  bool;
    auto waitProcessing() -> void;
    auto setProcessReqFunc(const std::function<void(Request*)>& func) ->void;

    auto stopping() -> void override;

};


#endif //TEST_TASK_POOLWORKTHREADS_H
