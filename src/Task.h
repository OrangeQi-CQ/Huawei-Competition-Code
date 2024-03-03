#pragma once

#include "Workbench.h"
#include "Robot.h"


class Task {
public:
    Workbench *workbench_buy;
    Workbench *workbench_sell;
    std::vector<double> value;
};



class TaskManager {
public:

    // GameMap *gamemap;

    /**
     * 每个物品的买卖价格
    */
    double buy_price[10] = {1e9, 3.000, 4.400, 5.800, 15.400, 17.200, 19.200, 76.000};
    double sell_price[10] = {0, 6.000, 7.600, 9.200, 22.500, 25.000, 27.500, 105.000};

    double material_priority[15];

    // 买卖关系的优先级
    double priority[55][55];

    // 工作台的优先级
    double wb_priority[55];

    // 每个工作台准备发往的工作台
    int target[55];

    /**
     * 储存这一帧当中合法的task
    */
    std::vector<Task> tasks;

    double cal_profit(int material_type);

    /**
     * 建立图论模型
    */
    void init_map(Workbench workbench[]);

    /**
     * 每一帧当中初始化所有可行的任务
    */
    void init_tasks(Workbench workbench[], Robot robot[]);


} ;