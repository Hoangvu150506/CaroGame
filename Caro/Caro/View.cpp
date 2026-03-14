#include "Data.h"
#include "View.h"
#include "Bot.h"
#include "Logic.h"


void DrawBoardRaylib() {
    // 1. Vẽ lưới bàn cờ (Bắt buộc dùng LEFT, TOP, CELL_SIZE)
    for (int i = 0; i <= BOARD_SIZE; i++) {
        // Vẽ các đường ngang
        DrawLine(LEFT, TOP + i * CELL_SIZE, LEFT + BOARD_SIZE * CELL_SIZE, TOP + i * CELL_SIZE, BLACK);
        // Vẽ các đường dọc
        DrawLine(LEFT + i * CELL_SIZE, TOP, LEFT + i * CELL_SIZE, TOP + BOARD_SIZE * CELL_SIZE, BLACK);
    }

    // 2. Vẽ quân cờ X, O dựa trên tọa độ mảng
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Tinh chỉnh cộng thêm 10, 5 để chữ X, O nằm giữa ô cờ (tùy CELL_SIZE)
            if (_A[i][j].c == -1) DrawText("X", _A[i][j].x + 10, _A[i][j].y + 5, CELL_SIZE - 10, RED);
            if (_A[i][j].c == 1)  DrawText("O", _A[i][j].x + 10, _A[i][j].y + 5, CELL_SIZE - 10, BLUE);
        }
    }

    // 3. Vẽ khung chọn (Cursor) màu xanh lá
    DrawRectangleLinesEx(Rectangle{ (float)_X, (float)_Y, (float)CELL_SIZE, (float)CELL_SIZE }, 4.0f, DARKGREEN);

    if (_WINNER == -1 || _WINNER == 1) {
        // 1. Tính toán tâm của ô cờ đầu tiên và ô cờ cuối cùng
        float startX = _A[_winLine.r1][_winLine.c1].x + CELL_SIZE / 2.0f;
        float startY = _A[_winLine.r1][_winLine.c1].y + CELL_SIZE / 2.0f;

        float endX = _A[_winLine.r2][_winLine.c2].x + CELL_SIZE / 2.0f;
        float endY = _A[_winLine.r2][_winLine.c2].y + CELL_SIZE / 2.0f;

        Vector2 startPos = { startX, startY };
        Vector2 endPos = { endX, endY };

        // 2. Chọn màu nổi bật tùy vào người thắng (X thắng đỏ, O thắng xanh)
        Color winColor = (_WINNER == -1) ? RED : BLUE;

     

        // 3. Vẽ một đường thẳng nét liền, dày và đậm màu
        // Thay độ dày 10.0f thành số lớn hơn nếu bạn muốn nét kẻ to hơn nữa
        DrawLineEx(startPos, endPos, 10.0f, winColor);              
    }
}

