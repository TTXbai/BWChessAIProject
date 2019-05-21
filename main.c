#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "PlayGame.h"
#include "AI.h"
#include "main.h"


void pve_play()
{
	int turn = BLACK;
	int row,line;
	int i;
	int input;
	init();
	InitChessboard();
	int tempChessboard[64];
	int tempPlayableboard[64];
	//turn_flag = BLACK;
	printf("Please input the AI side: 0 for Black and others for WHITE\n");
	scanf("%d", &input);
	if(input)
		turn_flag = WHITE;
	else
		turn_flag = BLACK;
	while(1)
	{
		DrawChessboard();
		if(turn == BLACK)
		{
			RefreshPlayableboard(turn);
			for(i = 0; i < 64; i++)if(Playableboard[i])break;
			if(i == 64)
			{
				turn = WHITE;
				continue;
			}
			DrawPlayableboard();
			if(turn_flag == BLACK)
			{
				memcpy(tempChessboard, Chessboard, sizeof(int) * 64);
				memcpy(tempPlayableboard, Playableboard, sizeof(int) * 64);
				AI_start(Chessboard, Playableboard, turn, "constant.txt");
				memcpy(Chessboard,tempChessboard,sizeof(int) * 64);
				memcpy(Playableboard, tempPlayableboard, sizeof(int) * 64);
			}
			printf("It is BLACK turn, please input row and line:\n");
			scanf("%d %d",&row,&line);
			if(PlayChess(row,line,turn))
			{
				turn = WHITE;
				runtime++; 
			}
		}
		else if(turn == WHITE)
		{
			RefreshPlayableboard(turn);
			for(i = 0; i < 64; i++)if(Playableboard[i])break;
			if(i == 64)
			{
				turn = BLACK;
				continue;
			}
			DrawPlayableboard();
			if(turn_flag == WHITE)
			{
				memcpy(tempChessboard, Chessboard, sizeof(int) * 64);
				memcpy(tempPlayableboard, Playableboard, sizeof(int) * 64);
				AI_start(Chessboard, Playableboard, turn,"constant.txt");
				memcpy(Chessboard,tempChessboard,sizeof(int) * 64);
				memcpy(Playableboard, tempPlayableboard, sizeof(int) * 64);
			}
			printf("It is WHITE turn, please input row and line:\n");
			scanf("%d %d",&row,&line);
			if(PlayChess(row,line,turn))
			{
				turn = BLACK;
				runtime++; 
			}
		}
		if(IfGameOver(Chessboard))
		{
			printf("GameOver\n");
			exit(0);
		}

	}
}

