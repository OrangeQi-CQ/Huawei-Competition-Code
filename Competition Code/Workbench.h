#pragma once


#include "Map.h"
#include "Point.h"
#include "state.h"

#include <set>
#include <bitset>


class Workbench {
public:

    friend class TaskManager;
    friend class Game;

    /**
     * 参数：当前帧的 ID
     * 作用：读取某一个工作帧的状态
    */
    void read(int id);

    /*************************************************************/
    // 关于预定的一组函数

    /**
     * 功能：查询这个原料格是否没有预定，被预定返回1
    */
    bool check_reserved_material(int type_material);

    /**
     * 功能：预定类型为 type_material 的原料格
    */
    void reserve_material(int type_material);


    /**
     * 功能：收到原料 type_material，取消预定
    */
    void cancel_reserve_material(int type_material);


    /**
     * 功能：查询这个产品格已经预定，被预定过返回1
    */
    bool check_reserved_product();

    /**
     * 功能：预定产品格
    */
    void reserve_product();


    /**
     * 功能：取消预定产品格
    */
    void cancel_reserve_product();

    /*************************************************************/
    //查询类函数


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

    /**
     * 功能：查询这个工作台还需要多久能有产品
     * -1 是因为缺乏原料而被阻塞
     * 0 是已经有产品格
     * 其他是已经有产品了
    */
    double waiting_time();


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

    // 这个工作台所需要的全部产品类型
    std::set<int> tot_material;

    // 这个工作台已经拥有的原料
    std::set<int> now_material;

    // 这个工作台缺少的原料
    std::set<int> lack_material;

    /**
     * 记录这个原料格是否已经被预定
    */
    std::bitset<15> material_is_reserved;

    /**
     * 记录这个产品格是否已经被预定
    */
    bool product_is_reserved;
};

