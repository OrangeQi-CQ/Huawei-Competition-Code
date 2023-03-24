#include "Task.h"

#include <iostream>
#include <algorithm>




double TaskManager::cal_profit(int material_type) {
    return sell_price[material_type] - buy_price[material_type];
}



// 建图的过程
void TaskManager::init_map(Workbench workbench[]) {
    for (int type_i = 1; type_i <= 3; type_i++) {
        for (int i : wbtypes[type_i]) {
            workbench[i].weight = 1;
        }
    }

    for (int type_i = 1; type_i <= 7; type_i++) {
        for (int i : wbtypes[type_i]) {
            for (int j : wbtypes[9]) {
                workbench[i].target.insert(j);
                workbench[j].source[type_i].insert(i);
            }
        }
    }

    // 计算每个点的重量，以及所有的倾向来源和倾向目的
    for (int type_j = 4; type_j <= 9; type_j++) {
        for (int j : wbtypes[type_j]) {
            workbench[j].weight = 0;

            for (int type_i : reliance[type_j]) {
                if (type_i == 7) {
                    for (int i : wbtypes[7]) {
                        workbench[j].source[type_i].insert(i);
                        workbench[i].target.insert(j);
                    }
                    continue;
                }

                workbench[j].min_weight[type_i] = 1e7;

                for (int i : wbtypes[type_i]) {
                    double weight_i = workbench[i].weight + cal_distance(workbench[i].pos(), workbench[j].pos());
                    weight_i *= (1 + workbench[i].target.size());
                    workbench[j].min_weight[type_i] = std::min(workbench[j].min_weight[type_i], weight_i);
                }

                for (int i : wbtypes[type_i]) {
                    double weight_i = workbench[i].weight + cal_distance(workbench[i].pos(), workbench[j].pos());
                    weight_i *= (1 + workbench[i].target.size());
                    if (weight_i < 2 * workbench[j].min_weight[type_i] + 20) {
                        workbench[j].source[type_i].insert(i);
                        workbench[i].target.insert(j);
                    }
                }

                workbench[j].weight += workbench[j].min_weight[type_i];
            }
        }
    }
}




