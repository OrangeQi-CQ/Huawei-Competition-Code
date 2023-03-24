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

    if (frameID == 1) {
        for (int i = 1; i <= num_workbench; i++) {
            wbtypes[workbench[i].type()].insert(i);
        }
    }

    scanf("%s", templine);
    fflush(stdout);
}


/*******************************************************************/
/*******************************************************************/


void Game::avoid_wall() {
    for (int i = 0; i < 4; i++) {
        if (robot[i].pos().x < 1 and robot[i].li_speed.x < -2
            or robot[i].pos().x > 49 and robot[i].li_speed.x > 2
            or robot[i].pos().y < 1 and robot[i].li_speed.y < -2
            or robot[i].pos().y > 49 and robot[i].li_speed.y > 2) {
            robot[i].instruct.push_back({0, 1});
        }

    }
}


#if 0
void Game::avoid_colision() {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            float a=robot[i].direction-cal_dir(robot[i].pos(),robot[j].pos());
            float b=cal_dir(robot[j].pos(),robot[i].pos())-robot[j].direction;
            while (a < -PI) {
                a += PI * 2;
            }
            while (a > PI) {
                a -= PI * 2;
            }
            while (b < -PI) {
                b += PI * 2;
            }
            while (a > PI) {
                b -= PI * 2;
            }
            bool f=false;
            double ab=cal_distance(robot[i].pos(),robot[j].pos());
            if(abs(a+b)<PI && ab<8){
            a=abs(a);
            b=abs(b);
            double c=PI-a-b;
            double av=sqrt(robot[i].li_speed.x*robot[i].li_speed.x+robot[i].li_speed.y*robot[i].li_speed.y);
            double bv=sqrt(robot[j].li_speed.x*robot[j].li_speed.x+robot[j].li_speed.y*robot[j].li_speed.y);
            double ac=ab*sin(a)/sin(PI-a-b);
            double bc=ab*sin(b)/sin(PI-a-b);
            double t= std::min(ac/av,bc/bv);
            for(int k=1;k<1000;k++){
                ab=sqrt(ac*ac+bc*bc-2*ac*bc*cos(c));
                if(ab<1.1){
                    f=true;
                    break;
                }
                ac-=av*t/1000;
                bc-=bv*t/1000;
            }
            }
            //if(ac*bc<0) continue;
            if(f||ab<1.1)//||( ab<2.5 && abs(abs(robot[i].direction-robot[j].direction)-PI)<0.3))
            {
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

                ii=PI*ii*(1/abs(ii))*0.99;

                //if( ab<2.5 && abs(abs(robot[i].direction-robot[j].direction)-PI)<0.3) ii=PI;

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

                ii=PI*ii*(1/abs(ii))*0.99;

                //if( ab<2.5 && abs(abs(robot[i].direction-robot[j].direction)-PI)<0.3) ii=PI;

                tt = {1, ii * cc};
                robot[j].instruct.push_back(tt);
                //robot[i].instruct.push_back({0,1});
            }

        }
    }
}
#endif



void Game::avoid_colision() {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {

            // 如果 ij 的距离很大，就忽略碰撞风险
            double dis = cal_distance(robot[i].pos(), robot[j].pos());
            if (dis > 8) {
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
            while (b < -PI) {
                b += PI * 2;
            }
            while (b > PI) {
                b -= PI * 2;
            }

            // 有迎头相撞的风险
            if ((dis > 5 and fabs(a) < PI / 4 and fabs(b) < PI / 4)
                or (dis < 5 and fabs(a) < PI * 0.5 and fabs(b) < PI * 0.5)) {

                double ti = PI, tj = PI;
                if (a < 0) {
                    ti *= -1;
                }
                if (b > 0) {
                    tj *= -1;
                }

                // 根据距离决定转向幅度
                if (dis < 3) {
                    ;
                } else {
                    ti *= 0.5;
                    tj *= 0.5;
                }
                // 调整转向的优先级
                if (robot[i].type()) {
                    robot[j].instruct.push_back({1, tj});
                } else if (robot[j].type()) {
                    robot[i].instruct.push_back({1, ti});
                } else {
                    double d1 = cal_distance(robot[i].pos(), robot[i].target_pos());
                    double d2 = cal_distance(robot[j].pos(), robot[j].target_pos());
                    if (d1 < d2) {
                        robot[j].instruct.push_back({1, tj});
                    } else {
                        robot[i].instruct.push_back({1, ti});
                    }
                }
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

                Instruct tt = {1, ii *cc};


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

                tt = {1, ii *cc};
                robot[j].instruct.push_back(tt);
                //robot[i].instruct.push_back({0,1});
            }
        }
    }
}





