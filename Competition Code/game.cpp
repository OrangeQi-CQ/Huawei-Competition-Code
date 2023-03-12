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
        workbench[i].read(object);
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




void Startgame::set_robot(int id, int from, int to) {


    // 它所处的工作台类型
    int pos_type = workbench[robot[id].workbench()].type();
    auto &near_workbench = workbench[robot[id].workbench()];

    if (workbench[robot[id].workbench()].pos() == robot[id].target_pos()
        and near_workbench.find_material(robot[id].object()) == 1) {

        robot[id].destroy();
        robot[id].change_target({-1, -1}, -1);
        return;
    }

    // 如果它已经有目标了
    if (robot[id].target_pos().x != -1) {

        //可以卖
        if (robot[id].workbench() >= 0
            and cal_distance(robot[id].pos(), robot[id].target_pos()) < 0.3) {

            robot[id].sell();
            robot[id].change_target({-1, -1}, -1);
        }

        // 可以买
        else if (robot[id].object() == 0
                 and pos_type == from) {

            robot[id].buy();
            robot[id].change_target({-1, -1}, -1);
        }

        //继续原计划
        else {
            robot[id].move_to_target();
        }
    }

    // 完成任务了，需要设定新目标
    else {

        // 找最近的可以买的工作台
        if (robot[id].object() == 0) {
            int mindis = 1e9;
            for (int i = 0; i < num_workbench; i++) {
                if (workbench[i].type() == from
                    and cal_distance(robot[id].pos(), workbench[i].pos()) < mindis) {

                    robot[id].change_target({workbench[i].pos()}, 0);
                    mindis = cal_distance(robot[id].pos(), workbench[i].pos());
                }
            }
        }

        // 找最近的可以卖的工作台
        else {
            int mindis = 1e9;
            for (int i = 0; i < num_workbench; i++) {
                if (workbench[i].type() == to
                    and workbench[i].find_material(from) == 0
                    and cal_distance(robot[id].pos(), workbench[i].pos()) < mindis) {

                    robot[id].change_target({workbench[i].pos()}, 1);
                    mindis = cal_distance(robot[id].pos(), workbench[i].pos());
                }
            }

        }
    }
}


void Startgame::calculate_frame() {


    // for (int i = 1; i <= 3; i++) {
    //     if (object.material[i].in_seek.size()) {
    //         int x0 = workbench[*(object.material[i].in_seek.begin())].type();
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

