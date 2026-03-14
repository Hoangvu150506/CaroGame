#ifndef DATA_H
#define DATA_H

#include "raylib.h"

// Hằng số từ tài liệu [cite: 52, 53, 54]
#define BOARD_SIZE 20
#define LEFT 702       // Điều chỉnh lại tọa độ cho phù hợp màn hình đồ họa
#define TOP 196
#define CELL_SIZE 50 // Kích thước ô vuông bàn cờ

// Cấu trúc dữ liệu [cite: 66]
struct _POINT {
    int x, y, c;
};

// Khai báo biến (Dùng extern để tránh lỗi trùng lặp khi include) [cite: 67, 68, 69, 70]
extern _POINT _A[BOARD_SIZE][BOARD_SIZE];
extern bool _TURN;
extern int _COMMAND;
extern int _X, _Y;
extern int _p1Moves;
extern int _p2Moves;
extern int _WINNER;



struct WinLine {
    int r1, c1; // Tọa độ hàng/cột của quân cờ đầu tiên
    int r2, c2; // Tọa độ hàng/cột của quân cờ thứ 5
};

extern WinLine _winLine;
extern int _WINNER; // Để View.cpp biết lúc nào thì vẽ đường thẳng

extern int _GAME_STATE; // 0: Đang ở Menu, 1: Đang chơi game
extern int _GAME_MODE;  // 1: Người vs Người (PvP), 2: Người vs Máy (PvE)

extern bool _IS_PAUSED;
#endif