#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "game.h"
#include "geometry.h"
#include "state.h"

#include <iostream>


void Robot::ReadFrame(int id) {
    id_ = id;
    std::cin >> workbench_id_ >> object_type_ 
            >> time_value_ >> collision_value_ >> angle_speed_ >> linear_speed_ 
            >> direction_ >> position_;
}


void Robot::PrintInstruct(int id) {
    for (auto instruct : instructs_) {
        std::cout << instruct.first << " " << id_ << " ";
        if (instruct.first == std::string("forward") || instruct.first == std::string("rotate")) {
            std::cout << instruct.second;
        }
        std::cout << "\n";
    }
    instructs_.clear();
}


void Robot::AddMision(Workbench *buy_ptr, Workbench *sell_ptr) {
    sell_ptr->robotlock = id_;

    if (sell_ptr->now_material.size() + sell_ptr->material_is_reserved.count() + 1
            == sell_ptr->tot_material.size()) {
        tot_material[sell_ptr->type()]++;
    }
    missions_.emplace_back(buy_ptr, false, sell_ptr, false);
}


void Robot::CalcInstruct() {
    assert(HasMission());
    auto [buy_ptr, sell_ptr] = missions_.front();

    if (buy_ptr->GetId() == workbench_id_ && buy_ptr->GetType() == object_type_) {
        Buy();
        SetTargetPosition(sell_ptr->GetPosition());
        return;
    }

    if (sell_ptr->GetId() == workbench_id_ && sell_ptr->GetType() == object_type_) {
        Sell();
        SetTargetPosition(std::nullopt);
        return;
    }

    if (!target_position_.has_value()) {
        FinishMission();
        return;
    }
}


void Robot::FinishMission() {
    assert(!target_position_.has_value());
    missions_.front().second->robotlock = 0;
    missions_.pop_front();
}


void Robot::SetTargetPosition(std::optional<Point> target) {
    target_position_ = target;
}

void Robot::CalcMovingInstruct() {
    /**
     * 第零步：计算当前朝向 direction 与目标方向的偏差角
    */
    double dir_bias = direction_ - cal_dir(position_, target_position_);
    double dis = cal_distance(target_position_, position_);

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
        instructs_.push_back({1, -1.0 * sig * PI});
    }
    instructs_.push_back({1, -20 * dir_bias});


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


void Robot::Buy() {
    Mission &mission = missions_.front();
    auto [buy_ptr, sell_ptr] = missions_.front();
    double dis = cal_distance(position_, buy_ptr->pos()) 
                + cal_distance(buy_ptr->pos(), sell_ptr->pos());
    
    double time_need = dis / 6 + 1;
    double time_rest = (9000 - frameID) / 50;

    if (time_need > time_rest) {
        target_position_ = position_;
    }

    if (dis / 6 + 1 > (9000 - frameID) / 50) {
        target_position = {50 - pos().x, 50 - pos().y};
        move_to_target();
        return;
    }

    instructs_.push_back({2});
}

void Robot::Sell() {
    instructs_.push_back({3});

}

void Robot::Destroy() {
    instructs_.push_back({4});
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
    static Point c[9] = {{0.5, 0.5}, {0.5, 49.5}, {49.5, 0.5}, {49.5, 49.5}, {25, 50}, {25, 0}, {0, 25}, {50, 25}};
    int t = 4;
    if (mapID() == 1 or mapID() == 2) {
        t = 8;
    }
    for (int i = 0; i < t; i++) {
        if (cal_distance(position_, c[i]) < 5) {
            return c[i];
        }
    }

    return {-1, -1};
}

double Robot::corner_dis() {
    return cal_distance(position_, corner_pos());
}

