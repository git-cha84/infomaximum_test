
#include <memory>

#include "RequestCreator.h"
#include "PoolWorkThreads.h"
#include "Request.h"
#include "SafeThreadQueue.h"
#include "StopListener.h"
#include "Log.hpp"

void longRunningFunction() {
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        if (elapsedTime.count() >= GlobConf::RUNNING_TIME) {
            break;
        }
    }
}

static auto GetRequest = []() throw() {
    auto *req = new Request();
    int rand = req->getRand();
    while (--rand > 0)
        longRunningFunction();
    debugLog("[Create Request]: Created Req id=", req->getId());
    return req;
};

static auto ProcessRequest = [](Request *req) throw() {
    int rand = GlobConf::MORE_COMPUT_COMPLEXETY * req->getRand();
    while (--rand > 0)
        longRunningFunction();
    debugLog("[Processor Request]: Processed Req id=", req->getId());
};

int main() {

    debugLogFile("log.txt");
    debugLog("Begin Program");
    SafeThreadQueue<std::shared_ptr<Request *>> queueReq;

    std::shared_ptr<PoolWorkThreads> poolThr = std::make_shared<PoolWorkThreads>(queueReq);
    poolThr->setProcessReqFunc(ProcessRequest);

    for (int i = 0; i < GlobConf::NUMBER_OF_THREADS;  poolThr->runProcessQueue()?i++:0){}


    std::shared_ptr<RequestCreator> reqCreator = std::make_shared<RequestCreator>(queueReq);
    reqCreator->setProduceReqFunc(GetRequest);
    std::shared_ptr<StopListener> listener = poolThr;
    reqCreator->addStopList(listener);
    reqCreator->runFillQueue();
    reqCreator->waitFilling();


    debugLog("End Program");
    return 0;
}