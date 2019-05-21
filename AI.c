#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "AI.h"


int turn_flag;
int runtime;
double total_time;
int Playableboard[64];
int Chessboard[64];

//weight
const int weight[64] = {
	400,-25,100,100,100,100,-25,400,
	-25,-50,5,5,5,5,-50,-25,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	100,5,1,1,1,1,5,100,
	-25,-50,5,5,5,5,-50,-25,
	400,-25,100,100,100,100,-25,400
};

/*Easily judge first, and should be update*/  
float judgeValue(int Chessboard[64])
{
	float value = 0;
	for (int i = 0; i < 64; ++i)
	{
		value += Chessboard[i];
	}
	return value * turn_flag;
}

float judgeValue2(int Chessboard[64],int PlayableNum,float Constant)
{
	float value = 0;
	for (int i = 0; i < 64; ++i)
	{
		value += Chessboard[i] * weight[i];
	}
	value = value * Constant + (float)PlayableNum * turn_flag * 10 * (1 - Constant);
	if (turn_flag == 1)
		return value;
	else
		return value * (-1.0);
}


/*
1. create the root node of search tree
2. create the tree nodes by PlayableBoard 
3. Using AI_process to proces the search
4. Choose the maximun value to return
*/
int AI_start(int Chessboard[64], int Playableboard[64], int turn, char* config)
{
	struct PlayNode* root = New_PlayNode(Chessboard, Playableboard, turn, NULL);
	//printf("Debug: AI start\nFirst step: expand_search_tree...\n");
	expand_search_tree(root);
	//printf("Debug: Second step: AI_process...\n");
	AI_process(root,config);
	int num = Print_choice(root);
	Free_all(root);
	return num;
}

/*
1.search for PROCESS_TIME, each time from the root to choose the maximum critical value
2.repeat this process until no child
	If the leaf node have been accessed (f > 0), expand the search tree and continue repeat.
3.the leaf node should get a random number to process the last chessboard situation
4.reverse_passing the node and finally update each node
*/
void AI_process(struct PlayNode* root, char* config)
{
	int i, times;
	clock_t start,end;
	struct PlayNode* temp,*final;
	float Constant[64];
	Init_Constant(config, Constant);
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
		reverse_passing2(temp, judgeValue2(temp->Chessboard,temp->PlayableNum, Constant[temp -> runtime]) , Constant);
		//added a new way to balance the random result, while at the last will not determine with it
		
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
void reverse_passing(struct PlayNode* p,float passing_value)
{
	if(p -> Father == NULL)
		return;
	if(p -> Random_Child == NULL && p -> Child == NULL)
	{
		float value = judgeValue(p->Chessboard);
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

void reverse_passing2(struct PlayNode*p, float passing_value, float Constant[64])
{
	//printf("passing value = %lf\n",passing_value);
	if(p -> Father == NULL)
		return;
	if(p -> Child == NULL)
	{
		//printf("runtime = %d\n",p -> runtime);
		float value = judgeValue2(p->Chessboard,p->PlayableNum, Constant[p -> runtime]);
		p -> Father -> value += value;
		reverse_passing2(p -> Father, value, Constant);
	} 
	else
	{
		p -> Father -> value += passing_value;
		reverse_passing2(p -> Father, passing_value, Constant);
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
	if( p -> Father)
	{
		p -> runtime = p->Father->runtime+1;
	}
	else
	{
		p -> runtime = runtime;
	}
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

int Print_choice(struct PlayNode* p)
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
	printf("Choose the %d child, score = %lf\n", index, score);
	for(i = 0; i < 64; i++)
	{
		if(p -> Playableboard[i] == 1)
			index--;
		if(index < 0)
			break;
	}
	printf("Total time = %f s\n", total_time);
	return i;
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
	else if(p -> PlayableNum > 0)
	{
		for(i = 0; i < p -> PlayableNum; i++)
			Free_all(p -> Child[i]);
		free(p);
		return;
	}
	else 
	{
		Free_all(p -> Child[0]);
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

void Init_Constant(char *config, float Constant[64])
{
	FILE *fp;
	fp = fopen(config, "rt");
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
	return;
}