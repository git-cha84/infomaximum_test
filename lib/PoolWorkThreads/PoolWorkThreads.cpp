

#include "PoolWorkThreads.h"

auto PoolWorkThreads::threadProc() -> void {

    debugLog("[PoolWorkThreads::threadProc]: Begin thread id=",this->thr_count_.load());
    std::shared_ptr<Request*> req = nullptr;
    while (this->tasks_queue_.pop(req) && !this->stop_all_threads_) {
        Request* reqPtr=req.operator*();
        this->process_req_(reqPtr);
        this->processed_req_count_++;
    }
}

auto PoolWorkThreads::runProcessQueue() ->  bool {
    debugLog("[PoolWorkThreads::runProcessQueue]: Begin");
    try {
        std::thread workThr = std::thread(&PoolWorkThreads::threadProc, this);
        this->work_threads_.push_back(std::move(workThr));
    } catch (...) {
        debugLog("[PoolWorkThreads::runProcessQueue]: End. Error creation thread");
        return false;
    }
    debugLog("[PoolWorkThreads::runProcessQueue]: End. Thread was created");
    return true;
}

PoolWorkThreads::PoolWorkThreads(SafeThreadQueue<std::shared_ptr<Request*>> & queueTasks) : tasks_queue_{queueTasks} {

}

auto PoolWorkThreads::waitProcessing() -> void {


        for (auto &thr:this->work_threads_) {
            try {
                debugLog("[PoolWorkThreads::waitProcessing]: Begin stop thread");
                if (thr.joinable()) {
                    thr.join();
                    debugLog("[PoolWorkThreads::waitProcessing]: Was destructed thread");
                }
            } catch (...) {
                debugLog("[PoolWorkThreads::waitProcessing]: Error destructed thread");
            }
        }
    debugLog("[PoolWorkThreads::waitProcessing]: End ");
}

PoolWorkThreads::~PoolWorkThreads() {
    debugLog("[PoolWorkThreads::~PoolWorkThreads()]: invoked");
    this->stopProcession();
    this->waitProcessing();
    debugLog("[PoolWorkThreads::~PoolWorkThreads()]: Processed Request: ",this->processed_req_count_.load());
    debugLog("[PoolWorkThreads::~PoolWorkThreads()]: End Destructor");
}

auto PoolWorkThreads::stopProcession() -> void {
    this->stop_all_threads_ = true;
    this->tasks_queue_.selectUnBlockMode();
    debugLog("[PoolWorkThreads::stopProcession]: Set stopping all threads in TRUE");
}

auto PoolWorkThreads::stopping() -> void {
    this->stopProcession();
    this->waitProcessing();
}

auto PoolWorkThreads::setProcessReqFunc(const std::function<void(Request *)> &func) -> void {
    this->process_req_=func;
}
