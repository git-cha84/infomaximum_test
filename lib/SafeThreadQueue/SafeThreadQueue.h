#ifndef TEST_TASK_SAFETHREADQUEUE_H
#define TEST_TASK_SAFETHREADQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

#include "Log.hpp"

template<class T>
class SafeThreadQueue : protected std::queue<T> {
private:
    std::mutex queue_mutex_;
    std::mutex wait_mutex_;
    std::condition_variable wait_cv_;
    std::atomic<int> count_{0};
    std::atomic<bool> is_block_mode_{true};

public:
    SafeThreadQueue();

    SafeThreadQueue(SafeThreadQueue &queue);

    SafeThreadQueue(const SafeThreadQueue &) = delete;

    SafeThreadQueue(SafeThreadQueue &&) = delete;

    SafeThreadQueue &operator=(const SafeThreadQueue &) = delete;

    SafeThreadQueue &operator=(SafeThreadQueue &&) = delete;

    virtual ~SafeThreadQueue() = default;

    auto push(T item) -> void;

    auto pop(T &item) -> bool;

    auto selectBlockMode() -> void;

    auto selectUnBlockMode() -> void;
};

template<class T>
auto SafeThreadQueue<T>::push(T item) -> void {
    const std::lock_guard<std::mutex> lockQueue(this->queue_mutex_);
    std::queue<T>::push(item);
    this->count_++;
    debugLog("[SafeThreadQueue<T>::push]: Decremented items counter: count=", this->count_.load());
    this->wait_cv_.notify_one();
    debugLog("[SafeThreadQueue<T>::push]: Notifing waiting pop item");
}

template<class T>
auto SafeThreadQueue<T>::pop(T &item) -> bool {
    {
        std::unique_lock<std::mutex> lockWait(this->wait_mutex_);
        this->wait_cv_.wait(lockWait, [&] {

            if (this->count_ > 0) {
                debugLog("[SafeThreadQueue<T>::pop]: UnLock becouse queue is not empty");
                return true;
            }

            if (!this->is_block_mode_) {
                debugLog("[SafeThreadQueue<T>::pop]: UnLock becouse queue was selected in unBlock mode");
                return true;
            }

            debugLog("[SafeThreadQueue<T>::pop]: Lock becouse queue is in Block mode and queue is empty");
            return false;

        });
    }
    {
        const std::lock_guard<std::mutex> lockQueue(this->queue_mutex_);
        debugLog("[SafeThreadQueue<T>::pop]: Was Lock SafeThreadQueue::queue_mutex_");

        if (std::queue<T>::empty()) {
            debugLog("[SafeThreadQueue<T>::pop]: End. Queue is empty");
            return false;
        }

        item = std::queue<T>::front();
        std::queue<T>::pop();
        this->count_--;
        debugLog("[SafeThreadQueue<T>::pop]: End and increment counter items: count_=", this->count_.load());
        return true;
    }
}

template<class T>
auto SafeThreadQueue<T>::selectBlockMode() -> void {
    this->is_block_mode_ = true;
    debugLog("[SafeThreadQueue<T>::selectBlockMode()]: Set Queue in Block mode");
}

template<class T>
auto SafeThreadQueue<T>::selectUnBlockMode() -> void {
    this->is_block_mode_ = false;
    debugLog("[SafeThreadQueue<T>::selectUnBlockMode()]: Set Queue in unBlock mode");
    this->wait_cv_.notify_all();
    debugLog("[SafeThreadQueue<T>::selectUnBlockMode()]: Notifing that queue was selected in unBlock mode");
}

template<class T>
SafeThreadQueue<T>::SafeThreadQueue():std::queue<T>() {}

template<class T>
SafeThreadQueue<T>::SafeThreadQueue(SafeThreadQueue &q):std::queue<T>(q), queue_mutex_{q.queue_mutex_},
                                                        wait_mutex_{q.wait_mutex_}, wait_cv_{q.wait_cv_},
                                                        count_{q.count_}, is_block_mode_{q.is_block_mode_} {

}


#endif
