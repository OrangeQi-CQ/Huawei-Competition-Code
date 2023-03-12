#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"


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




void Robot::move_to_target() {
    // 计算当前朝向 direction 与目标方向的偏差角
    double dir_bias = direction - cal_dir(position, target_position);
    double dis = cal_distance(target_position, position);


    if (dir_bias > PI) {
        dir_bias -= 2 * PI;
    } else if (dir_bias < -PI) {
        dir_bias += 2 * PI;
    }

    int sig = dir_bias > 0 ? 1 : -1;
    if (fabs(dir_bias) >= PI / 6) {
        instruct.push_back({1, -1.0 * sig * PI});
    }
    instruct.push_back({1, -6 * dir_bias});


    if (dis < 1) {
        if (fabs(dir_bias) > PI / 2) {
            instruct.push_back({0, -1});
        } else if (fabs(dir_bias) > PI / 4) {
            instruct.push_back({0, 2});
        } else {
            instruct.push_back({0, 4});
        }
        return;
    }
    

    if (dis >= 1) {
        if (fabs(dir_bias) > PI / 2) {
            instruct.push_back({0, -1});
        } else if (fabs(dir_bias) > PI / 4) {
            instruct.push_back({0, 5});
        } else {
            instruct.push_back({0, 6});
        }

        return;
    }
}




void Robot::finish_mission() {
    if (object()) {
        destroy();
    }

    mission = {NULL, 0, NULL, 0};
    Has_mission = 0;
}



void Robot::perform_mission() {

    // 如果它已经有目标了
    if (target_pos().x != -1) {

        // 到达买方
        if (!mission.flg_buy and mission.des_buy->ID() == workbench()) {

            buy();
            mission.flg_buy = 1;
            change_target({-1, -1}, -1);
            return;
        }

        // 到达卖方
        if (!mission.flg_sell and mission.des_sell->ID() == workbench()) {

            sell();
            mission.flg_sell = 1;
            change_target({-1, -1}, -1);
            return;
        }


        //继续原计划
        move_to_target();
        return;
    }


    // 否则设定目标，或结束任务
    if (!mission.des_buy) {

        // 还没有买，则前往买方
        change_target(mission.des_buy->pos(), 0);
    } else if (!mission.des_sell) {

        // 还没有卖，则前往卖方
        change_target(mission.des_sell->pos(), 1);
    } else {

        // 结束任务
        finish_mission();
    }

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



int Robot::object() {
    return object_type;
}


int Robot::workbench() {
    return workbench_type;
}


void Robot::buy() {
    instruct.push_back({2});
}


void Robot::sell() {
    instruct.push_back({3});
}

void Robot::destroy() {
    instruct.push_back({4});
}


Point Robot::pos() {
    return position;
}

Point Robot::target_pos() {
    return target_position;
}

int Robot::target_be() {
    return target_behavior;
}

void Robot::change_target(Point des, int beh) {
    target_position = des;
    target_behavior = beh;
}


bool Robot::has_mission() {
    return Has_mission;
}