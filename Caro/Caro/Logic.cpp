#include "Data.h"
#include "Logic.h"

// Tận dụng Bước 4 [cite: 71, 73]
void ResetData() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            _A[i][j].x = LEFT + j * CELL_SIZE;
            _A[i][j].y = TOP + i * CELL_SIZE;
            _A[i][j].c = 0; // 0: chưa đánh, -1: X, 1: O [cite: 73]
        }
    }
    _TURN = true;
    int center = BOARD_SIZE / 2;
    _X = _A[center][center].x;
    _Y = _A[center][center].y;
}

// Tận dụng Bước 10 [cite: 116, 117]
int CheckBoard(int pX, int pY) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0) {
                if (_TURN == true) _A[i][j].c = -1;
                else _A[i][j].c = 1;
                return _A[i][j].c;
            }
        }
    }
    return 0;
}

// Logic di chuyển tận dụng Bước 11 (Bỏ GotoXY) [cite: 118, 119, 121]

void MoveRight() {
    // Kiểm tra xem có vượt quá biên phải của ma trận không [cite: 119]
    if (_X < _A[0][BOARD_SIZE - 1].x) _X += CELL_SIZE;
}
void MoveLeft() {
    // Kiểm tra biên trái [cite: 119]
    if (_X > _A[0][0].x) _X -= CELL_SIZE;
}
void MoveDown() {
    // Kiểm tra biên dưới [cite: 119]
    if (_Y < _A[BOARD_SIZE - 1][0].y) _Y += CELL_SIZE;
}
void MoveUp() {
    // Kiểm tra biên trên [cite: 121]
    if (_Y > _A[0][0].y) _Y -= CELL_SIZE;
}

bool CheckDirection(int row, int col, int dr, int dc) {
    int value = _A[row][col].c;
    if (value == 0) return false;

    int count = 1;
    // Khởi tạo 2 đầu mút ban đầu chính là viên cờ vừa đánh
    int r1 = row, c1 = col;
    int r2 = row, c2 = col;

    // Kiểm tra tiến về một hướng (Ví dụ: sang phải)
    for (int i = 1; i < 5; i++) {
        int r = row + dr * i;
        int c = col + dc * i;
        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && _A[r][c].c == value) {
            count++;
            r1 = r; c1 = c; // Cập nhật liên tục đầu mút 1
        }
        else break;
    }

    // Kiểm tra lùi về hướng ngược lại (Ví dụ: sang trái)
    for (int i = 1; i < 5; i++) {
        int r = row - dr * i;
        int c = col - dc * i;
        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && _A[r][c].c == value) {
            count++;
            r2 = r; c2 = c; // Cập nhật liên tục đầu mút 2
        }
        else break;
    }

    // Nếu đủ 5 viên thì chốt sổ tọa độ vào biến toàn cục
    if (count >= 5) {
        _winLine.r1 = r1; _winLine.c1 = c1;
        _winLine.r2 = r2; _winLine.c2 = c2;
        return true;
    }

    return false;
}
// Bước 9: Kiểm tra thắng thua (Sinh viên cần tự viết luật Caro) [cite: 103, 106]
int TestBoard(int pX, int pY) {
    // 1. Chuyển tọa độ màn hình sang chỉ số mảng (row, col)
    int row = -1, col = -1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (_A[i][j].x == pX && _A[i][j].y == pY) {
                row = i; col = j;
                break;
            }
        }
    }

    if (row == -1) return 2; // Chưa tìm thấy vị trí hợp lệ

    // 2. Kiểm tra 4 hướng từ ô vừa đánh
    if (CheckDirection(row, col, 1, 0) || // Dọc
        CheckDirection(row, col, 0, 1) || // Ngang
        CheckDirection(row, col, 1, 1) || // Chéo xuôi
        CheckDirection(row, col, 1, -1))  // Chéo ngược
    {
        return (_A[row][col].c == -1) ? -1 : 1; // Trả về người thắng [cite: 106]
    }

    // 3. Kiểm tra hòa (Ma trận đầy) [cite: 106]
    bool isFull = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (_A[i][j].c == 0) { isFull = false; break; }
        }
    }
    if (isFull) return 0; // Hòa [cite: 106]

    return 2; // Chưa ai thắng [cite: 111]
}