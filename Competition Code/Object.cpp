#include "Object.h"



void Object::init() {
    for (int i = 0; i <= 10; i++) {
        material[i].on_sale.clear();
        material[i].in_seek.clear();
        // has[i] = 0;
    }
}