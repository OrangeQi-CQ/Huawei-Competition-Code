#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"
#include "state.h"


#include <iostream>

using std::vector;




// bool Startgame::read_until_OK() {
//     char line[1024];
//     while (fgets(line, sizeof line, stdin)) {
//         if (line[0] == 'O' && line[1] == 'K') {
//             return true;
//         }
//     }
//     return false;
// }



void Startgame::read_frame() {
    scanf("%d%d", &current_money, &num_workbench);

    object.init();

    for (int i = 0; i < num_workbench; i++) {
        workbench[i].read(object, i);
    }

    for (int id = 0; id < 4; id++) {
        robot[id].read(id);
    }



    scanf("%s", templine);
    fflush(stdout);
}


/*******************************************************************/
/*******************************************************************/




bool Startgame::set_robot(int id, vector<int> type_buy,
                          vector<int> type_sell) {


    // 如果它已经有任务了，就继续执行它的任务
    if (robot[id].has_mission()) {
        robot[id].perform_mission();
        return true;
    }


    // 两个指针，分别指向前往购买的工作台，前往售卖的工作台
    Workbench *workbench_buy = NULL, *workbench_sell = NULL;


    /**
     * 第一步：寻找离他最近的属于 type_buy 类型的工作台
    */

    double mindis = 1e9;

    for (int i = 0; i < num_workbench; i++) {

        int flg = 0;
        for (int x : type_buy) {
            if (workbench[i].type() == x) {
                flg = 1;
                break;
            }
        }
        if (flg == 0 or !workbench[i].have_product() or workbench[i].isd) {
            continue;
        }

        double nowdis = cal_distance(robot[id].pos(), workbench[i].pos());

        if (nowdis < mindis) {
            workbench_buy = &workbench[i];
            mindis = nowdis;
        }
    }

    if (workbench_buy == NULL) {
        return false;
    }



    /**
     * 第二步：寻找离 workbench_buy 最近的 type_sell 类型的工作台
    */

    mindis = 1e9;

    for (int i = 0; i < num_workbench; i++) {

        bool flg = 0;
        for (int x : type_sell) {
            if (workbench[i].type() == x) {
                flg = 1;
                break;
            }
        }
        if (flg == 0 or workbench[i].find_material(workbench_buy->type()) or workbench[i].isd) {
            continue;
        }

        double nowdis = cal_distance(workbench_buy->pos(), workbench[i].pos());

        if (nowdis < mindis) {
            workbench_sell = &workbench[i];
            mindis = nowdis;
        }
    }

    if (workbench_sell == NULL) {
        return false;
    }


    // 找到了一组买家和卖家，就设定任务
    robot[id].set_mission(workbench_buy, workbench_sell);
    return true;

}


// void Startgame::set_robot(int id, int type_buy, int type_sell) {
//     // 如果它已经有任务了，就继续执行它的任务
//     if (robot[id].has_mission()) {
//         robot[id].perform_mission();
//         return;
//     }


//     // 两个指针，分别指向前往购买的工作台，前往售卖的工作台
//     Workbench *workbench_buy = NULL, *workbench_sell = NULL;


//     // 寻找离他最近的 type_buy 类型的工作台

//     double mindis = 1e9;

//     for (int i = 0; i < num_workbench; i++) {
//         double nowdis = cal_distance(robot[id].pos(), workbench[i].pos());

//         if (workbench[i].type() == type_buy
//             and workbench[i].have_product()
//             and nowdis < mindis) {

//             workbench_buy = &workbench[i];
//             mindis = nowdis;
//         }
//     }

//     if (workbench_buy == NULL) {
//         return;
//     }



//     // 寻找离 workbench_buy 最近的 type_sell 类型的工作台

//     mindis = 1e9;

//     for (int i = 0; i < num_workbench; i++) {
//         double nowdis = cal_distance(workbench_buy->pos(), workbench[i].pos());

//         if (workbench[i].type() == type_sell
//             and workbench[i].find_material(type_buy) == 0
//             and nowdis < mindis) {

//             workbench_sell = &workbench[i];
//             mindis = nowdis;
//         }
//     }

//     if (workbench_sell == NULL) {
//         return;
//     }


//     // 找到了一组买家和卖家，就设定任务
//     robot[id].set_mission(workbench_buy, workbench_sell);

// }






void Startgame::calculate_frame() {


    set_robot(0, {1}, {4, 5, 9});
    set_robot(1, {2}, {4, 6, 9});
    set_robot(2, {3}, {5, 6, 9});


    if (set_robot(3, {7}, {8, 9})) {
        ;
    } else if (set_robot(3, {4, 5, 6}, {7, 9})) {
        ;
    } else if (set_robot(3, {1}, {4, 5, 9})) {
        ;
    } else if (set_robot(3, {2}, {4, 6, 9})) {
        ;
    } else {
        set_robot(3, {3}, {5, 6, 9});
    }



    /**
     * 预防碰撞死锁
    */

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (cal_distance(robot[i].pos(), robot[j].pos()) < 1.2) {

                double cc = (rand() % 50) / 100 + 0.5;
                cc = 1;
                double ii = (cal_dir(robot[j].pos(),
                                     robot[i].pos()) - robot[i].direction);//<0?-1:1;
                while (ii < -PI) {
                    ii += PI * 2;
                }
                while (ii > PI) {
                    ii -= PI * 2;
                }
                Instruct tt = {1, ii * cc};
                robot[i].instruct.push_back(tt);
                tt = {0, 6};
                robot[i].instruct.push_back(tt);
                cc = (rand() % 50) / 100 + 0.5;
                cc = 1;
                ii = (cal_dir(robot[i].pos(),
                              robot[j].pos()) - robot[j].direction);//<0?-1:1;
                while (ii < -PI) {
                    ii += PI * 2;
                }
                while (ii > PI) {
                    ii -= PI * 2;
                }
                tt = {1, ii * cc};
                robot[j].instruct.push_back(tt);
                tt = {0, 6};
                robot[j].instruct.push_back(tt);

            }

        }

    }

}


/*******************************************************************/
/*******************************************************************/


void Startgame::print_frame() {
    printf("%d\n", frameID);

    for (int id = 0; id < 4; id++) {
        robot[id].print_instruct(id);
    }

    puts("OK");
    fflush(stdout);
}



void Startgame::lets_work() {

    // 读入游戏地图
    Map.read_map();


    // 逐帧处理
    int new_frameID;
    while (scanf("%d", &new_frameID) != EOF) {
        if (new_frameID != frameID + 1) {
            // read_until_OK();
            scanf("%d", frameID);
        } else {
            frameID = new_frameID;
        }

        frameID = new_frameID;

        read_frame();

        calculate_frame();

        print_frame();
    }
};

