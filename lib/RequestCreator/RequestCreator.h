#ifndef TEST_TASK_REQUESTCREATOR_H
#define TEST_TASK_REQUESTCREATOR_H


#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <functional>

#include "SafeThreadQueue.h"
#include "Request.h"
#include "GlobalConfig.h"
#include "StopListener.h"

class RequestCreator {
private:
    SafeThreadQueue<std::shared_ptr<Request*> > & req_queue_;
    std::vector<std::shared_ptr<StopListener>> stop_listens_;
    const int MAX_CREATION_REQUEST_{GlobConf::MAX_CREATION_REQUEST};
    std::thread fill_thr_;
    std::atomic<bool> stop_fill_{false};
    std::function<Request*() throw()> producer_req_;

    auto fillingProc() ->void;
    auto stopListeners() ->void;


public:
    explicit RequestCreator(SafeThreadQueue<std::shared_ptr<Request*>> & queueTasks);
    RequestCreator() = delete;
    RequestCreator(const RequestCreator&) = delete;
    RequestCreator(RequestCreator&&) = delete;
    RequestCreator& operator=(const RequestCreator&) = delete;
    RequestCreator& operator=(RequestCreator&&) = delete;

    virtual ~RequestCreator();

    auto runFillQueue() -> bool;
    auto waitFilling() -> void;
    auto stopFill() -> void;
    auto addStopList(std::shared_ptr<StopListener>& stopListen) ->void;
    auto setProduceReqFunc(const std::function<Request*()>& func) ->void;
};


#endif //TEST_TASK_REQUESTCREATOR_H
