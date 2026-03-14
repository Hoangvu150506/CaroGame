#ifndef LOGIC_H
#define LOGIC_H

// Tận dụng Bước 4 [cite: 71, 73]
void ResetData();

// Tận dụng Bước 10 [cite: 116, 117]
int CheckBoard(int pX, int pY);

// Logic di chuyển tận dụng Bước 11 (Bỏ GotoXY) [cite: 118, 119, 121]
void MoveRight(); 
void MoveLeft(); 
void MoveDown(); 
void MoveUp(); 
bool CheckDirection(int row, int col, int dr, int dc);
// Bước 9: Kiểm tra thắng thua (Sinh viên cần tự viết luật Caro) [cite: 103, 106]
int TestBoard(int pX,int pY);
#endif