void Game::calculate_frame() {

    taskmanager.init_tasks(workbench, robot);

    // 第一轮分配，只考虑手里有任务的机器人，尝试给他续上
    for (int i = 0; i < 4; i++) {
        if (!robot[i].has_mission()) {
            continue;
        }

        double max_score = 0;
        Workbench *des_buy = NULL, *des_sell = NULL;

        for (Task task : taskmanager.tasks) {
            if (task.workbench_buy != robot[i].missions.rbegin()->des_sell) {
                continue;
            }

            // 任务的原料或产品其一被预定了，直接跳过
            if (task.workbench_buy->check_reserved_product()
                or task.workbench_sell->check_reserved_material(task.workbench_buy->type())) {
                continue;
            }

            double now_score = task.value[i];

            // 挑选最优任务
            if (now_score > max_score) {
                des_buy = task.workbench_buy;
                des_sell = task.workbench_sell;
                max_score = now_score;
            }
        }

        if (des_buy != NULL and des_sell != NULL) {
            des_buy->reserve_product();
            des_sell->reserve_material(des_buy->type());
            robot[i].add_mission(des_buy, des_sell);
        }


        if (robot[i].has_mission()) {
            robot[i].perform_mission();
        }
    }

    // 第二轮分配，只考虑没有任务的机器人
    for (int i = 0; i < 4; i++) {
        if (robot[i].has_mission()) {
            continue;
        }

        double max_score = 0;
        Workbench *des_buy = NULL, *des_sell = NULL;

        for (Task task : taskmanager.tasks) {

            // 任务的原料或产品其一被预定了，直接跳过
            if (task.workbench_buy->check_reserved_product()
                or task.workbench_sell->check_reserved_material(task.workbench_buy->type())) {
                continue;
            }

            double now_score = task.value[i];

            // 挑选最优任务
            if (now_score > max_score) {
                des_buy = task.workbench_buy;
                des_sell = task.workbench_sell;
                max_score = now_score;
            }
        }

        if (des_buy != NULL and des_sell != NULL) {
            des_buy->reserve_product();
            des_sell->reserve_material(des_buy->type());
            robot[i].add_mission(des_buy, des_sell);

        }

        if (robot[i].has_mission()) {
            robot[i].perform_mission();
        }
    }

    avoid_colision();
    avoid_wall();

    // 防止在墙角堵死，规定同一个墙角只能进去一个机器人
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (robot[i].corner_pos().x != 25 and robot[i].corner_pos() == robot[j].corner_pos()){
                if (robot[i].corner_dis() < robot[j].corner_dis()) {
                    robot[j].instruct.push_back({0, 0});
                } else {
                    robot[i].instruct.push_back({0, 0});
                }
            }
        }
    }
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
    for (int i = 1; i <= 100; i++) {
        scanf("%s", gamemap[i] + 1);

        for (int j = 1; j <= 100; j++) {
            if (gamemap[i][j] >= '1' and gamemap[i][j] <= '9') {
                int type = gamemap[i][j] - '0';
                workbench[++num_workbench].position = {0.25 + 0.5 * (j - 1), 49.75 - 0.5 * (i - 1)};
                workbench[num_workbench].workbench_type = type;
                wbtypes[type].insert(num_workbench);
            }
        }
    }

    reliance[4] = {1, 2};
    reliance[5] = {1, 3};
    reliance[6] = {2, 3};
    reliance[7] = {4, 5, 6};
    reliance[8] = {7};

    if (wbtypes[7].size()) {
        reliance[9] = {7};
    } else if (reliance[4].size() + reliance[5].size() + reliance[6].size()) {
        reliance[9] = {4, 5, 6};
    } else {
        reliance[9] = {1, 2, 3};
    }

    // taskmanager 建图
    taskmanager.init_map(workbench);

    scanf("%s", templine);
    puts("OK");
    fflush(stdout);

    // 逐帧处理
    while (scanf("%d", &frameID) != EOF) {
        rest_time = 1.0 * (9000 - frameID) / 50;
        read_frame();
        calculate_frame();
        print_frame();
    }
};
