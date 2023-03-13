#pragma once



#include "Map.h"




class GameMap {
public:
    void read_map();


private:
    char game_map[105][105];
    char templine[1024];
};