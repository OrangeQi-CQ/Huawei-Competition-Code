#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"


#include <iostream>

int frameID = 0;
int current_money;
int num_workbench;





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

    for (int i = 0; i < num_workbench; i++) {
        workbench[i].read(object, i);
    }

    for (int id = 0; id < 4; id++) {
        robot[id].read(id);
    }

    object.init();

    scanf("%s", templine);
    fflush(stdout);
}


/*******************************************************************/
/*******************************************************************/



void Startgame::set_robot(int id, int type_buy, int type_sell) {
    // 如果它已经有任务了，就继续执行它的任务
    if (robot[id].has_mission()) {
        robot[id].perform_mission();
        return;
    }


    // 两个指针，分别指向前往购买的工作台，前往售卖的工作台
    Workbench *workbench_buy = NULL, *workbench_sell = NULL;


    // 寻找离他最近的 type_buy 类型的工作台

    double mindis = 1e9;

    for (int i = 0; i < num_workbench; i++) {
        double nowdis = cal_distance(robot[id].pos(), workbench[i].pos());

        if (workbench[i].type() == type_buy
            and nowdis < mindis) {

            workbench_buy = &workbench[i];
            mindis = nowdis;
        }
    }

    if (workbench_buy == NULL) {
        return;
    }



    // 寻找离 workbench_buy 最近的 type_sell 类型的工作台

    mindis = 1e9;

    for (int i = 0; i < num_workbench; i++) {
        double nowdis = cal_distance(workbench_buy->pos(), workbench[i].pos());

        if (workbench[i].type() == type_sell
            and workbench[i].find_material(type_buy) == 0
            and nowdis < mindis) {

            workbench_sell = &workbench[i];
            mindis = nowdis;
        }
    }

    if (workbench_sell == NULL) {
        return;
    }


    // 找到了一组买家和卖家，就设定任务
    robot[id].set_mission(workbench_buy, workbench_sell);

}






void Startgame::calculate_frame() {



    // for (int i = 1; i <= 3; i++) {
    //     if (object.material[i].in_seek.size()) {
    //         int x0 = workbench[*(object.material[i].in_seek.begin())].ID();
    //         set_robot(i - 1, i, workbench[x0].type());
    //     }
    // }


    if (frameID % 2 == 0) {
        set_robot(0, 1, 4);
        set_robot(1, 2, 4);
        set_robot(2, 3, 5);
    } else {
        set_robot(0, 1, 5);
        set_robot(1, 2, 6);
        set_robot(2, 3, 6);
    }


    if (object.material[4].on_sale.size()) {
        set_robot(3, 4, 7);
    } else if (object.material[5].on_sale.size()) {
        set_robot(3, 5, 7);
    } else if (object.material[6].on_sale.size()) {
        set_robot(3, 6, 7);
    } else if (object.material[7].on_sale.size()) {
        set_robot(3, 7, 8);
    } else {
        set_robot(3, 1, 5);
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