void DrawAndHandleMenu(Texture2D background, Font gameFont) {
    // Biến tĩnh lưu vị trí đang chọn (0: PvP, 1: PvE)
    static int menuFocus = 0;

    // --- LOGIC NHẬN BÀN PHÍM ---
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) menuFocus = 0;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) menuFocus = 1;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Vẽ nền
    DrawTexturePro(background,
        Rectangle{ 0, 0, (float)background.width, (float)background.height },
        Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        Vector2{ 0, 0 }, 0.0f, WHITE);

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawTextEx(gameFont, "GAME CO CARO", Vector2{ (float)GetScreenWidth() / 2 - 300, 200 }, 100, 2, WHITE);

    // Tọa độ 2 nút bấm
    Rectangle btnPvP = { (float)GetScreenWidth() / 2 - 250, 400, 500, 80 };
    Rectangle btnPvE = { (float)GetScreenWidth() / 2 - 250, 520, 500, 80 };

    // --- LOGIC NHẬN CHUỘT (Đồng bộ với bàn phím) ---
    bool mouseOnPvP = CheckCollisionPointRec(GetMousePosition(), btnPvP);
    bool mouseOnPvE = CheckCollisionPointRec(GetMousePosition(), btnPvE);
    if (mouseOnPvP) menuFocus = 0;
    if (mouseOnPvE) menuFocus = 1;

    // Trạng thái hover cuối cùng
    bool hoverPvP = (menuFocus == 0);
    bool hoverPvE = (menuFocus == 1);

    // Vẽ nút
    DrawRectangleRec(btnPvP, hoverPvP ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnPvP, 4, BLACK);
    DrawTextEx(gameFont, "1. DANH 2 NGUOI", Vector2{ btnPvP.x + 80, btnPvP.y + 20 }, 40, 2, hoverPvP ? WHITE : BLACK);

    DrawRectangleRec(btnPvE, hoverPvE ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnPvE, 4, BLACK);
    DrawTextEx(gameFont, "2. DANH VOI MAY", Vector2{ btnPvE.x + 80, btnPvE.y + 20 }, 40, 2, hoverPvE ? WHITE : BLACK);

    // --- XỬ LÝ CLICK HOẶC BẤM ENTER ---
    if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (mouseOnPvP || mouseOnPvE))) {
        if (menuFocus == 0) {
            _GAME_MODE = 1; _GAME_STATE = 1; _IS_PAUSED = false;
            ResetData(); _p1Moves = 0; _p2Moves = 0; _WINNER = 2;
        }
        else if (menuFocus == 1) {
            _GAME_MODE = 2; _GAME_STATE = 1; _IS_PAUSED = false;
            ResetData(); _p1Moves = 0; _p2Moves = 0; _WINNER = 2;
        }
    }
    EndDrawing();
}

