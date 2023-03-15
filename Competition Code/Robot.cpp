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





void Robot::set_mission(Workbench *workbench_buy, Workbench *workbench_sell) {
    if (Has_mission == 1) { // 如果本来就有一个任务，就继续执行
        // finish_mission();
        perform_mission();
    }

    // 进行一系列初始化
    Has_mission = 1;
    mission = {workbench_buy, 0, workbench_sell, 0};
    workbench_buy->reserve_product();
    workbench_sell->reserve_material(workbench_buy->type());
}



void Robot::perform_mission() {

    if (has_mission() == 0) {
        return; // 理论上不会发生
    }

    int t = mission.des_buy->type();


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

            mission.des_sell->cancel_reserve_material(t); // 取消卖出地的原料预定
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
    if (type()) {
        // 理论上不会发生
        // destroy();
    }

    // 一系列清空。
    // 特别注意产品预定标志在结束购买或出售的时候就已经修改过了
    mission.des_buy->cancel_reserve_product();
    mission.des_sell->cancel_reserve_material(mission.des_buy->type());
    mission = {NULL, 0, NULL, 0};
    change_target({-1, -1});
    Has_mission = 0;
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
            instruct.push_back({0, 1});
            return;
        }

        if (fabs(dir_bias) > PI / 2) {
            instruct.push_back({0, -2});
            return;
        }


        if (fabs(dir_bias) > PI / 6) {
            instruct.push_back({0, 1});
            return;
        }

        instruct.push_back({0, 4});
        return;
    }

    // // 距离目标比较近
    if (dis < 4) {
        if (fabs(dir_bias) > PI / 2) {
            instruct.push_back({0, -1});
            return;
        }

        if (fabs(dir_bias) > PI * 3) {
            instruct.push_back({0, 2});
            return;
        }

        if (fabs(dir_bias) > PI / 6) {
            instruct.push_back({0, 3});
            return;
        }

        instruct.push_back({0, 6});
        return;
    }


    // 距离目标比较远
    if (dis >= 4) {
        if (fabs(dir_bias) < PI / 2) {
            instruct.push_back({0, 6});
            return;
        }

    }
}








void Robot::buy() {
    double dis = cal_distance(position,
                              mission.des_buy->pos()) + cal_distance(mission.des_buy->pos(),
                                      mission.des_sell->pos());

    if (dis / 4 + 2 > (9000 - frameID) / 50) {
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
    return Has_mission;
}

int Robot::workbench() {
    return workbench_type;
}

