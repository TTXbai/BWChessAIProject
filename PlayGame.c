#include <stdio.h>
#include <string.h>
#include "PlayGame.h"

int Chessboard[64];
int Playableboard[64];

/*int main()
{
	int turn = BLACK;
	int row,line;
	InitChessboard();
	while(1)
	{
		DrawChessboard();
		if(turn == BLACK)
		{
			RefreshPlayableboard(turn);
			DrawPlayableboard();
			printf("It is BLACK turn, please input row and line:\n");
			scanf("%d %d",&row,&line);
			if(PlayChess(row,line,turn))
			turn = WHITE;
		}
		else if(turn == WHITE)
		{
			RefreshPlayableboard(turn);
			DrawPlayableboard();
			printf("It is WHITE turn, please input row and line:\n");
			scanf("%d %d",&row,&line);
			if(PlayChess(row,line,turn))
			turn = BLACK;
		}
		//Judge if game over
	}
	return 0;
}*/

void InitChessboard()
{
	int i;
	for(i = 0; i < 64; i++)
		Chessboard[i] = BLANK;
	/*start  chessboard*/
	Chessboard[3 * 8 + 3] = Chessboard[4 * 8 + 4] = WHITE;
	Chessboard[3 * 8 + 4] = Chessboard[4 * 8 + 3] = BLACK;
	return;
}

