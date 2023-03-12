#pragma once


#include "Robot.h"
#include "Map.h"
#include "Object.h"
#include "Point.h"
#include <set>

class Workbench {
public:


    /**
     * 参数：当前帧的 ID
     * 作用：读取某一个工作帧的状态
    */
    void read(Object &object, int id);

    /**
     * 功能：返回这个工作台的类型
    */
    int type();


    /**
     * 功能：返回这个工作台的位置
    */
    Point pos();

    /**
     * 功能：查询这个工作台是否拥有原料 x
    */
    bool find_material(int x);

    /**
     * 功能：查询这个工作台是否已经生产处商品
    */
    bool have_product();

    /**
     * 功能：查询这个工作台的编号
    */
    int ID();


private:
    /**
     * 变量含义：
     * 工作台类型，范围 [1, 9]
     * 同时也是商品的类型，[1,7] 工作台生产该编号的商品，[8,9]不输出
    */
    int workbench_type;


    // 横纵坐标
    Point position;


    /**
     * 变量含义：
     * 距离生产结束还有多少帧；
     * 若为 0，则产品输出格满而阻塞
     * 若为 -1，则是由于缺乏原料还有多久
    */
    int remaining_prodution_frame;

    /**
     * 是否已经生产出来商品了
     * 有则为 1，停止继续生产
    */  
    int Have_product;

    // 所处的工作台 ID
    int workbenchID;

    // 这个工作台生产商品需要的原料
    std::set<int> req_material;

    // 这个工作台已经拥有的原料
    std::set<int> now_material;

};




