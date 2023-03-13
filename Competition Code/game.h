#pragma once

#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "Point.h"
#include "state.h"
#include "Task.h"





class Game {
public:

    /**
     * 参数：机器人ID，希望他在 from 类工作台购买，去 to 类工作台售卖
     *       如果他正在路上，就维持他原本的计划
     * 作用：设定机器人的调度
    */
    bool set_robot(int id, std::vector<int> _buy, std::vector<int> _sell);

    /**
     * 避免碰撞
    */
    void avoid_colision();

    // 跳过不完整的帧
    // bool read_until_OK();

    // 读入当前帧的各对象参数
    void read_frame();

    // 进行当前帧的决策和运算
    void calculate_frame();

    // 输出当前帧的指令
    void print_frame();

    // main 函数的调用接口，开始整个程序
    void lets_work();

private:

    /**
     * 用来读取时的一些问题
    */

   Workbench workbench[55];
   Robot robot[4];
   GameMap Map;
   TaskManager taskmanager;
   char templine[1024];

};
