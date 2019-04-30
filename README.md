# BWChessAIProject
The reversi chess game is written in C, and have an simple AI for the project

C语言实现的黑白棋，并有一个简单的AI

4.30.2019

提交了AI第一版的代码，由于查重的缘故，AI取消了部分功能，应该能正常运行

目前的AI强度偏弱，是由于搜索树的判定以及部分参数没调整好的缘故

## AI实现
- 蒙特卡洛树搜索（MCTS)

- α-β剪枝
  
## 提升点
1. 完善AI的参数判断
2. 增加强化学习
3. 增加多重判断，以增加强度（虽然可能使AI更弱
4. 优化算法实现
5. Main函数需要独立出来（代码仍不够规范emmm

## 命令
  gcc -o playit AI.c PlayGame.c -lm
