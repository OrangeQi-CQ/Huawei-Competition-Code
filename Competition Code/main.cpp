#include "state.h"
#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"


#include <iostream>

/*
g++ *.cpp -o D:\ccc\hw5\WindowsRelease\Demo\SimpleDemo.exe
*/


Game game;


int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output test.txt", "w", stdout);

    game.lets_work();
    return 0;
} 
