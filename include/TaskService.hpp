#include <Arduino.h>

#ifndef TaskService_hpp
#define TaskService_hpp

using namespace std;

#define HP_MAX_DEFAULT          30
#define STAPWATCH_DEFAULT_SS    20   

class TaskService
{
private:
    bool _is_await = true;
    int32_t HP_max = 0;
    int32_t HP_curr = 0;
    int32_t MS_aim = 0;
    int32_t MS_left = 0;
    int32_t MS_old = 0;
    int32_t MS_curr = 0;

public:
    void hit_detected();
    void hit_detected(uint16_t max);
    void hit_detected(uint16_t max, uint16_t ss);
    bool aim_task_handler();
    bool is_await();
    int32_t time_left_ms();
    int32_t aim_curr_hp();
    int32_t aim_max_hp();
};

#endif