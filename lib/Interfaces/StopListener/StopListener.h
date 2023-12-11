#ifndef TEST_TASK_STOPLISTENER_H
#define TEST_TASK_STOPLISTENER_H

class StopListener{
public:
    StopListener() = default;
    StopListener(StopListener&) = default;
    virtual auto stopping() -> void = 0;
};

#endif //TEST_TASK_STOPLISTENER_H
