#pragma once

#include "Workbench.h"
#include "Robot.h"


class Task {
public:
    Workbench *workbench_buy;
    Workbench *workbench_sell;

    std::vector<double> cost_performance;

};



class TaskManager {
public:

    /**
     * 每个物品的买卖价格
    */
    double buy_price[10] = {1e9, 3000, 4400, 5800, 15400, 17200, 19200, 76000};
    double sell_price[10] = {0, 6000, 7600, 9200, 22500, 25000, 27500, 105000};



    /**
     * 储存这一帧当中合法的task
    */
    std::vector<Task> tasks;

    double cal_profit(int material_type);

    /**
     * 每一帧当中初始化所有可行的任务
    */
    void init_tasks(Workbench workbench[], Robot robot[]);


} ;