#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"
#include "state.h"
#include <iostream>



void Robot::read(int ID) {
    RobotID = ID;
    scanf("%d", &workbench_type);
    scanf("%d", &object_type);
    scanf("%lf%lf", &time_value, &collision_value);
    scanf("%lf", &an_speed);
    scanf("%lf%lf", &li_speed.x, &li_speed.y);
    scanf("%lf", &direction);
    scanf("%lf%lf", &position.x, &position.y);
}


void Robot::print_instruct(int ID) {
    for (auto inst : instruct) {
        switch (inst.instruct_type) {
            case 0:
                printf("%s %d %lf\n", "forward", ID, inst.parameter);
                break;
            case 1:
                printf("%s %d %lf\n", "rotate", ID, inst.parameter);
                break;
            case 2:
                printf("%s %d\n", "buy", ID);
                break;
            case 3:
                printf("%s %d\n", "sell", ID);
                break;
            case 4:
                printf("%s %d\n", "destroy", ID);
                break;
        }
    }

    instruct.clear();
}


void Robot::add_mission(Workbench *workbench_buy, Workbench *workbench_sell) {
    // 进行一系列初始化
    workbench_sell->robotlock = ID();

    if (workbench_sell->now_material.size() + workbench_sell->material_is_reserved.count() + 1== workbench_sell->tot_material.size()) {
        tot_material[workbench_sell->type()]++;
    }

    Mission mis = {workbench_buy, 0, workbench_sell, 0};
    missions.push_back(mis);
}



void Robot::perform_mission() {
    if (!has_mission()) {
        return; // 理论上不会发生
    }

    Mission &mission = missions.front();

    // 如果它已经有目的地了
    if (target_pos().x != -1) {

        // 到达买入处，他有商品，就尝试购买
        if (cal_distance(mission.des_buy->pos(), pos()) < 0.4
            and mission.des_buy->have_product()
            and type() == 0) {


            buy(); // 生成指令到 instruct
            mission.flg_buy = 1; // 标志已经发出购买指令
            return;
        }

        // 判断购买成功
        if (mission.flg_buy == 1
            and type() > 0
            and cal_distance(mission.des_buy->pos(), pos()) < 0.4) {

            mission.des_buy->cancel_reserve_product(); // 取消买入地的产品预定
            change_target({-1, -1}); // 修改 target_position
            return;
        }


        // 到达卖出处并且他有空余原料格
        if (!mission.flg_sell
            and cal_distance(mission.des_sell->pos(), pos()) < 0.4
            and mission.des_sell->find_material(mission.des_buy->type()) == 0
            and type() > 0) {


            sell(); // 生成指令到 instruct
            mission.flg_sell = 1; // 修改mission的过程标志
            return;

        }

        // 判断售卖成功
        if (mission.flg_sell == 1
            and type() == 0
            and cal_distance(mission.des_sell->pos(), pos()) < 0.4) {

            // // // 代表 des_sell 类型的产品已经开始生产了
            // tot_material[mission.des_sell->type()]++;

            mission.des_sell->cancel_reserve_material(
                            mission.des_buy->type()); // 取消卖出地的原料预定
            change_target({-1, -1});
            return;
        }

        //继续原计划
        move_to_target();
        return;
    }


    // 否则设定目的地，或结束任务
    if (!mission.flg_buy) {

        // 还没有买，则前往买方
        change_target(mission.des_buy->pos());
        move_to_target();
    } else if (!mission.flg_sell) {

        // 还没有卖，则前往卖方
        change_target(mission.des_sell->pos());
        move_to_target();
    } else {

        // 结束任务
        finish_mission();
    }
}




void Robot::finish_mission() {
    missions.front().des_sell->robotlock = 0;


    missions.pop_front();
    change_target({-1, -1});
}



void Robot::change_target(Point des) {
    target_position = des;
}


