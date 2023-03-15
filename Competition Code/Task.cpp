#include "Task.h"

#include <iostream>




double TaskManager::cal_profit(int material_type) {
    return sell_price[material_type] - buy_price[material_type];
}



void TaskManager::init_tasks(Workbench workbench[], Robot robot[]) {

    tasks.clear();

    for (int i = 1; i <= num_workbench; i++) {

        // 他生产过程被阻塞，或者等待时间大于 5 秒
        if (!workbench[i].have_product() and
            (workbench[i].waiting_time() < 0
             or workbench[i].waiting_time() > 3)) {
            continue;
        }

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


            // 他已经没有原料格了
            if (workbench[j].find_material(t)) {
                continue;
            }

            // 他已经有这种原料了
            if (workbench[j].find_material(workbench[i].type())) {
                continue;
            }

            // 他的原料格已经被预定了
            if (workbench[j].check_reserved_material(workbench[i].type())) {
                continue;
            }

            // 依次计算这个任务针对每个机器人的性价比
            std::vector<double> cost_performance(4, -1);

            for (int id = 0; id < 4; id++) {
                if (robot[id].has_mission()) {
                    cost_performance[id] = -1;
                    continue;
                }

                double prof = cal_profit(workbench[i].type());

                // 尽可能前往已经有一部分原料的工作台，帮助他更快合成商品
                if (!workbench[j].waiting_time() < 0
                    and workbench[j].now_material.size() + workbench[j].material_is_reserved.count()
                    < workbench[j].tot_material.size()) {
                    prof *= (1 + workbench[j].now_material.size() +
                             workbench[j].material_is_reserved.count());
                }

                double time = cal_distance(workbench[i].pos(), workbench[j].pos()) / 6
                              + std::max(cal_distance(workbench[i].pos(), robot[id].pos()) / 6,
                                         workbench[i].waiting_time()) + 2;

                cost_performance[id] = prof / time;


            }

            tasks.push_back({&workbench[i],
                             &workbench[j],
                             cost_performance});

        }
    }
}