int PlayChess(int row, int line, int turn)
{
	if (Playableboard[row * 8 + line] == 0)
	{
		printf("Error, cannot play\n");
		return 0;
	}
	Chessboard[row * 8 + line] = turn;
	int i = row,j = line,k,flag = 0;
	if(turn == WHITE)
	{
	/*Judge left and right*/			
		if(j && Chessboard[i * 8 + j - 1] == BLACK)
			for( k = j - 2; k >= 0; k--)
			{
				if(Chessboard[i * 8 + k] == BLACK)
					continue;
				else if(Chessboard[i * 8 + k] == WHITE)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = j - 1; k >= 0; k--)
			{
				if(Chessboard[i * 8 + k] == BLACK)
					Chessboard[i * 8 + k] = WHITE;
				else if(Chessboard[i * 8 + k] == WHITE)
				{
					flag = 0;
					break;
				}
			}		
		}
		if(j < 7 && Chessboard[i * 8 + j + 1] == BLACK)
			for( k = j + 2; k < 8; k++)
			{
				if(Chessboard[i * 8 + k] == BLACK)
					continue;
				else if(Chessboard[i * 8 + k] == WHITE)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = j + 1; k < 8; k++)
			{
				if(Chessboard[i * 8 + k] == BLACK)
					Chessboard[i * 8 + k] = WHITE;
				else if(Chessboard[i * 8 + k] == WHITE)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		/*Judge up and down*/
		if(i && Chessboard[(i - 1) * 8 + j] == BLACK)
			for( k = i - 2; k >= 0; k--)
			{
				if(Chessboard[k * 8 + j] == BLACK)
					continue;
				else if(Chessboard[k * 8 + j] == WHITE)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = i - 1; k >= 0; k--)
			{
				if(Chessboard[k * 8 + j] == BLACK)
					Chessboard[k * 8 + j] = WHITE;
				else if(Chessboard[k * 8 + j] == WHITE)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		if(i < 7 && Chessboard[(i + 1) * 8 + j] == BLACK)
			for( k = i + 2; k < 8; k++)
			{
				if(Chessboard[k * 8 + j] == BLACK)
					continue;
				else if(Chessboard[k * 8 + j] == WHITE)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = i + 1; k < 8; k++)
			{
				if(Chessboard[k * 8 + j] == BLACK)
					Chessboard[k * 8 + j] = WHITE;
				else if(Chessboard[k * 8 + j] == WHITE)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		/*Judge oblique line*/
		if(i < 7 && j < 7 && Chessboard[(i + 1) * 8 + j + 1] == BLACK)
		{
			for( k = 2; k + i < 8 && k + j < 8; k++ )
			{
				if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
					continue;
				else if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; k + i < 8 && k + j < 8; k++ )
			{
				if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
					Chessboard[(k+i)*8 + (k+j)] = WHITE;
				else if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i < 7 && j > 0 && Chessboard[(i + 1) * 8 + j - 1] == BLACK)
		{
			for( k = 2; k + i < 8 && j - k  >= 0; k++ )
			{
				if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
					continue;
				else if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; k + i < 8 && j - k  >= 0; k++ )
			{
				if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
					Chessboard[(k+i)*8 + (j - k)] = WHITE;
				else if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i > 0 && j < 7 && Chessboard[(i - 1) * 8 + j + 1] == BLACK)
		{
			for( k = 2; i - k >= 0 && k + j < 8; k++ )
			{
				if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
					continue;
				else if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; i - k >= 0 && k + j < 8; k++ )
			{
				if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
					Chessboard[(i - k)*8 + (k+j)] = WHITE;
				else if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i > 0 && j > 0 && Chessboard[(i - 1) * 8 + j - 1] == BLACK)
		{
			for( k = 2; i - k >= 0 && j - k >= 0; k++ )
			{
				if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
					continue;
				else if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; i - k >= 0 && j - k >= 0; k++ )
			{
				if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
					Chessboard[(i-k)*8 + (j-k)] = WHITE;
				else if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
	}
	else
	{
		if(j && Chessboard[i * 8 + j - 1] == WHITE)
			for( k = j - 2; k >= 0; k--)
			{
				if(Chessboard[i * 8 + k] == WHITE)
					continue;
				else if(Chessboard[i * 8 + k] == BLACK)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = j - 1; k >= 0; k--)
			{
				if(Chessboard[i * 8 + k] == WHITE)
					Chessboard[i * 8 + k] = BLACK;
				else if(Chessboard[i * 8 + k] == BLACK)
				{
					flag = 0;
					break;
				}
			}		
		}
		if(j < 7 && Chessboard[i * 8 + j + 1] == WHITE)
			for( k = j + 2; k < 8; k++)
			{
				if(Chessboard[i * 8 + k] == WHITE)
					continue;
				else if(Chessboard[i * 8 + k] == BLACK)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = j + 1; k < 8; k++)
			{
				if(Chessboard[i * 8 + k] == WHITE)
					Chessboard[i * 8 + k] = BLACK;
				else if(Chessboard[i * 8 + k] == BLACK)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		/*Judge up and down*/
		if(i && Chessboard[(i - 1) * 8 + j] == WHITE)
			for( k = i - 2; k >= 0; k--)
			{
				if(Chessboard[k * 8 + j] == WHITE)
					continue;
				else if(Chessboard[k * 8 + j] == BLACK)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = i - 1; k >= 0; k--)
			{
				if(Chessboard[k * 8 + j] == WHITE)
					Chessboard[k * 8 + j] = BLACK;
				else if(Chessboard[k * 8 + j] == BLACK)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		if(i < 7 && Chessboard[(i + 1) * 8 + j] == WHITE)
			for( k = i + 2; k < 8; k++)
			{
				if(Chessboard[k * 8 + j] == WHITE)
					continue;
				else if(Chessboard[k * 8 + j] == BLACK)
				{
					flag = 1;
					break;
				}	
				else /*get blank*/
					break;
			}
		if(flag)
		{
			for( k = i + 1; k < 8; k++)
			{
				if(Chessboard[k * 8 + j] == WHITE)
					Chessboard[k * 8 + j] = BLACK;
				else if(Chessboard[k * 8 + j] == BLACK)
				{
					flag = 0;
					break;
				}	
				else /*get blank*/
					break;
			}
		}
		/*Judge oblique line*/
		if(i < 7 && j < 7 && Chessboard[(i + 1) * 8 + j + 1] == WHITE)
		{
			for( k = 2; k + i < 8 && k + j < 8; k++ )
			{
				if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
					continue;
				else if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; k + i < 8 && k + j < 8; k++ )
			{
				if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
					Chessboard[(k+i)*8 + (k+j)] = BLACK;
				else if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i < 7 && j > 0 && Chessboard[(i + 1) * 8 + j - 1] == WHITE)
		{
			for( k = 2; k + i < 8 && j - k  >= 0; k++ )
			{
				if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
					continue;
				else if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; k + i < 8 && j - k  >= 0; k++ )
			{
				if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
					Chessboard[(k+i)*8 + (j - k)] = BLACK;
				else if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i > 0 && j < 7 && Chessboard[(i - 1) * 8 + j + 1] == WHITE)
		{
			for( k = 2; i - k >= 0 && k + j < 8; k++ )
			{
				if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
					continue;
				else if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; i - k >= 0 && k + j < 8; k++ )
			{
				if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
					Chessboard[(i - k)*8 + (k+j)] = BLACK;
				else if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
		if(i > 0 && j > 0 && Chessboard[(i - 1) * 8 + j - 1] == WHITE)
		{
			for( k = 2; i - k >= 0 && j - k >= 0; k++ )
			{
				if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
					continue;
				else if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
				{
					flag = 1;
					break;
				}
				else
					break;
			}
		}
		if(flag)
		{
			for( k = 1; i - k >= 0 && j - k >= 0; k++ )
			{
				if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
					Chessboard[(i-k)*8 + (j-k)] = BLACK;
				else if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
				{
					flag = 0;
					break;
				}
				else
					break;
			}
		}
	}
	return 1;
}

void RefreshPlayableboard(int turn)
{
	int i, j, k;
	if(turn == WHITE)
	{
		for( i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				int flag = 0;
				if(Chessboard[i * 8 + j] == BLANK)
				{	
					/*Judge left and right*/			
					if(j && Chessboard[i * 8 + j - 1] == BLACK)
						for( k = j - 2; k >= 0; k--)
						{
							if(Chessboard[i * 8 + k] == BLACK)
								continue;
							else if(Chessboard[i * 8 + k] == WHITE)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(j < 7 && Chessboard[i * 8 + j + 1] == BLACK)
						for( k = j + 2; k < 8; k++)
						{
							if(Chessboard[i * 8 + k] == BLACK)
								continue;
							else if(Chessboard[i * 8 + k] == WHITE)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					/*Judge up and down*/
					if(i && Chessboard[(i - 1) * 8 + j] == BLACK)
						for( k = i - 2; k >= 0; k--)
						{
							if(Chessboard[k * 8 + j] == BLACK)
								continue;
							else if(Chessboard[k * 8 + j] == WHITE)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i * 8 + j] = 1;
						continue;
					}
					if(i < 7 && Chessboard[(i + 1) * 8 + j] == BLACK)
						for( k = i + 2; k < 8; k++)
						{
							if(Chessboard[k * 8 + j] == BLACK)
								continue;
							else if(Chessboard[k * 8 + j] == WHITE)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					/*Judge oblique line*/
					if(i < 7 && j < 7 && Chessboard[(i + 1) * 8 + j + 1] == BLACK)
					{
						for( k = 2; k + i < 8 && k + j < 8; k++ )
						{
							if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
								continue;
							else if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i < 7 && j > 0 && Chessboard[(i + 1) * 8 + j - 1] == BLACK)
					{
						for( k = 2; k + i < 8 && j - k  >= 0; k++ )
						{
							if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
								continue;
							else if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i > 0 && j < 7 && Chessboard[(i - 1) * 8 + j + 1] == BLACK)
					{
						for( k = 2; i - k >= 0 && k + j < 8; k++ )
						{
							if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
								continue;
							else if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i > 0 && j > 0 && Chessboard[(i - 1) * 8 + j - 1] == BLACK)
					{
						for( k = 2; i - k >= 0 && j - k >= 0; k++ )
						{
							if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
								continue;
							else if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
				}
				if(!flag)
				{
					Playableboard[i *8 + j] = 0;
				}

			}
			
		}
	}
	else
	{
		for( i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				int flag = 0;
				if(Chessboard[i * 8 + j] == BLANK)
				{	
					/*Judge left and right*/			
					if(j && Chessboard[i * 8 + j - 1] == WHITE)
						for( k = j - 2; k >= 0; k--)
						{
							if(Chessboard[i * 8 + k] == WHITE)
								continue;
							else if(Chessboard[i * 8 + k] == BLACK)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(j < 7 && Chessboard[i * 8 + j + 1] == WHITE)
						for( k = j + 2; k < 8; k++)
						{
							if(Chessboard[i * 8 + k] == WHITE)
								continue;
							else if(Chessboard[i * 8 + k] == BLACK)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					/*Judge up and down*/
					if(i && Chessboard[(i - 1) * 8 + j] == WHITE)
						for( k = i - 2; k >= 0; k--)
						{
							if(Chessboard[k * 8 + j] == WHITE)
								continue;
							else if(Chessboard[k * 8 + j] == BLACK)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i * 8 + j] = 1;
						continue;
					}
					if(i < 7 && Chessboard[(i + 1) * 8 + j] == WHITE)
						for( k = i + 2; k < 8; k++)
						{
							if(Chessboard[k * 8 + j] == WHITE)
								continue;
							else if(Chessboard[k * 8 + j] == BLACK)
							{
								flag = 1;
								break;
							}	
							else /*get blank*/
								break;
						}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					/*Judge oblique line*/
					if(i < 7 && j < 7 && Chessboard[(i + 1) * 8 + j + 1] == WHITE)
					{
						for( k = 2; k + i < 8 && k + j < 8; k++ )
						{
							if(Chessboard[(k+i)*8 + (k+j)] == WHITE)
								continue;
							else if(Chessboard[(k+i)*8 + (k+j)] == BLACK)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i < 7 && j > 0 && Chessboard[(i + 1) * 8 + j - 1] == WHITE)
					{
						for( k = 2; k + i < 8 && j - k  >= 0; k++ )
						{
							if(Chessboard[(k+i)*8 + (j - k)] == WHITE)
								continue;
							else if(Chessboard[(k+i)*8 + (j - k)] == BLACK)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i > 0 && j < 7 && Chessboard[(i - 1) * 8 + j + 1] == WHITE)
					{
						for( k = 2; i - k >= 0 && k + j < 8; k++ )
						{
							if(Chessboard[(i - k)*8 + (k+j)] == WHITE)
								continue;
							else if(Chessboard[(i - k)*8 + (k+j)] == BLACK)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
					if(i > 0 && j > 0 && Chessboard[(i - 1) * 8 + j - 1] == WHITE)
					{
						for( k = 2; i - k >= 0 && j - k >= 0; k++ )
						{
							if(Chessboard[(i-k)*8 + (j-k)] == WHITE)
								continue;
							else if(Chessboard[(i-k)*8 + (j-k)] == BLACK)
							{
								flag = 1;
								break;
							}
							else
								break;
						}
					}
					if(flag)
					{
						Playableboard[i *8 + j] = 1;
						continue;
					}
				}
				if(!flag)
				{
					Playableboard[i *8 + j] = 0;
				}

			}
			
		}
	}
}
void DrawChessboard()
{
	/*
	*/
	putchar('\n');
	int i,j;
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(Chessboard[i * 8 + j] == BLANK)
				printf(" + ");
			else if(Chessboard[i *8 +j] == BLACK)
				printf("⚫ ");
			else if(Chessboard[i* 8 +j] == WHITE)
				printf("⚪ ");
		}
		putchar('\n');
		putchar('\n');
	}
	return;
}

void DrawPlayableboard()
{
	putchar('\n');
	int i,j;
	printf("  0 1 2 3 4 5 6 7\n");
	for(i = 0; i < 8; i++)
	{
		printf("%d ", i);
		for(j = 0; j < 8; j++)
		{
			if(Playableboard[i * 8 + j] == 0)
				printf("0 ");
			else if(Playableboard[i *8 +j] == 1)
				printf("1 ");
		}
		putchar('\n');
	}
	return;
}

int IfGameOver(int Chessboard_A[64])
{
	int i;
	/*judge if all ChessBoard is over*/
	for(i = 0; i < 64; i++)
	{
		if(Chessboard_A[i] == BLANK)
			break;
	}
	if(i == 64) return 1;
	/*judge if white win*/
	for(i = 0; i < 64; i++)
	{
		if(Chessboard_A[i] == BLACK) 
			break;
	}
	if(i == 64) return 1;
	/*judge if black win*/
	for(i = 0; i < 64; i++)
	{
		if(Chessboard_A[i] == WHITE)
			break;
	}
	if(i == 64) return 1;
	memcpy(Chessboard,Chessboard_A,sizeof(int)*64);
	RefreshPlayableboard(BLACK);
	for(i = 0; i < 64; i++)
	{
		if(Playableboard[i] == 1)
			break;
	}
	if(i == 64)
	{
		RefreshPlayableboard(WHITE);
		for(i = 0; i < 64; i++)
		{
			if(Playableboard[i] == 1)
				break;
		}
		if(i == 64)
			return 1;
	}
	return 0;
}
