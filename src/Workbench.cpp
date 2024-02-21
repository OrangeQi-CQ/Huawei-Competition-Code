#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "game.h"
#include "geometry.h"
#include "state.h"

#include <iostream>



void Workbench::read(int id) {
    workbenchID = id;

    scanf("%d", &workbench_type);

    scanf("%lf%lf", &position.x, &position.y);
    scanf("%d", &remaining_prodution_frame);

    int material_state;
    scanf("%d", &material_state);
    if (!tot_material.size()) {
        switch (workbench_type) {
            case 4:
                tot_material = {1, 2};
                break;
            case 5:
                tot_material = {1, 3};
                break;
            case 6:
                tot_material = {2, 3};
                break;
            case 7:
                tot_material = {4, 5, 6};
                break;
            case 8:
                tot_material = {7};
                break;
            default:
                break;
        }
    }
    now_material.clear();
    lack_material.clear();
    for (int i = 1; i <= 9; i++) {
        if (material_state & (1 << i)) {
            now_material.insert(i);
        }
    }
    for (int t : tot_material) {
        if (now_material.find(t) == now_material.end()) {
            lack_material.insert(t);
        }
    }

    scanf("%d", &Have_product);
}



/*************************************************************/
// 关于预定的一组函数

bool Workbench::check_reserved_material(int type_material) {
    return material_is_reserved[type_material];
}

void Workbench::reserve_material(int type_material) {
    material_is_reserved[type_material] = 1;
}


void Workbench::cancel_reserve_material(int type_material) {
    material_is_reserved[type_material] = 0;
}


bool Workbench::check_reserved_product() {
    return product_is_reserved;
}

void Workbench::reserve_product() {
    product_is_reserved = 1;
}


void Workbench::cancel_reserve_product() {
    product_is_reserved = 0;
}


/*************************************************************/
// 用于查询的函数


int Workbench::ID() {
    return workbenchID;
}


bool Workbench::find_material(int x) {
    return now_material.find(x) != now_material.end();
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


double Workbench::waiting_time() {
    return 1.0 * remaining_prodution_frame / 50;
}