void TaskManager::init_tasks(Workbench workbench[], Robot robot[]) {
    tasks.clear();
    memset(priority, 0, sizeof priority);
    memset(wb_priority, 0, sizeof wb_priority);
    memset(target, 0, sizeof target);

    /**
     * 计算原料优先级
    */

    material_priority[7] = 100;
    std::fill(material_priority, material_priority + 10, 1);

    if (wbtypes[7].size()) {
        int maxv = 0;
        for (int i = 4; i <= 6; i++) {
            maxv = std::max(maxv, tot_material[i]);
        }
        for (int i = 4; i <= 6; i++) {
            material_priority[i] += 50 * (maxv - tot_material[i]);
        }
    } else {
        for (int i = 4; i <= 6; i++) {
            material_priority[i] = 50;
        }
    }

    /**
     * 计算边和点的优先级。
    */
    for (int i = 1; i <= num_workbench; i++) {
        wb_priority[i] = material_priority[workbench[i].type()];
    }
    for (int type_j = 9; type_j >= 4; type_j--) {
        for (int j : wbtypes[type_j]) {

            for (int type_i : reliance[type_j]) {
                for (int i : workbench[j].source[type_i]) {
                    // 有一方已经被预定
                    if (workbench[i].check_reserved_product() or workbench[j].check_reserved_material(type_i)) {
                        continue;
                    }

                    // 原料格已满，且已经或即将阻塞，或等待时间太久
                    if (workbench[j].find_material(type_i)) {
                        if (workbench[j].lack_material.size() or workbench[j].waiting_time() > 3 or workbench[j].have_product()) {
                            continue;
                        }
                    }

                    // 买的地方没有产品
                    if (!workbench[i].have_product() and (workbench[i].waiting_time() < 0 or workbench[i].waiting_time() > 5)) {
                        continue;
                    }

                    // 设定边权优先级
                    priority[i][j] = wb_priority[j] / 10 + 1;

                    if (type_i >= 4 and type_i <= 6) {
                        priority[i][j] *= 10;
                    }

                    priority[i][j] *= material_priority[type_j];

                    // 偏心式调度
                    if (workbench[j].lack_material.size() > workbench[j].material_is_reserved.count()) {
                        priority[i][j] *= (1 + 5 * (workbench[j].now_material.size() + workbench[j].material_is_reserved.count()));
                    }

                    // 某个工作台的优先级等于从他出发的各条路径优先级的最大值
                    if (priority[i][j] > wb_priority[i]) {
                        wb_priority[i] = priority[i][j];
                    }
                }
            }
        }
    }

    /**
    * 对于每一个机器人，计算每个任务针对每个机器人的性价比
    */
    bool has_workbench7 = ((int)wbtypes[7].size() > 0);

    for (int i = 1; i <= num_workbench; i++) {
        for (int j = 1; j <= num_workbench; j++) {
            if (workbench[i].target.find(j) == workbench[i].target.end()) {
                continue;
            }

            if (priority[i][j] == 0) {
                continue;
            }

            std::vector<double> cost_performance(4, -1);

            for (int id = 0; id < 4; id++) {

                if (robot[id].has_mission() and robot[id].missions.rbegin()->des_sell != &workbench[i]) {
                    continue;
                }

                double prof = cal_profit(workbench[i].type()) * priority[i][j];
                // double prof = cal_profit(workbench[i].type());

                // 只要存在 7 类工作台，就不鼓励送到 9 号工作台
                if (workbench[j].type() == 9) {
                    if (workbench[i].type() != 7 and has_workbench7) {
                        prof *= 0.2;
                    }
                }

                // 预估购买耗时（从这个机器人空闲了开始算）
                double time_buy = 1e7;
                if (robot[id].has_mission()) {
                    time_buy = cal_distance(robot[id].missions.rbegin()->des_sell->pos(), workbench[i].pos()) / 6;
                    time_buy = std::max(time_buy, workbench[i].waiting_time() - robot[id].time_to_free());
                } else {
                    time_buy = cal_distance(robot[id].pos(), workbench[i].pos()) / 6;
                    time_buy = std::max(time_buy, workbench[i].waiting_time());
                }

                // 预估售卖耗时（从这个机器人买到开始算）
                double time_sell = cal_distance(workbench[i].pos(), workbench[j].pos()) / 6;
                if (workbench[j].find_material(workbench[i].type())
                    and workbench[j].waiting_time() - robot[id].time_to_free() - time_sell - time_buy > 0
                    and rest_time > 30) {

                    cost_performance[id] = -1;
                    continue;
                }
                time_sell = std::max(time_sell, workbench[j].waiting_time() - robot[id].time_to_free() - time_buy);

                double time = time_sell + time_buy;

                // 针对从 7 购买的某种情况进行特判
                if (workbench[i].type() == 7 and frameID <= 7500) {
                    if (robot[id].missions.size()
                        and robot[id].missions.rbegin()->des_sell == &workbench[i]) {
                        ;
                    } else {
                        cost_performance[id] = -1;
                        continue;
                    }
                }

                cost_performance[id] = prof / time;

                if (workbench[i].type() == 7 and cost_performance[id] > 0) {
                    cost_performance[id] = 1e7;
                }
            }

            tasks.push_back({&workbench[i], &workbench[j], cost_performance});
        }
    }
}





