#include "Task.h"

#include <iostream>




double TaskManager::cal_profit(int material_type) {
    return sell_price[material_type] - buy_price[material_type];
}



void TaskManager::init_tasks(Workbench workbench[]) {

    // cout << "init taks" << endl;

    tasks.clear();

    for (int i = 1; i <= num_workbench; i++) {
        if (workbench[i].have_product() == 0) {
            continue;
        }

        // printf("workbench %d haveproduct\n", i);

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


            // printf("possible : %d %d", i, j);


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

            // double profit =

            tasks.push_back({&workbench[i],
                             &workbench[j],
                             cal_distance(workbench[i].pos(), workbench[j].pos()),
                             cal_profit(workbench[i].type())});


            // printf("task: %d %d, profit = %lf\n", i, j, cal_profit(workbench[i].type()));

        //     cout << endl;
        //     cout << endl;
        }
    }

    // cout << tasks.size() << endl;

}