void DrawAndHandleGame(Texture2D background, Font gameFont) {

    if (IsKeyPressed(KEY_ESCAPE) && _WINNER == 2) {
        _IS_PAUSED = !_IS_PAUSED; // Bật / Tắt tạm dừng
    }
    // 1. XỬ LÝ NHẬP PHÍM VÀ LOGIC
    if (_WINNER == 2) {
        if (_TURN == true) { // LƯỢT NGƯỜI 1
            if (IsKeyPressed(KEY_W)) MoveUp();
            if (IsKeyPressed(KEY_S)) MoveDown();
            if (IsKeyPressed(KEY_A)) MoveLeft();
            if (IsKeyPressed(KEY_D)) MoveRight();

            if (IsKeyPressed(KEY_ENTER)) {
                if (CheckBoard(_X, _Y) != 0) {
                    _p1Moves++;
                    _WINNER = TestBoard(_X, _Y);
                    if (_WINNER == 2) _TURN = !_TURN;
                }
            }
        }
        else { // LƯỢT NGƯỜI 2 HOẶC MÁY
            if (_GAME_MODE == 1) { // CHẾ ĐỘ 2 NGƯỜI
                if (IsKeyPressed(KEY_UP)) MoveUp();
                if (IsKeyPressed(KEY_DOWN)) MoveDown();
                if (IsKeyPressed(KEY_LEFT)) MoveLeft();
                if (IsKeyPressed(KEY_RIGHT)) MoveRight();

                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                    if (CheckBoard(_X, _Y) != 0) {
                        _p2Moves++;
                        _WINNER = TestBoard(_X, _Y);
                        if (_WINNER == 2) _TURN = !_TURN;
                    }
                }
            }
            else if (_GAME_MODE == 2) { // CHẾ ĐỘ ĐÁNH VỚI MÁY
                BotMove();
                if (CheckBoard(_X, _Y) != 0) {
                    _p2Moves++;
                    _WINNER = TestBoard(_X, _Y);
                    if (_WINNER == 2) _TURN = !_TURN;
                }
            }
        }
    }

    // 2. VẼ GIAO DIỆN
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Vẽ nền và Bàn cờ
    DrawTexturePro(background,
        Rectangle{ 0, 0, (float)background.width, (float)background.height },
        Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        Vector2{ 0, 0 }, 0.0f, WHITE);
    DrawBoardRaylib();

    // Thông tin người chơi
    DrawTextEx(gameFont, "PLAYER 1 (X)", Vector2{ 50, 300 }, 60, 2, RED);
    DrawTextEx(gameFont, TextFormat("So buoc: %d", _p1Moves), Vector2{ 50, 350 }, 30, 2, BLACK);

    if (_GAME_MODE == 1) DrawTextEx(gameFont, "PLAYER 2 (O)", Vector2{ 50, 800 }, 60, 2, BLUE);
    else DrawTextEx(gameFont, "COMPUTER (O)", Vector2{ 50, 800 }, 60, 2, BLUE);

    DrawTextEx(gameFont, TextFormat("So buoc: %d", _p2Moves), Vector2{ 50, 850 }, 30, 2, BLACK);

    if (_WINNER == 2) {
        if (_TURN) DrawTextEx(gameFont, "LUOT DI: PLAYER 1", Vector2{ 50, 575 }, 70, 2, RED);
        else {
            if (_GAME_MODE == 1) DrawTextEx(gameFont, "LUOT DI: PLAYER 2", Vector2{ 50, 575 }, 70, 2, BLUE);
            else DrawTextEx(gameFont, "MAY DANG SUY NGHI...", Vector2{ 50, 575 }, 60, 2, BLUE);
        }
    }

    // 3. GIAO DIỆN GAME OVER (Và bắt click chuột / bàn phím)
    if (_WINNER != 2) {
        // Biến tĩnh lưu nút đang chọn (0: Chơi lại, 1: Về menu)
        static int gameOverFocus = 0;

        // --- BẮT PHÍM ĐIỀU HƯỚNG ---
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) gameOverFocus = 0;
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) gameOverFocus = 1;

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4f));

        int boxWidth = 800; int boxHeight = 400;
        int boxX = (GetScreenWidth() - boxWidth) / 2;
        int boxY = (GetScreenHeight() - boxHeight) / 2;

        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(RAYWHITE, 0.3f)); // Nền mờ 30%
        DrawRectangleLinesEx(Rectangle{ (float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight }, 5, DARKGRAY);

        DrawTextEx(gameFont, "GAME OVER", Vector2{ (float)boxX + 200, (float)boxY + 50 }, 80, 2, DARKGRAY);

        if (_WINNER == -1) DrawTextEx(gameFont, "NGUOI CHOI 1 (X) THANG!", Vector2{ (float)boxX + 100, (float)boxY + 150 }, 55, 2, RED);
        else if (_WINNER == 1) DrawTextEx(gameFont, (_GAME_MODE == 1) ? "NGUOI CHOI 2 (O) THANG!" : "MAY TINH DA THANG!", Vector2{ (float)boxX + 150, (float)boxY + 150 }, 55, 2, BLUE);
        else if (_WINNER == 0) DrawTextEx(gameFont, "HAI BEN HOA NHAU!", Vector2{ (float)boxX + 180, (float)boxY + 150 }, 55, 2, DARKGRAY);

        Rectangle btnPlayAgain = { (float)boxX + 100, (float)boxY + 280, 280, 80 };
        Rectangle btnMenu = { (float)boxX + 420, (float)boxY + 280, 280, 80 };

        // --- ĐỒNG BỘ CHUỘT VỚI PHÍM ---
        bool mouseOnPlay = CheckCollisionPointRec(GetMousePosition(), btnPlayAgain);
        bool mouseOnMenu = CheckCollisionPointRec(GetMousePosition(), btnMenu);

        if (mouseOnPlay) gameOverFocus = 0;
        if (mouseOnMenu) gameOverFocus = 1;

        bool isHoverPlay = (gameOverFocus == 0);
        bool isHoverMenu = (gameOverFocus == 1);

        // Nút CHƠI LẠI
        DrawRectangleRec(btnPlayAgain, isHoverPlay ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(btnPlayAgain, 3, BLACK);
        DrawTextEx(gameFont, "CHOI LAI", Vector2{ btnPlayAgain.x + 40, btnPlayAgain.y + 20 }, 45, 2, isHoverPlay ? WHITE : BLACK);

        // Nút VỀ MENU
        DrawRectangleRec(btnMenu, isHoverMenu ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(btnMenu, 3, BLACK);
        DrawTextEx(gameFont, "VE MENU", Vector2{ btnMenu.x + 50, btnMenu.y + 20 }, 45, 2, isHoverMenu ? WHITE : BLACK);

        // --- XỬ LÝ CHỌN BẰNG ENTER HOẶC CLICK CHUỘT TRÁI ---
        if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (mouseOnPlay || mouseOnMenu))) {
            if (gameOverFocus == 0) {
                int currentMode = _GAME_MODE;
                ResetData();
                _p1Moves = 0; _p2Moves = 0; _WINNER = 2;
                _GAME_MODE = currentMode;

                // Trả focus về nút "Chơi lại" cho ván sau
                gameOverFocus = 0;
            }
            else if (gameOverFocus == 1) {
                _GAME_STATE = 0;
            }
        }
    }
    // 4.  GIAO DIỆN TẠM DỪNG (PAUSE MENU) 
    if (_IS_PAUSED && _WINNER == 2) {
        static int pauseFocus = 0; // 0: Tiếp tục, 1: Về menu

        // Bắt phím W, S để điều hướng
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) pauseFocus = 0;
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) pauseFocus = 1;

        // Phủ lớp đen mờ lên toàn màn hình
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

        int boxWidth = 500; int boxHeight = 400;
        int boxX = (GetScreenWidth() - boxWidth) / 2;
        int boxY = (GetScreenHeight() - boxHeight) / 2;

        // Bảng Pause
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(RAYWHITE, 0.9f));
        DrawRectangleLinesEx(Rectangle{ (float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight }, 5, DARKGRAY);

        DrawTextEx(gameFont, "TAM DUNG", Vector2{ (float)boxX + 100, (float)boxY + 50 }, 70, 2, DARKGRAY);

        Rectangle btnContinue = { (float)boxX + 100, (float)boxY + 160, 300, 70 };
        Rectangle btnMenu = { (float)boxX + 100, (float)boxY + 260, 300, 70 };

        bool mouseOnContinue = CheckCollisionPointRec(GetMousePosition(), btnContinue);
        bool mouseOnMenu = CheckCollisionPointRec(GetMousePosition(), btnMenu);

        if (mouseOnContinue) pauseFocus = 0;
        if (mouseOnMenu) pauseFocus = 1;

        bool isHoverContinue = (pauseFocus == 0);
        bool isHoverMenu = (pauseFocus == 1);

        // Nút Tiếp tục
        DrawRectangleRec(btnContinue, isHoverContinue ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(btnContinue, 3, BLACK);
        DrawTextEx(gameFont, "TIEP TUC", Vector2{ btnContinue.x + 60, btnContinue.y + 15 }, 45, 2, isHoverContinue ? WHITE : BLACK);

        // Nút Về Menu
        DrawRectangleRec(btnMenu, isHoverMenu ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(btnMenu, 3, BLACK);
        DrawTextEx(gameFont, "VE MENU", Vector2{ btnMenu.x + 70, btnMenu.y + 15 }, 45, 2, isHoverMenu ? WHITE : BLACK);

        // Xử lý chọn
        if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (mouseOnContinue || mouseOnMenu))) {
            if (pauseFocus == 0) {
                _IS_PAUSED = false; // Tắt pause, chơi tiếp
            }
            else if (pauseFocus == 1) {
                _GAME_STATE = 0;    // Về màn hình chính
                _IS_PAUSED = false; // Reset lại biến Pause
            }
        }
    }
    EndDrawing();
}