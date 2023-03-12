#pragma once

#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"



class GameMap {
public:
    void read_map();


private:
    char game_map[105][105];
    char templine[10];
};