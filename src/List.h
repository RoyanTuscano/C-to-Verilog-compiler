#ifndef Lists
#define Lists
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class List {
public:

	typedef struct node {
		int vertex_name;
		string operation;
		string sign_operation;
		string output;
		string input1;
		string input2;
		string input3;
		int latency;
		int time_cycle_start;	//ASAP cycle start
		int time_cycle_end;		//ASAP cycle end
		int time_cycle_start_ALAP;		
		int time_cycle_end_ALAP;
		bool ASAP_Scheduled;
		bool ALAP_Scheduled;
		int if_loop_number;
		int else_loop_number;
		vector<float> Prob;			//FDS propability for a vertex
		node* next_vertex;			//next
		node* previous_vertex;		//previous
		vector<node*> succ_vertex;
		vector<node*> prev_vertex;
		float self_force;		//FDS 
		float pre_force;
		float succ_force;
		int schedulecycle;
		bool scheduled;
		int if_variable;
		bool if_scheduled;
		int statenumber;
		//node *nextstate;
	}*nodePtr;

	nodePtr head;
	nodePtr curr;
	nodePtr prev;
	nodePtr temp;
	nodePtr tail;
	int t_latency;
	vector<bool> ifloops;
	vector<float> ALU_Prob;
	vector<float> MULTI_Prob;
	vector<float> DIVMOD_Prob;
	vector<float> LOGIC_Prob;

	typedef struct state {
		int cycle;
		string state_num;
		vector<node*> operation_ptr;
		state* succ_state;
		state * next_if;
		state *next_else;
		//vector<state*> nextif;
		//vector<state*>nextelse;
		state* next_state;
		int if_number;
		int else_number;
		int if_branch;
		vector<string> type_condition;
	}*stateptr;
	stateptr state_head;
	stateptr state_tail;
	stateptr state_curr;
	int no_of_states;

public:
	List();
	state* AddOperation(vector<List::node*> &a, int start, int end, int cycle, string cond, int if_num, int else_num, string statenum);
	void printstate();
	node* AddVertex(int data,int latency, string operation,string output,string input1,string input2,string input3, int num1, int num2,string argument);
	//void Delete_vertex(int deldata);
	void PrintList();
	void Add_timecycle(int start, int end, struct node *current);
	int ASAP_schedule(int start, int end, bool schd, struct node *current);
	int ALAP_schedule(int start, int end, struct node *current, bool schd);
	void AddpreviousPtr(string input1, string input2,string input3,string input4,string if_else, int ref_loop);

};

#endif
