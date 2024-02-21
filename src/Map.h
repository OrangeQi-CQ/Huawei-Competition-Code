#pragma once
#include <set>

// #include "Workbench.h"



class GameMap {
public:
    // friend class Robot;
    // friend class TaskManager;
    // friend class Workbench;

    /**
     * 读入地图
    */
    // void read_map(Workbench workbench[]);


    char game_map_[105][105];
    char templine_[1024];
};