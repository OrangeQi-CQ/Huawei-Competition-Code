#include "state.h"

int frameID = 0;
int current_money = 0;
int num_workbench = 0;
double rest_time = 180;



char gamemap[105][105];
std::set<int> reliance[10];
std::set<int> wbtypes[10];
int tot_material[10] = {0};