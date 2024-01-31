#pragma once
extern int frameID;
extern int current_money;
extern int num_workbench;
extern double rest_time;


#include "string.h"
#include "Map.h"
#include <set>


extern std::set<int> reliance[10];
extern char gamemap[105][105];

/**
 * 记录这种元素总共已经生产了多少个
*/
extern int tot_material[10];

/**
* 某种工作台的编号集合
*/
extern std::set<int> wbtypes[10];


int mapID();