#if 0
void TaskManager::init_tasks(Workbench workbench[], Robot robot[]) {

    bool has_workbench7 = gamemap->wbtypes[7].size() > 0;

    /**
     * 计算所有物品的优先级
    */
    // std::fill(material_priority + 1, material_priority + 8, 1);
    memset(material_priority, 0, sizeof material_priority);
    // 计算 7 为起点的优先级
    material_priority[7][8] = material_priority[7][9] = 300;

    // 计算 456 为起点的优先级
    int tmplack[4];
    for (int i = 1; i <= num_workbench; i++) {
        if (workbench[i].type() == 7) {
            for (int x : workbench[i].lack_material) {
                tmplack[x - 3]++;
                if (workbench[i].waiting_time() < 0) {
                    tmplack[x - 3] += 2;
                }
            }
        }
    }
    int tmpmin = 100, tmptot = 0;
    for (int i = 1; i <= 3; i++) {
        tmpmin = std::min(tmpmin, tmplack[i]);
    }
    for (int i = 1; i <= 3; i++) {
        tmplack[i] -= tmpmin;
        tmptot += tmplack[i];
    }
    for (int i = 1; i <= 3; i++) {
        material_priority[i + 3][7] += tmplack[i] / (tmptot + 1) * 100 + 20;
    }

    // 计算 123 为起点的优先级
    for (int i = 1; i <= 3; i++) {
        for (int j = 4; j <= 6; j++) {
            material_priority[i][j] = 5;
        }
    }
    if (has_workbench7) {
        int minv = 1e5, sumv = 0;
        for (int i = 4; i <= 6; i++) {
            minv = std::min(minv, gamemap->tot_material[i]);
        }
        for (int i = 4; i <= 6; i++) {
            gamemap->tot_material[i] -= minv;
            sumv += gamemap->tot_material[i];
        }

        sumv++;
        material_priority[1][4] += (sumv - gamemap->tot_material[4]) / sumv * 10;
        material_priority[2][4] += (sumv - gamemap->tot_material[4]) / sumv * 10;
        material_priority[1][5] += (sumv - gamemap->tot_material[5]) / sumv * 10;
        material_priority[3][5] += (sumv - gamemap->tot_material[5]) / sumv * 10;
        material_priority[2][6] += (sumv - gamemap->tot_material[6]) / sumv * 10;
        material_priority[3][6] += (sumv - gamemap->tot_material[6]) / sumv * 10;

    }



    /**
     * 计算所有可能的任务的性价比
    */

    tasks.clear();

    for (int i = 1; i <= num_workbench; i++) {
        // 他生产过程被阻塞，或者等待时间大于 5 秒
        if (!workbench[i].have_product() and
            (workbench[i].waiting_time() < 0 or workbench[i].waiting_time() > 5)) {
            continue;
        }

        // 他的产品已经被预定
        if (workbench[i].check_reserved_product()) {
            continue;
        }

        for (int j = 1; j <= num_workbench; j++) {
            bool flg = 1; // 标志能否组成一组task
            int t = workbench[j].type();
            switch (workbench[i].type()) {
                case 1:
                    if (t != 4 and t != 5 and t != 9) {
                        flg = 0;
                    }
                    break;
                case 2:
                    if (t != 4 and t != 6 and t != 9) {
                        flg = 0;
                    }
                    break;
                case 3:
                    if (t != 5 and t != 6 and t != 9) {
                        flg = 0;
                    }
                    break;
                case 4:
                    ;
                case 5:
                    ;
                case 6:
                    if (t != 7 and t != 9) {
                        flg = 0;
                    }
                    break;
                case 7:
                    if (t != 8 and t != 9) {
                        flg = 0;
                    }
                    break;
                default:
                    flg = 0;
                    break;
            }


            // 产品型号不匹配
            if (flg == 0) {
                continue;
            }


            // 他已经没有原料格了，并且不能继续生产
            if (workbench[j].find_material(workbench[i].type())) {
                if (workbench[j].lack_material.size() or workbench[j].have_product()) {
                    continue;
                }
            }

            // 他的原料格已经被预定了
            if (workbench[j].check_reserved_material(workbench[i].type())) {
                continue;
            }


            /**
             * 对于每一个机器人，计算这个任务针对每个机器人的性价比
            */
            std::vector<double> cost_performance(4, -1);

            for (int id = 0; id < 4; id++) {
                double prof = cal_profit(workbench[i].type()) * material_priority[workbench[i].type()][workbench[j].type()];

                // 只要存在 7 类工作台，就不鼓励送到 9 号工作台
                if (workbench[j].type() == 9) {
                    if (workbench[i].type() != 7 and has_workbench7) {
                        prof *= 0.2;
                    }
                }

                // 预估购买耗时（从这个机器人空闲了开始算）
                double time_buy;
                if (robot[id].has_mission()) {
                    time_buy = cal_distance(robot[id].missions.rbegin()->des_sell->pos(), workbench[i].pos()) / 6;
                    time_buy = std::max(time_buy, workbench[i].waiting_time() - robot[id].time_to_free());
                } else {
                    time_buy = cal_distance(robot[id].pos(), workbench[i].pos()) / 6;
                    time_buy = std::max(time_buy, workbench[i].waiting_time());
                }

                // 预估售卖耗时（从这个机器人买到开始算）
                double time_sell = cal_distance(workbench[i].pos(), workbench[j].pos()) / 6;
                if (workbench[j].find_material(workbench[i].type())
                    and workbench[j].waiting_time() - robot[id].time_to_free() - time_sell - time_buy > 0
                    and rest_time > 30) {

                    prof *= -1;
                }
                time_sell = std::max(time_sell, workbench[j].waiting_time());

                double time = time_sell + time_buy;

                // 针对从 7 购买的某种情况进行特判
                if (workbench[i].type() == 7 and rest_time > 30) {
                    bool flg = 0;

                    if (robot[id].missions.size()
                        and robot[id].missions.rbegin()->des_sell == &workbench[i]) {
                        ;
                    } else {
                        prof *= -1;
                    }
                }
                cost_performance[id] = prof / time;
            }

            tasks.push_back({&workbench[i], &workbench[j], cost_performance});

        }
    }
}

#endif

