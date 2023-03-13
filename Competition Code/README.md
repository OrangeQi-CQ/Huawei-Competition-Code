# Huawei Competion Code


## Robot 类的公用接口

### 重要成员

- `mission`：是一个 `Mission` 类成员，记录了当前机器人的任务目标和任务状态。包括前往购买的工作台指针，购买成功标志，前往售卖的工作台指针，售卖成功标志。

- `instruct`：是一个 `std::vector<Instruct>` 成员，负责暂存当前帧这个机器人即将发出的指令。

- `target_position` 是一个 `Point` 类，记录这一帧这个机器人想要前往的位置坐标



### 行为

读入和输出

- `void read(int ID)`：在一帧中读入编号为 `id` 的机器人的信息

- `void print_instruct(int ID)`：在这一帧结束的时候，由 `Startgame::print_frame()` 调用，输出编号为 `ID` 的机器人在这一帧的指令

防碰撞

- `void avoid_collision()`：生成规避碰撞的指令，写入 `instruct` 中 

mission 任务周期

- `void set_mission(Workbench *workbench_buy, Workbench *workbench_sell)`：两个步骤
  - 设定当前机器人的 `mission`，即根据 `*workbench_buy` 和 `*workbench_sell` 初始化 `mission` 
  - 完成原料和产品的预定


- `void perform_mission()`： 根据当前机器人的 `mission` 成员决策这一帧里需要做什么，包括
  -  调用`change_target(Point, int)`，根据任务状态修改目的地 
  -  调用 `move_to_target()` 前往目的地 `target_position`
  -  `buy()`，`sell()` ，`destroy()` 等行为


- `void finish_mission`：结束任务，清空 `mission` 和 `target_posion`，并将 `Has_mission` 设为 `0`

mission 周期内可以调用的指令

- `void change_target(Point target_point)`：修改当前机器人现在想前往的坐标

- `void move_to_target()`：根据机器人想要前往的坐标 `target_position`，结合机器人这一帧读入的自身状态信息，生成他这一帧的运动指令集，写入 `instruct` 中

- `void buy()`：将购买指令写入 `instruct` 中

- `void sell()`：将售卖指令写入 `instruct` 中

- `void destroy()`：将销毁指令写入 `instruct` 中

### 查询

- `int ID()`：查询当前机器人的编号

- `int type()`：查询当前机器人所携带物品的类型

- `Point pos()`：查询当前机器人所在的位置坐标

- `Point target_pos()`：查询这个机器人正在前往的位置坐标

- `bool has_mission()`：查询这个机器人是否正处在任务中

- `int workbench()`：查询这个机器人所处的工作台编号，没有则返回 -1


 
## Workbench 类的公用接口

### 行为

- `void read(int id)`： 在一帧当中读入当前工作台（编号为 `id`）的信息

- `bool reserve_cell(int type_material)`：预定类型为 `type_material` 的原料格，有空余原料格则返回 `true`


- `void cacel_reserve(int type_material)`：原料格收到原料，取消预定


### 查询

- `int ID()`： 返回工作台ID编号

- `Point pos()`： 返回当前工作台坐标 

- `int type()`： 返回工作台类型，即生产商品类型

- `bool find_material(int type_material)`：  查询是否已经拥有 `type_material` 这种原料


- `bool have_product()`： 查询这个机器人产品格里面是否有产品




