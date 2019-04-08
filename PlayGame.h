/*Game logic define file*/

#define BLACK	1
#define WHITE	2
#define BLANK	0
#define TURN_BLACK 1
#define TURN_WHITE 2


void InitChessboard();
int PlayChess(int row, int line, int turn);
void RefreshPlayableboard(int turn);
void DrawChessboard();
void DrawPlayableboard();