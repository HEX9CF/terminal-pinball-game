# 基于 curses 库实现多线程弹球游戏

## 简介

一个基于 `curses` 库的终端游戏，包含一个球和一个挡板。玩家通过控制挡板来接住球，取得游戏分数。

## 功能

1. **初始化**：初始化屏幕，设置非阻塞输入模式并隐藏光标。
2. **定义对象**：定义球和挡板的结构体，初始化它们的位置、速度和外观。
3. **游戏线程**：在一个循环函数中更新球和挡板的位置，检测球与墙壁、板的碰撞，更新屏幕显示并检测游戏结束条件。
4. **处理输入**：获取用户输入并根据输入更新板的位置。
5. **结束游戏**：显示游戏结束画面，打印分数。

## 使用方法

1. 编译代码：
    ```sh
    gcc -o game main.c -lncurses -lpthread
    ```

2. 运行游戏：
    ```sh
    ./game
    ```

## 游戏操作
- **进入主界面**：在Linux终端运行程序后，进入主界面。
- **开始游戏**：按 `s` 键开始游戏。
- **移动挡板**：
  - 按 `h` 键向左移动挡板。
  - 按 `l` 键向右移动挡板。
- **调节小球速度**：
  - 按 `j` 键降低小球速度。
  - 按 `k` 键增加小球速度。
- **游戏结束**：若小球飞出边界，游戏结束，屏幕上会打印分数。
- **重新开始游戏**：按 `r` 键重新开始游戏。
- **退出游戏**：按 `q` 键退出游戏。

## 注意事项
- 确保终端支持 `curses` 库。
- 游戏过程中请勿调整终端窗口大小，以免影响游戏显示效果。