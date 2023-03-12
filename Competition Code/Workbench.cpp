#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"

#include <iostream>



void Workbench::read(Object &object) {
    scanf("%d", &workbench_type);
    scanf("%lf%lf", &position.x, &position.y);


    scanf("%d", &remaining_prodution_frame);

    int material_state;
    scanf("%d", &material_state);

    now_material.clear();
    std::set<int> lack;

    switch (workbench_type) {
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            lack = {1, 2};
            break;
        case 5:
            lack = {1, 3};
            break;
        case 6:
            lack = {2, 3};
            break;
        case 7:
            lack = {4, 5, 6};
            break;
        case 8:
            lack = {7};
            break;
        default:
            lack = {1, 2, 3, 4, 5, 6, 7};
            break;
    }


    for (int i = 1; i <= 9; i++) {
        if (material_state & (1 << i)) {
            now_material.insert(i);
            lack.erase(i);
            // object.material[i].in_seek.erase(workbenchID);
        }
    }

    for (int x : lack) {
        object.material[x].in_seek.insert(workbenchID);
    }


    scanf("%d", &Have_product);

    if (Have_product) {
        object.material[type()].on_sale.insert(workbenchID);
    }
}



bool Workbench::find_material(int x) {
    return now_material.find(x) != now_material.end();
}



int Workbench::ID() {
    return workbenchID;
}


int Workbench::type() {
    return workbench_type;
}

Point Workbench::pos() {
    return position;
}

bool Workbench::have_product() {
    return Have_product;
}
