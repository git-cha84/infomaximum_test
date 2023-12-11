
#include "RequestCreator.h"


auto RequestCreator::runFillQueue() -> bool{
    try {
        this->fill_thr_ = std::thread(&RequestCreator::fillingProc, this);
    } catch (...) {
        debugLog("[RequestCreator::runFillQueue]: Creation thread was FAILED");
        return false;
    }
    debugLog("[RequestCreator::runFillQueue]: Was created thread");
    return true;
}

auto RequestCreator::fillingProc() ->void{
    int i = 0;
    while (!this->stop_fill_ && ((++i)<this->MAX_CREATION_REQUEST_)) {
        auto * r = this->producer_req_();
        std::shared_ptr<Request*> req=std::make_shared<Request*>( r);
        this->req_queue_.push(req);
        debugLog("[RequestCreator::fillingProc]: Push request from creator: id=",req.operator*()->getId());
    }
    this->stopListeners();
    debugLog("[RequestCreator::fillingProc]: Invoke listeners for stopping");
}

auto RequestCreator::waitFilling()->void{

        try {
            if (this->fill_thr_.joinable()) {
                this->fill_thr_.join();
                debugLog("[RequestCreator::waitFilling]: Destructed thread");
            }
        } catch (...) {
            debugLog("[RequestCreator::waitFilling]: Destructed thread FAILED");
        }

}

RequestCreator::RequestCreator(SafeThreadQueue<std::shared_ptr<Request*>> & queueTasks):req_queue_{queueTasks} {
    debugLog("[RequestCreator::RequestCreator]: invoked");
}

RequestCreator::~RequestCreator() {
    debugLog("[RequestCreator::~RequestCreator()]: invoked");
    this->stopFill();
    this->waitFilling();
    this->stopListeners();
    debugLog("[RequestCreator::~RequestCreator()]: End Destructor");
}

auto RequestCreator::stopFill() -> void {
    this->stop_fill_=true;
    debugLog("[RequestCreator::stopFill]: Set stop creation request");
}

auto RequestCreator::addStopList(std::shared_ptr<StopListener> & stopListen) -> void {
    this->stop_listens_.push_back(stopListen);
}

auto RequestCreator::stopListeners() -> void {
    for(auto const & stopper:this->stop_listens_)
        stopper->stopping();
}

auto RequestCreator::setProduceReqFunc(const std::function<Request *()> &func) -> void {
    this->producer_req_=func;
}
