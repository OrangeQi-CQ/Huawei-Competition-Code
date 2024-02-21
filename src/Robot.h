#pragma once

#include "geometry.h"
#include "Workbench.h"
#include "state.h"

#include <bits/stdc++.h>
#include <vector>
#include <list>



class Robot {
public:
    using Instruct = std::pair<std::string, double>;
    using Mission = std::pair<Workbench *, Workbench *>;

    /**
     * @brief 每个机器人在每一帧 开头 都需要调用这个函数，读入机器人当前的状态参数。
    */
    void ReadFrame(int id);

    /**
     * @brief 每个机器人在每一帧 结束 都需要调用这个函数，输出机器人当前帧的控制指令
    */
    void PrintInstruct(int id);

    /**
     * @brief 指定买家和卖家，为机器人添加一组任务
    */
    void AddMision(Workbench *buy_ptr, Workbench *sell_ptr);

    /**
     * @brief 计算当前帧的指令
     * 1. 根据 mission 中的信息，维护： target_position
     * 2. 根据当前状态判断是否该 buy() 或 sell() 或 destroy()
    */
    void CalcInstruct();

    /**
     * @brief 结束任务
    */
    void FinishMission();

private:
    /**
     * @brief 设定目的地
    */
    void SetTargetPosition(std::optional<Point> target);

    /**
     * @brief 计算移动类指令
    */
    void CalcMovingInstruct();

    /**
     * @brief 生成购买的指令
    */
    void Buy();

    /**
     * @brief 生成出售的指令
    */
    void Sell();

    /**
     * @brief 生成销毁的指令
    */
    void Destroy();

public:
    int GetId() { return id_; }

    /**
     * @brief 查询携带物品的编号。
     * @return 0 没有物品，1-7 对应 7 种物品
    */
    int GetObjectType() { return object_type_; }

    Point GetPosition() { return position_; }

    std::optional<Point> GetTargetPos() { return target_position_; }

    bool HasMission() { return !missions_.empty(); }

    /**
     * @brief 返回这个机器人所在的工作台编号
     * @return -1：没有处在任何工作台附近；否则返回所在工作台编号
    */
    int GetWorkbenchId() { return workbench_id_; }

    double GetTimeFree();

    /**
     * 功能：返回这个机器人所处在的墙角编号
    */
    Point GetCornerId();

    Point GetCornerDis();


private:
    std::vector<Instruct> instructs_; // 这一帧准备输出的指令集
    std::list<Mission> missions_; // 这个机器人已经有的任务集合

    int id_;    // 机器人的编号
    int workbench_id_ = -1; // 所在工作台编号，如果周围没有工作台则为 -1
    int object_type_; // 手头物品的编号，没有物品为 0
    double time_value_;    // 时间价值系数（似乎没卵用）
    double collision_value_; // 碰撞价值系数（似乎没卵用）
    double angle_speed_; // 角速度（正为逆时针，负为顺时针）
    Speed linear_speed_; // 横、纵坐标线速度
    double direction_; // 方向（0 为朝右，pi/2 为朝上，-pi/2 为朝下）
    Point position_; // 横、纵位置坐标

    double radius_;  // 机器人半径
    double weight_; // 机器人重量
    std::optional<Point> target_position_ = std::nullopt;  // 这个机器人正在前往的工作台编号
};