void eve_train()
{
	init();
	gen_random_config();
	//RefreshConfig();
	//gen_random_config();
	int tempChessboard[64];
	int tempPlayableboard[64];
	int turn = BLACK;
	int row,line;
	int times = 1;
	int i;
	int play;
	int status;
	int fd[2],nbytes;
	int *write_fd = &fd[1];
    int *read_fd = &fd[0];
	int result = pipe(fd);
	int Wincount = 0;
	while(1)
	{
		runtime = 0;
		pid_t pid = fork();
		if(pid == 0)
		{
			InitChessboard();
			int turn = BLACK;
			while(1)
			{
				//DrawChessboard();
				if(turn == BLACK)
				{
					RefreshPlayableboard(turn);
					for(i = 0; i < 64; i++)if(Playableboard[i])break;
					if(i == 64)
					{
						turn = WHITE;
						turn_flag = WHITE;
						continue;
					}
					//DrawPlayableboard();
					memcpy(tempChessboard, Chessboard, sizeof(int) * 64);
					memcpy(tempPlayableboard, Playableboard, sizeof(int) * 64);
					if(times % 2 == 0)
						play = AI_start(Chessboard, Playableboard, turn, "constant.txt");
					else
						play = AI_start(Chessboard, Playableboard, turn, "constant_temp.txt");
					memcpy(Chessboard,tempChessboard,sizeof(int) * 64);
					memcpy(Playableboard, tempPlayableboard, sizeof(int) * 64);
					if(PlayChess(play / 8,play % 8,turn))
					{
						turn = WHITE;
						runtime++; 
					}
				}
				else if(turn == WHITE)
				{
					RefreshPlayableboard(turn);
					for(i = 0; i < 64; i++)if(Playableboard[i])break;
					if(i == 64)
					{
						turn = BLACK;
						turn_flag = BLACK;
						continue;
					}
					//DrawPlayableboard();
					memcpy(tempChessboard, Chessboard, sizeof(int) * 64);
					memcpy(tempPlayableboard, Playableboard, sizeof(int) * 64);
					if(times % 2 == 1)
						play = AI_start(Chessboard, Playableboard, turn, "constant.txt");
					else
						play = AI_start(Chessboard, Playableboard, turn, "constant_temp.txt");
					memcpy(Chessboard,tempChessboard,sizeof(int) * 64);
					memcpy(Playableboard, tempPlayableboard, sizeof(int) * 64);
					if(PlayChess(play / 8,play % 8,turn))
					{
						turn = BLACK;
						runtime++; 
					}
				}
				if(IfGameOver(Chessboard))
				{
					int winner = JudgeWinner(Chessboard);
					DrawChessboard();
					printf("GameOver\n");
					printf("Winner is %d\n", winner);
					char string[3];
					string[0] = winner + 0x31;
					string[1] = 0;
					result = write(*write_fd,string,strlen(string));
					break;
				}

			}
			printf("running %d time\n", times);
			exit(0);
		}
		else
		{
			waitpid(-1,&status,0);
			char readbuffer[10];
			nbytes = read(*read_fd,readbuffer,sizeof(readbuffer)-1);
			int Winner = atoi(readbuffer);
			if(Winner == 0)//White win
			{
				Wincount--;
				Wincount = -1 * Wincount;
			}
			else if(Winner == 2)//Black win
			{
				Wincount++;
				Wincount = -1 * Wincount;
			}
			else
			{
				Wincount = -1 * Wincount;
			}
			if(Wincount >= 3 && times % 2 == 1 || Wincount <= -3 && times % 2 == 0)
			{
				printf("Temp config wins\n");
				RefreshConfig();
				gen_random_config();
				Wincount = 0;
				times = 0;
			}
			if(Wincount >= 3 && times % 2 == 0 || Wincount <= -3 && times % 2 == 1)
			{
				printf("Origin config wins\n");
				gen_random_config();
				Wincount = 0;
				times = 0;
			}
			if(times >= 6)
			{
				gen_random_config();
				Wincount = 0;
				times = 0;
			}
			printf("DEBUG: Wincount = %d\n", Wincount);
			times++;
		}

	}
}

void gen_random_config()
{
	FILE* fp;
	fp = fopen("constant_temp.txt","w");
	float rand_constant;
	int i, seed;
	for(i = 0; i < 64; i++)
	{
		seed = rand()%10000;
		rand_constant = (float)seed /10000.0;
		fprintf(fp,"%f\n",rand_constant);
	}
	fclose(fp);
	return;
}

void RefreshConfig()
{
	FILE* fp;
	fp = fopen("constant_temp.txt","r");
	//fp2 = fopen("constant.txt","w");
	float Constant[64];
	int ch, i = 0, n = 0, k =0;
	char str[10];
	//printf("Reading config\n");
	while((ch = fgetc(fp)) != EOF) {
        if(ch == '\t' || ch == '\n') {
            if(i > 0) {
                str[i++] = '\0';
                Constant[k++] = atof(str);
                i = 0;
                ++n;
            }
        }
        else str[i++] = ch;
    }
	//printf("Reading config terminated.\n");
	fclose(fp);
	fp = fopen("constant.txt","w");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp,"%f\n",Constant[i]);
	}
	fclose(fp);
	return;
}

int main()
{
	printf("Welcome to Reversi AI created by TTX\n");
	printf("Which mode do you want to play? 0 for pve, others for training\n");
	int input;
	scanf("%d", &input);
	if(input)
	{
		eve_train();
	}
	else
	{
		pve_play();
	}
	return 0;
}

