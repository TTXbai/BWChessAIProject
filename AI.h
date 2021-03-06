#define PROCESS_TIME	100000

#define CONTROL_VALUE	1
#define INFINITE	0x1000000


extern int turn_flag;
extern int runtime;
extern int Playableboard[64];
extern int Chessboard[64];

struct PlayNode
{
	int Playableboard[64];
	int Chessboard[64];
	int PlayableNum;
	float value;
	int frequency;
	int turn;
	int runtime;
	struct PlayNode* Father;
	struct PlayNode* Random_Child;
	struct PlayNode** Child;
};

float judgeValue(int Chessboard[64]);
float judgeValue2(int Chessboard[64],int PlayableNum,float Constant);

/*
1. create the root node of search tree
2. create the tree nodes by PlayableBoard 
3. Using AI_process to proces the search
4. Choose the maximun value to return
*/
int AI_start(int Chessboard[64], int Playableboard[64], int turn, char* config);

/*
1.search for PROCESS_TIME, each time from the root to choose the maximum critical value
2.repeat this process until no child
	If the leaf node have been accessed (f > 0), expand the search tree and continue repeat.
3.the leaf node should get a random number to process the last chessboard situation
4.reverse_passing the node and finally update each node

*/
void AI_process(struct PlayNode* root, char* config);

/*
1.Get the message from PlayNode p
2.Continually get random number and get random situation of the Chessboard
	Create a new node to repersent the game_state
	If the Game is end, stop and reverse_passing
*/
struct PlayNode* Random_get_final_PlayNode(struct PlayNode* p);

/*
update the f = f+1, value = value + child_value
	until the father is NULL
*/
void reverse_passing(struct PlayNode* p,float passing_value);

void reverse_passing2(struct PlayNode*p, float passing_value, float Constant[64]);


/*
Expand the search tree according to Playableboard
*/
void expand_search_tree(struct PlayNode* p);

struct PlayNode* New_PlayNode(int Chessboard[64], int Playableboard[64], int turn, struct PlayNode* Father);

int Print_choice(struct PlayNode* p);

float Calculate_value(struct PlayNode* p);


struct PlayNode* Get_Max_Value_Of_Node(struct PlayNode* root);

void Free_all(struct PlayNode* p);

/*init rand message*/
void init();

void Init_Constant(char *config, float Constant[64]);