#include "../include/TaskService.hpp"

void TaskService::hit_detected() {
    hit_detected(HP_MAX_DEFAULT, STAPWATCH_DEFAULT_SS);
}

void TaskService::hit_detected(uint16_t max) {
    hit_detected(max, STAPWATCH_DEFAULT_SS);
}

void TaskService::hit_detected(uint16_t max, uint16_t ss)
{
    if (_is_await)
    {
        _is_await = false;
        MS_aim = ss * 1000;
        HP_curr = HP_max = max;
        MS_old = MS_curr = millis();
    }
    else
        HP_curr--;
}

bool TaskService::aim_task_handler()
{
    if (_is_await)
        return false;

    MS_left = MS_aim - ((MS_curr = millis()) - MS_old);

    if (MS_left < 0)
        return !(_is_await = true);
    else
    if (HP_curr <= 0)
        return _is_await = true;

    return false;
}

bool TaskService::is_await() {
    return _is_await;
}

int32_t TaskService::time_left_ms() {
    return MS_left;
}

int32_t TaskService::aim_curr_hp() {
    return HP_curr;
}

int32_t TaskService::aim_max_hp() {
    return HP_max;
}
