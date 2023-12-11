#ifndef TEST_TASK_REQUEST_H
#define TEST_TASK_REQUEST_H

#include <mutex>
#include <random>

#include "Log.hpp"
#include "GlobalConfig.h"

class Request {
private:

    static int count;
    static std::mutex count_mutex;
    static std::mt19937 rand_gen_;
    int id;
    int rand_int_;
public:
    Request();
    auto getRand()->int;
    auto  getId() ->int;

    virtual ~Request();

};

#endif //TEST_TASK_REQUEST_H
