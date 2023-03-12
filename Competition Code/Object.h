#pragma once
#include <set>



class Material {
public:
    int ID;

    // 哪些工作台现在生产出来了这种原料
    std::set<int> on_sale;

    // 哪些工作台现在需求这种原料
    std::set<int> in_seek;

};



class Object {
public:

    /**
     * 功能：产生任务集
     * 每读取一帧，就要进行一次 init
    */
    void init();

    Material material[55];
    std::set<int> lock;


private:
    int objectID;
};

