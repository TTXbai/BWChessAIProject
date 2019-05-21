/*Game logic define file*/

#define BLACK	1
#define WHITE	-1
#define BLANK	0
#define TURN_BLACK 
#define TURN_WHITE -1

extern int Chessboard[64];
extern int Playableboard[64];


void InitChessboard();
int PlayChess(int row, int line, int turn);
void RefreshPlayableboard(int turn);
void DrawChessboard();
void DrawPlayableboard();
int IfGameOver(int ChessBorad[64]);
int JudgeWinner(int ChessBorad[64]);