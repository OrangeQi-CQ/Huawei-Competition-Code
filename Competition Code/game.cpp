#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"
#include "state.h"


#include <iostream>

using std::vector;




// bool Game::read_until_OK() {
//     char line[1024];
//     while (fgets(line, sizeof line, stdin)) {
//         if (line[0] == 'O' && line[1] == 'K') {
//             return true;
//         }
//     }
//     return false;
// }



void Game::read_frame() {
    scanf("%d%d", &current_money, &num_workbench);

    for (int i = 1; i <= num_workbench; i++) {
        workbench[i].read(i);
    }

    for (int id = 0; id < 4; id++) {
        robot[id].read(id);
    }



    scanf("%s", templine);
    fflush(stdout);
}


/*******************************************************************/
/*******************************************************************/



#if 0
bool Game::set_robot(int id, vector<int> type_buy,
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
        if (flg == 0 or !workbench[i].have_product()) {
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
        if (flg == 0 or workbench[i].find_material(workbench_buy->type())) {
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
#endif



// void Game::avoid_colision() {
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             if (cal_distance(robot[i].pos(), robot[j].pos() > 3)) {
//                 continue;
//             }

//             Direction v1 = robot[i].direction, v2 = robot[j].direction;
//             Direction dv = v1 - v2;
//             if (dv > PI) {
//                 dv -= 2 * PI;
//             }
//             if (dv < -PI) {
//                 dv += 2 * PI;
//             }


//         }
//     }

// }

#if 1
void Game::avoid_colision() {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            float a = robot[i].direction - cal_dir(robot[i].pos(), robot[j].pos());
            float b = cal_dir(robot[j].pos(), robot[i].pos()) - robot[j].direction;
            while (a < -PI) {
                a += PI * 2;
            }
            while (a > PI) {
                a -= PI * 2;
            }
            a = abs(a);
            while (b < -PI) {
                b += PI * 2;
            }
            while (a > PI) {
                b -= PI * 2;
            }
            b = abs(b);
            double av = sqrt(robot[i].li_speed.x * robot[i].li_speed.x + robot[i].li_speed.y
                             * robot[i].li_speed.y);
            double bv = sqrt(robot[j].li_speed.x * robot[j].li_speed.x + robot[j].li_speed.y
                             * robot[j].li_speed.y);
            double ab = cal_distance(robot[i].pos(), robot[j].pos());
            double ac = ab * sin(a) / sin(PI - a - b);
            double bc = ab * sin(b) / sin(PI - a - b);
            //if(ac*bc<0) continue;
            if (abs(ac / av - bc / bv) * (av + bv) < 1.2 || (ab < 2.5
                    && abs(abs(a - b) - PI) < 0.3)) {
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

                ii = PI * ii * (1 / abs(ii)) * 0.99;

                Instruct tt = {1, ii * cc};
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

                ii = PI * ii * (1 / abs(ii)) * 0.99;

                tt = {1, ii * cc};
                robot[j].instruct.push_back(tt);
                //robot[i].instruct.push_back({0,1});
            }
        }
    }
}
#endif





void Game::calculate_frame() {

    taskmanager.init_tasks(workbench);

    for (int i = 0; i < 4; i++) {
        if (robot[i].has_mission()) {
            robot[i].perform_mission();
            continue;
        }

        double dis = 1e9, score = 0;
        Workbench *des_buy = NULL, *des_sell = NULL;

        // std::cout << taskmanager.tasks.size() << std::endl;

        for (Task task : taskmanager.tasks) {

            if (task.workbench_buy->check_reserved_product()
                or task.workbench_sell->check_reserved_material(task.workbench_buy->type())) {
                continue;
            }

            // printf("%d %d\n", task.workbench_buy->ID(), task.workbench_sell ->ID());
            double now_dis = task.dis + cal_distance(task.workbench_buy->pos(),
                             robot[i].pos());
            double now_score = task.profit / (now_dis + 3);

            if (now_score > score) {
                des_buy = task.workbench_buy;
                des_sell = task.workbench_sell;
                score = now_score;
            }
        }

        // printf("robot %d score == %lf\n", i, score);

        if (des_buy != NULL and des_sell != NULL) {
            robot[i].set_mission(des_buy, des_sell);

            des_buy->reserve_product();
            des_sell->reserve_material(des_buy->type());
            robot[i].perform_mission();

            // printf("robot %d: %d %d\n", i, des_buy->ID(), des_sell->ID());
        }
    }

    avoid_colision();
}


/*******************************************************************/
/*******************************************************************/


void Game::print_frame() {
    printf("%d\n", frameID);

    for (int id = 0; id < 4; id++) {
        robot[id].print_instruct(id);
    }

    puts("OK");
    fflush(stdout);
}



void Game::lets_work() {

    // 读入游戏地图
    Map.read_map();



    // // 逐帧处理

    while (scanf("%d", &frameID) != EOF) {

        // printf("%d\n", frameID);

        read_frame();

        calculate_frame();

        print_frame();
    }
};
