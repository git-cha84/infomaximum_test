
#include "Request.h"

int Request::count = 0;
std::mutex Request::count_mutex;
std::mt19937 Request::rand_gen_;

Request::Request() {
    const std::lock_guard<std::mutex> lock(Request::count_mutex);
    Request::count++;
    this->id = Request::count;
    this->rand_int_=(int)this->rand_gen_() % GlobConf::MAX_RAND;
    debugLog("[Request::Request()]: Created Request id=",this->id);
}

auto Request::getRand() ->int {
    return this->rand_int_;
}

Request::~Request() {
    debugLog("[Request::~Request()]: Destructed Request id=",this->id);
}

auto Request::getId() ->int {
    return this->id;
}
