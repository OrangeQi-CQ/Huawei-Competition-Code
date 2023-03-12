#pragma once

#include "Point.h"
#include "Workbench.h"
#include <vector>




/**
 * 结构体：机器人发出的指令
*/
struct Instruct {
    /**
     * 指令的编号，从 0~4依次对应：
     * forward, rotate, buy, sell, destroy
    */
    int instruct_type;


    /**
     * 指令的参数2 (PS. 参数 1 是机器人编号)
    */
    double parameter = 0;
};




/**
 * 结构体：机器人正在执行的任务
*/
struct Mission {
    Workbench *des_buy;
    bool flg_buy = 0;
    Workbench *des_sell;
    bool flg_sell = 0;
};




class Robot {
public:

    /**
     * 功能：在当前帧中读入该机器人的参数
    */
    void read(int ID);


    /**
     * 参数：1. 一对横纵坐标，代表该机器人的目的地
     *      2. 一个整数表示目的，0 为买，1 为卖
     * 功能：结合当前的位置和状态参数，计算当前帧的指令，保存到 instruct 中
    */
    void move_to_target();



    /**
     * 功能：执行任务
    */
    void perform_mission();


    /**
     * 功能：输出这个机器人的指令，并清空指令集
    */
    void print_instruct(int ID);


    /**
     * 功能：返回这个机器人所携带的物品编号；
     * 0：没有物品
     * 1-7：对应 7 种物品
    */
    int object();


    /**
     * 功能：返回这个机器人所在的工作台编号；
     * -1：没有处在任何工作台附近
     * 0 ~ num_workbench - 1：所在工作台编号
    */
    int workbench();


    /**
     * 功能：输出卖出的指令到 instruct 中
    */
    void buy();

    /**
     * 功能：输出购买的指令到 instruct 中
    */
    void sell();


    /**
     * 功能：输出销毁的指令到 instruct 中
    */
    void destroy();

    /**
     * 功能：返回机器人的位置
    */
    Point pos();


    /**
     * 功能：返回这个机器人的目标位置
    */
    Point target_pos();


    /**
     * 功能：返回这个机器人的目标动作
    */
    int target_be();

    /**
     * 功能：修改这个机器人的目标位置、目标动作
    */
    void change_target(Point, int);


    /**
     * 功能：返回当前机器人是否在任务中
    */
    bool has_mission();


    /**
     * 功能：结束任务；
    */
    void finish_mission();


    



private:

    // 当前机器人编号，也就是 robot 的数组下标
    int RobotID;

    // 手头物品的编号，没有物品为 0
    int object_type;

    // 所在工作台编号，如果不再工作台为 -1
    int workbench_type = -1;

    // 时间价值系数
    double time_value;

    // 碰撞价值系数
    double collision_value;

    // 半径
    double radius;

    // 重量
    double weight;

    // 方向（0 为朝右，pi/2 为朝上，-pi/2 为朝下）
    double direction;

    // 角速度（正为逆时针，负为顺时针）
    double an_speed;

    // 横、纵坐标线速度
    Speed li_speed;

    // 横、纵位置坐标
    Point position;

    // 当前机器人即将发出的指令集
    std::vector<Instruct> instruct;

    // 记录这个机器人想要前往的工作台编号
    Point target_position = {-1, -1};

    // 记录这个机器人想要去目的地做的事情（买为 0，卖为1）
    int target_behavior = -1;

    // 这个机器人是否在任务中
    bool Has_mission = 0;

    // 这个机器人正在执行的任务
    Mission mission;

};
