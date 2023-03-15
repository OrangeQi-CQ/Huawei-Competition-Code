#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"
#include "state.h"


#include <iostream>

using std::vector;






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




void Game::avoid_colision() {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {

            // 如果 ij 的距离很大，就忽略碰撞风险
            if (cal_distance(robot[i].pos(), robot[j].pos()) > 5) {
                continue;
            }

            // robot[i] 的速度与 ij 连线的夹角
            angle a = robot[i].direction - cal_dir(robot[i].pos(), robot[j].pos());

            // robot[j] 的速度与 ji 连线的夹角
            angle b = cal_dir(robot[j].pos(), robot[i].pos()) - robot[j].direction;


            while (a < -PI) {
                a += PI * 2;
            }
            while (a > PI) {
                a -= PI * 2;
            }
            a = fabs(a);

            while (b < -PI) {
                b += PI * 2;
            }
            while (b > PI) {
                b -= PI * 2;
            }
            b = fabs(b);


            // 有迎头相撞的风险
            if (fabs(a) < PI / 6 and fabs(b) < PI / 6) {
                if (a > 0) {
                    robot[i].instruct.push_back({1, PI * 0.6});
                    robot[j].instruct.push_back({1, PI * 0.6});
                } else {
                    robot[i].instruct.push_back({1, -PI * 0.6});
                    robot[j].instruct.push_back({1, -PI * 0.6});
                }
                // robot[i].instruct.push_back({0, -1});
                // robot[j].instruct.push_back({0, -1});
                return;
            }



            // robot[i] 的速度大小
            double av = sqrt(robot[i].li_speed.x * robot[i].li_speed.x
                             + robot[i].li_speed.y * robot[i].li_speed.y);
            // robot[j] 的速度大小
            double bv = sqrt(robot[j].li_speed.x * robot[j].li_speed.x
                             + robot[j].li_speed.y * robot[j].li_speed.y);

            double ab = cal_distance(robot[i].pos(), robot[j].pos());
            double ac = ab * sin(a) / sin(PI - a - b);
            double bc = ab * sin(b) / sin(PI - a - b);
            //if(ac*bc<0) continue;

            // 判断可能发生碰撞
            if (abs(ac / av - bc / bv) * (av + bv) < 1.2
                || (ab < 2.5 && abs(abs(a - b) - PI) < 0.3)) {

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

                Instruct tt = {1, ii *cc * 3 / 2};
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

                tt = {1, ii *cc * 3 / 2};
                // robot[j].instruct.push_back(tt);
                //robot[i].instruct.push_back({0,1});
            }
        }
    }
}






void Game::calculate_frame() {

    taskmanager.init_tasks(workbench, robot);

    for (int i = 0; i < 4; i++) {
        if (robot[i].has_mission()) {
            robot[i].perform_mission();
            continue;
        }

        double max_score = 0;
        Workbench *des_buy = NULL, *des_sell = NULL;

        for (Task task : taskmanager.tasks) {

            if (task.workbench_buy->check_reserved_product()
                or task.workbench_sell->check_reserved_material(task.workbench_buy->type())) {
                continue;
            }

            double now_score = task.cost_performance[i];

            if (now_score > max_score) {
                des_buy = task.workbench_buy;
                des_sell = task.workbench_sell;
                max_score = now_score;
            }
        }

        if (des_buy != NULL and des_sell != NULL) {
            des_buy->reserve_product();
            des_sell->reserve_material(des_buy->type());
            robot[i].set_mission(des_buy, des_sell);
            // robot[i].perform_mission();
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

        read_frame();

        calculate_frame();

        print_frame();
    }
};
