#include "Robot.h"
#include "Workbench.h"
#include "Map.h"
#include "Object.h"
#include "game.h"
#include "Point.h"
#include "state.h"

#include <iostream>



void Workbench::read(int id) {
    workbenchID = id;

    scanf("%d", &workbench_type);
    scanf("%lf%lf", &position.x, &position.y);


    scanf("%d", &remaining_prodution_frame);

    int material_state;
    scanf("%d", &material_state);


    now_material.clear();

    for (int i = 1; i <= 9; i++) {
        if (material_state & (1 << i)) {
            now_material.insert(i);
        }
    }

    scanf("%d", &Have_product);

//     if (Have_product) {
//         object.material[type()].on_sale.insert(workbenchID);
//     }
// }
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