void Robot::move_to_target() {
    if (target_pos().x == -1) {
        return;
    }

    /**
     * 第零步：计算当前朝向 direction 与目标方向的偏差角
    */
    double dir_bias = direction - cal_dir(position, target_position);
    double dis = cal_distance(target_position, position);

    if (dir_bias > PI) {
        dir_bias -= 2 * PI;
    } else if (dir_bias < -PI) {
        dir_bias += 2 * PI;
    }


    /**
     * 第一步：决定角速度大小和方向
    */
    int sig = dir_bias > 0 ? 1 : -1;
    if (fabs(dir_bias) >= PI / 20) {
        instruct.push_back({1, -1.0 * sig * PI});
    }
    instruct.push_back({1, -20 * dir_bias});


    /**
     * 第二步：决定线速度大小
    */

    // 距离目标超级近
    if (dis < 1) {
        if (dis < 0.3) {
            if(mapID()==2||mapID()==3)
            instruct.push_back({0, -1});
            else instruct.push_back({0, 1});
            return;
        }

        if (fabs(dir_bias) > PI / 2) {
            instruct.push_back({0, -2});
            return;
        }


        if (fabs(dir_bias) > PI / 6) {
            instruct.push_back({0, 1.5});
            return;
        }

        instruct.push_back({0, 4});
        return;
    }

    // 距离目标比较近
    if (dis < 3) {
        if (fabs(dir_bias) > PI / 2) {
            if(mapID()==1)
            instruct.push_back({0, -0.5});
            else  instruct.push_back({0, -1});
            return;
        }

        if (fabs(dir_bias) > PI / 3) {
            instruct.push_back({0, 2.3});
            return;
        }

        if (fabs(dir_bias) > PI / 6) {
            if (mapID() == 2 or mapID() == 3) {
                instruct.push_back({0, 5});
            } else
            if(mapID()==1)
                instruct.push_back({0, 3.3});
            else instruct.push_back({0, 3});
            return;
        }

        instruct.push_back({0, 6});
        return;
    }


    // 距离目标比较远
    if (dis >= 3) {
        if (fabs(dir_bias) < PI / 2) {
            instruct.push_back({0, 6});
            return;
        }


        if (mapID() == 2 or mapID() == 3) {
            instruct.push_back({0, 3});
        } else
            instruct.push_back({0, 2});
    }
}








void Robot::buy() {
    Mission &mission = missions.front();
    double dis = cal_distance(position,
                              mission.des_buy->pos()) + cal_distance(mission.des_buy->pos(),
                                      mission.des_sell->pos());

    if (dis / 6 + 1 > (9000 - frameID) / 50) {
        target_position = {50 - pos().x, 50 - pos().y};
        move_to_target();
        return;
    }

    instruct.push_back({2});
}

void Robot::sell() {
    instruct.push_back({3});

}

void Robot::destroy() {
    instruct.push_back({4});
}









int Robot::ID() {
    return RobotID;
}

int Robot::type() {
    return object_type;
}

Point Robot::pos() {
    return position;
}

Point Robot::target_pos() {
    return target_position;
}

bool Robot::has_mission() {
    return missions.size() > 0;
}

int Robot::workbench() {
    return workbench_type;
}


double Robot::time_to_free() {
    int siz = missions.size();

    if (siz == 0) {
        return 0;
    }

    auto it = missions.begin();
    double res = cal_distance(position, it->des_buy->pos())
                 + cal_distance(it->des_buy->pos(), it->des_sell->pos());

    auto last = it;
    it++;
    for (; it != missions.end(); it++) {
        res += cal_distance(last->des_sell->pos(), it->des_buy->pos())
               + cal_distance(it->des_buy->pos(), it->des_sell->pos());
        last = it;
    }

    return res / 5 + siz;
}





Point Robot::corner_pos() {
    Point c[9] = {{0.5, 0.5}, {0.5, 49.5}, {49.5, 0.5}, {49.5, 49.5}, {25, 50}, {25, 0}, {0, 25}, {50, 25}};
    int t = 4;
    if (mapID() == 1 or mapID() == 2) {
        t = 8;
    }
    for (int i = 0; i < t; i++) {
        if (cal_distance(position, c[i]) < 5) {
            return c[i];
        }
    }

    return {-1, -1};
}

double Robot::corner_dis() {
    return cal_distance(position, corner_pos());
}

