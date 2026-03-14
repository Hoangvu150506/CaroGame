#include "raylib.h"
#include "Data.h"
#include "Logic.h"
#include "View.h"
#include "Bot.h"

// Định nghĩa biến toàn cục
_POINT _A[BOARD_SIZE][BOARD_SIZE];
bool _TURN;
int _COMMAND;
int _X, _Y;
WinLine _winLine;
int _p1Moves = 0;
int _p2Moves = 0;
int _WINNER = 2;
int _GAME_STATE = 0;
int _GAME_MODE = 1;
bool _IS_PAUSED = false;

int main() {
    InitWindow(1200 * 2, 752 * 2, "Do An Caro - KHTN - MENU & BOT");
    SetTargetFPS(60);
    SetExitKey(0);
    Texture2D background = LoadTexture("background.png");
    Font gameFont = LoadFont("game_font.ttf");

    ResetData();
    _p1Moves = 0; _p2Moves = 0; _WINNER = 2;

    while (!WindowShouldClose()) {

        // Tất cả logic đồ sộ đã được giấu gọn gàng vào 2 hàm này
        if (_GAME_STATE == 0) {
            DrawAndHandleMenu(background, gameFont);
        }
        else if (_GAME_STATE == 1) {
            DrawAndHandleGame(background, gameFont);
        }

    }

    UnloadFont(gameFont);
    UnloadTexture(background);
    CloseWindow();
    return 0;
}