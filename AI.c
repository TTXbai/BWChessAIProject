#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "AI.h"
#include "PlayGame.h"

int turn_flag;
int runtime;
double total_time;
//weight
const int weight[64] = {
	2000,-25,100,100,100,100,-25,2000,
	-25,-50,5,5,5,5,-50,-25,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	-25,-50,5,5,5,5,-50,-25,
	2000,-25,100,100,100,100,-25,2000
};

/*Easily judge first, and should be update*/  
int judgeValue(int Chessboard[64])
{
	int value = 0;
	for (int i = 0; i < 64; ++i)
	{
		value += Chessboard[i];
	}
	return value * turn_flag;
}

int judgeValue2(int Chessboard[64])
{
	int value = 0;
	for (int i = 0; i < 64; ++i)
	{
		value += Chessboard[i] * weight[i];
	}
	return value * turn_flag;
}


/*
1. create the root node of search tree
2. create the tree nodes by PlayableBoard 
3. Using AI_process to proces the search
4. Choose the maximun value to return
*/
void AI_start(int Chessboard[64], int Playableboard[64], int turn)
{
	struct PlayNode* root = New_PlayNode(Chessboard, Playableboard, turn, NULL);
	//printf("Debug: AI start\nFirst step: expand_search_tree...\n");
	expand_search_tree(root);
	//printf("Debug: Second step: AI_process...\n");
	AI_process(root);
	Print_choice(root);
	Free_all(root);
	return;
}

/*
1.search for PROCESS_TIME, each time from the root to choose the maximum critical value
2.repeat this process until no child
	If the leaf node have been accessed (f > 0), expand the search tree and continue repeat.
3.the leaf node should get a random number to process the last chessboard situation
4.reverse_passing the node and finally update each node
*/
void AI_process(struct PlayNode* root)
{
	int i, times;
	clock_t start,end;
	struct PlayNode* temp,*final;
	//printf("Debug: Start loop\n");
	start = clock();
	for(times = 0; times < PROCESS_TIME; times++)
	{
		
		temp = Get_Max_Value_Of_Node(root);
		//printf("Debug: Found Max_value of search tree\n");
		if(temp -> frequency != 0)
		{
			//Only expand once even it expand fails
			expand_search_tree(temp);
			temp = Get_Max_Value_Of_Node(root);
		}
		//printf("Debug :Start Random_get_final_PlayNode\n");
		final = Random_get_final_PlayNode(temp);
		reverse_passing(final, 0);
		//added a new way to balance the random result, while at the last will not determine with it
		if(runtime <= 45)
		reverse_passing2(temp, 0);
	}
	end = clock();
	total_time += (double)(end-start)/CLOCKS_PER_SEC;
	printf("time cost = %f\n",(double)(end-start)/CLOCKS_PER_SEC);
}


/*
1.Get the message from PlayNode p
2.Continually get random number and get random situation of the Chessboard
	Create a new node to repersent the game_state
	If the Game is end, stop and reverse_passing
*/
struct PlayNode * Random_get_final_PlayNode(struct PlayNode* p)
{
	if(IfGameOver(p -> Chessboard))
	{
		//printf("Debug: It is Game over node\n");
		return p;
	} 
	if(p -> PlayableNum == 0)
	{
		//printf("Debug: Oh ,no way to play\n");
		RefreshPlayableboard(p -> turn * -1);
		//DrawChessboard();
		//DrawPlayableboard();
	}
	else
	{
		int num = rand() % p -> PlayableNum + 1;
		int i, index;
		//Get play index
		for(index = 0, i = 0; index < 64; index++)
		{
			if(p -> Playableboard[index] == 1)
				i++;
			if(i == num)
				break;
		}
		//printf("Debug: Get play index %d\n", index);
		int row = index / 8;
		int line = index % 8;
		// Not yet
		memcpy(Playableboard, p->Playableboard, sizeof(int) * 64);
		memcpy(Chessboard, p-> Chessboard, sizeof(int) * 64);
		PlayChess(row, line, p -> turn);
		RefreshPlayableboard(p -> turn * -1);
		//DrawPlayableboard();
	}
	struct PlayNode* next = New_PlayNode(Chessboard, Playableboard, p->turn * -1, p);
	if(IfGameOver(Chessboard))
	{
		//printf("Debug: GameOver\n");
		return next;
	}
	else
		return Random_get_final_PlayNode(next);
}

/*
update the f = f+1, value = value + child_value
	until the father is NULL
*/
void reverse_passing(struct PlayNode* p,int passing_value)
{
	if(p -> Father == NULL)
		return;
	if(p -> Random_Child == NULL && p -> Child == NULL)
	{
		int value = judgeValue(p->Chessboard);
		p -> value = value;
		p -> frequency = p -> frequency + 1;
		p -> Father -> value += p -> value;
		p -> Father -> frequency += 1;
		reverse_passing(p -> Father, value);
	} 
	else
	{
		p -> Father -> value += passing_value;
		p -> Father -> frequency += 1;
		reverse_passing(p -> Father, passing_value);
	}
	if(p -> Father -> Child == NULL)
		free(p);
	return;
}

