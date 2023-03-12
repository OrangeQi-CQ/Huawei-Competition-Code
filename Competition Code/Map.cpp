#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"


#include <iostream>


void GameMap::read_map() {
    for (int i = 1; i <= 100; i++) {
        scanf("%s", game_map[i] + 1);
    }

    scanf("%s", templine);
    puts("OK");
    fflush(stdout);
} 