void reverse_passing2(struct PlayNode*p, int passing_value)
{
	if(p -> Father == NULL)
		return;
	if(p -> Child == NULL)
	{
		int value = judgeValue2(p->Chessboard);
		p -> Father -> value += value;
		reverse_passing(p -> Father, value);
	} 
	else
	{
		p -> Father -> value += passing_value;
		reverse_passing(p -> Father, passing_value);
	}
	return;
}


/*
Expand the search tree according to PlayableBoard
*/
void expand_search_tree(struct PlayNode* p)
{
	if(p -> Child != NULL)
	{
		//debug
		//printf("Expanded already\n");
		return;
	}
	if(IfGameOver(p->Chessboard))
	{
		//printf("Can't expand already end");
		return;
	}
	//may be problem
	if(p -> PlayableNum == 0)
	{
		//printf("Debug: Expanded PlayableNum zero\n");
		struct PlayNode** Child =(struct PlayNode **)malloc(sizeof(struct PlayNode*) * 1);
		struct PlayNode* temp;
		RefreshPlayableboard(p -> turn * -1);
		temp = New_PlayNode(Chessboard, Playableboard, p->turn * -1, p);
		Child[0] = temp;
		p -> Child = Child;
	}
	else
	{
		struct PlayNode** Child =(struct PlayNode **)malloc(sizeof(struct PlayNode*) * p -> PlayableNum);
		int i, index;
		struct PlayNode* temp;
		for(index = 0, i = 0;index < 64; index++)
		{
			if(p -> Playableboard[index] == 1)
			{
				int row = index / 8;
				int line = index % 8;
				memcpy(Playableboard, p->Playableboard, sizeof(int) * 64);
				memcpy(Chessboard, p-> Chessboard, sizeof(int) * 64);
				PlayChess(row, line, p->turn);
				RefreshPlayableboard(p -> turn * -1);
				temp = New_PlayNode(Chessboard, Playableboard, p->turn * -1, p);
				Child[i++] = temp;
			}
		}
		p -> Child = Child;
	}
	//printf("Debug: Expand successed\n");
	return;
}

struct PlayNode * New_PlayNode(int Chessboard[64], int Playableboard[64], int turn, struct PlayNode* Father)
{
	struct PlayNode* p;
	p = (struct PlayNode*)malloc(sizeof(struct PlayNode));
	memcpy(p -> Playableboard, Playableboard, sizeof(int) * 64);
	memcpy(p -> Chessboard, Chessboard, sizeof(int) * 64);
	int i,count;
	for(i = 0, count = 0;i < 64; i++)
		if(Playableboard[i] == 1)
			count++;
	p -> PlayableNum = count;
	p -> value = 0;
	p -> frequency = 0;
	p -> turn = turn;
	p -> Father = Father;
	p -> Child = NULL;
	p -> Random_Child = NULL;
	return p;
}

float Calculate_value(struct PlayNode* p)
{
	float c = CONTROL_VALUE;
	float left,right,score;
	//printf("Debug: Calculate_value, frequency = %d, value = %d\n", p->frequency, p->value);
	if( p -> frequency == 0)
		return INFINITE;
	else
	{
		left = p -> value / p -> frequency;
		right = 2 * p -> Father -> frequency / p -> frequency;
		score = left + c * sqrt(right);
		return score;
 	}
}

void Print_choice(struct PlayNode* p)
{
	int i;
	float score = -INFINITE, temp;
	int index;
	for(i = 0; i < p-> PlayableNum; i++)
	{
		temp = Calculate_value(p -> Child[i]);
		if(temp > score)
		{
			score = temp;
			index = i;
		}
		if(score == INFINITE)
			break;
	}
	//debug
	//printf("Choose the %d child, score = %lf\n", index, score);
	for(i = 0; i < 64; i++)
	{
		if(p -> Playableboard[i] == 1)
			index--;
		if(index < 0)
			break;
	}
	printf("Row = %d, Line = %d \n", i / 8, i % 8);
	printf("Total time = %f s\n", total_time);
	return;
}

struct PlayNode * Get_Max_Value_Of_Node(struct PlayNode* root)
{
	if(root -> Child == NULL)
		return root;
	float score = -INFINITE, temp;
	int i, index = 0;
	for(i = 0; i < root -> PlayableNum; i++)
	{
		temp = Calculate_value(root -> Child[i]);
		if(temp > score)
		{
			score = temp;
			index = i;
		}
		if(score == INFINITE)
			break;
	}
	//printf("Debug: Choose the %d child of Node\n", index);
	return Get_Max_Value_Of_Node(root -> Child[index]);
}

void Free_all(struct PlayNode* p)
{
	int i;
	if(p -> Child == NULL)
	{
		free(p);
		return;
	}
	else
	{
		for(i = 0; i < p -> PlayableNum; i++)
			Free_all(p -> Child[i]);
		free(p);
		return;
	}
}

/*init rand message*/
void init()
{
	srand((unsigned)(time(NULL)));
	runtime = 0;
	total_time = 0;
	return;
}

/*still no main*/
int main()
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
	printf("Welcome to Reversi AI created by TTX\n");
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
				AI_start(Chessboard, Playableboard, turn);
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
				AI_start(Chessboard, Playableboard, turn);
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
			break;
		}
	}
	return 0;
}
