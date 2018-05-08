#include<iostream>
#include "ToVerilog.h"
#include "List.h"

void To_verilog(List *Roy, vector<string> &b, vector<string> &c, struct datanode *node)
{
	List::node *current;
	List::state *current_state;
	int state = 1;
	vector<List::node*> if_hold_loops;
	vector<List::node*> else_hold_loops;
	vector<List::node*> simple_hold;
	c.reserve(Roy->tail->vertex_name - 2);
	int i = 0;
	int in_cycle = 1;

	current_state=Roy->AddOperation(simple_hold, 0, -1, 0, "if",0,0,"wait");
	
	while (in_cycle <= Roy->t_latency)
	{

		current = Roy->head;
		current = current->next_vertex;
		while (current != Roy->tail)
		{
			if ((in_cycle >= current->schedulecycle) && (in_cycle <= (current->schedulecycle + current->latency - 1)))
			{

				if (current->if_loop_number == 0 && current->else_loop_number == 0)
				{
					simple_hold.push_back(current);
				}
				else if (current->if_loop_number != 0)
				{
					//holds if conditions
					if_hold_loops.push_back(current);
				}
				else if (current->else_loop_number != 0)
				{
					//holds else condition
					else_hold_loops.push_back(current);
				}
			}
			current = current->next_vertex;
		}
		
		
		if (if_hold_loops.size() != 0 || else_hold_loops.size() != 0)
		{
			sort_loop(if_hold_loops, "if");
			sort_loop(else_hold_loops, "else");
		}
		int start = 0;
		int temp = 1;
		for (int k = 0; k < if_hold_loops.size(); k++)
		{
			if (k + 1 < if_hold_loops.size())
			{
				if (if_hold_loops[k]->if_loop_number != if_hold_loops[k + 1]->if_loop_number)
				{
					current_state = Roy->AddOperation(if_hold_loops, start, k, in_cycle, "if", if_hold_loops[k]->if_loop_number,0,"S"+to_string(state++));

					add_simple(simple_hold, current_state);
					start = k + 1;
				}
			}
			else
			{
				current_state = Roy->AddOperation(if_hold_loops, start, k, in_cycle, "if", if_hold_loops[k]->if_loop_number,0, "S"+to_string(state++));
				add_simple(simple_hold, current_state);
			}
				
		}
		start = 0;
		for (int k = 0; k < else_hold_loops.size(); k++)
		{

			if (k + 1 < else_hold_loops.size())
			{
				if (else_hold_loops[k]->else_loop_number != else_hold_loops[k + 1]->else_loop_number)
				{
					current_state = Roy->AddOperation(else_hold_loops, start, k, in_cycle, "else",0, else_hold_loops[k]->else_loop_number, "S"+to_string(state++));
					add_simple(simple_hold, current_state);
					start = k + 1;
				}
			}
			else
			{
				current_state = Roy->AddOperation(else_hold_loops, start, k, in_cycle, "else", 0, else_hold_loops[k]->else_loop_number, "S"+to_string(state++));
				add_simple(simple_hold, current_state);
			}
		}
		start = 0;
		if (if_hold_loops.size() == 0 || else_hold_loops.size() == 0)
		{
			if (start < simple_hold.size())
			{
				Roy->AddOperation(simple_hold, start, simple_hold.size() - 1, in_cycle, "none",0,0, "S"+to_string(state++));
			}
	
		}

		
		if_hold_loops.clear();
		else_hold_loops.clear();
		simple_hold.clear();
		in_cycle++;
	}

	current_state=Roy->AddOperation(simple_hold, 0, -1, in_cycle, "none", 0, 0,"Final");
	Roy->state_tail = current_state;
	current_state->succ_state = Roy->state_head;
	current_state->operation_ptr.push_back(Roy->tail);
	//code for connecting states 
	//initialize the wait state;
	current_state = Roy->state_head;
	current_state->type_condition.push_back("if");
	current_state->operation_ptr.push_back(Roy->head);
	current_state->next_if = current_state->next_state;
	current_state->next_else = current_state;
	//move to next state
	current_state= current_state->next_state;
		while (current_state != Roy->state_tail)
		{
			//pass the current state and check its operation
			find_next_states(Roy,current_state);
			current_state = current_state->next_state;
		}
	

	//Roy->printstate();
}
void find_next_states(List* Roy,List::state* current)
{
	int time_cycle=current->cycle;
	vector<List::state*> holdstates; //find the states on same cycle
	//see if the state contains any if branch ie comparator
	bool check = false;
	for (int i = 0; i < current->operation_ptr.size(); i++)
	{
		if (current->operation_ptr[i]->if_variable != 0 || current->operation_ptr[i]->else_loop_number !=0 || current->operation_ptr[i]->if_loop_number != 0)
		{
			if (current->operation_ptr[i]->if_variable != 0)
			{
				current->if_branch = current->operation_ptr[i]->if_variable;
			}
			check = true;
		}
	}
	time_cycle++;
	if (current->if_branch != 0)
	{
		current->next_if = successor_state(Roy, current, time_cycle, current->if_branch, 0);
		current->next_else = successor_state(Roy, current, time_cycle, 0, current->if_branch); \

	}
	else if (current->if_number != 0 && current->else_number == 0)
	{
	
		//find the successor if
		current->next_if = successor_state(Roy, current, time_cycle, current->if_number, 0);
		//find the successor else
	
	}
	else if (current->if_number == 0 && current->else_number != 0)
	{
		current->next_else = successor_state(Roy, current, time_cycle, 0, current->else_number);
	}
	else
	{
		//check the states in the next cycle and connect to the state with no if and else condition
		
		current->succ_state = successor_state(Roy, current, time_cycle,0,0);
	}


}
List::state* successor_state(List* Roy, List::state* curren,int time_cycle, int if_loop_number, int else_loop_number)
{
	List::state* temp = curren->next_state;
	List::state* current = curren;
	while (time_cycle <= Roy->t_latency)
	{
		while (current != Roy->state_tail)
		{

			if (current->cycle == time_cycle)
			{
				if (current->else_number == 0 && current->if_number == 0 && if_loop_number==0 && else_loop_number==0)
				{
					return current;
				}
				else if (current->else_number == else_loop_number && else_loop_number >0)
				{
					return current;
				}
				else if (current->if_number == if_loop_number && if_loop_number > 0)
				{
					return current;
				}
			}
			current = current->next_state;
		}
		time_cycle++;
		current = temp;
	}
	return Roy->state_tail;
}

void add_simple(vector<List::node*> &a, List::state* curren)
{
	for (int i = 0; i < a.size(); i++)
	{
		curren->operation_ptr.push_back(a[i]);
	}
}
void sort_loop(vector<List::node*> &a,string if_else)
{
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = i + 1; j < a.size(); j++)
		{
			if (if_else == "if")
			{
				if (a[j]->if_loop_number < a[i]->if_loop_number)
				{
					a.insert(a.begin() + i, a[j]);
					a.erase(a.begin() + j + 1);
				}
			}
			else
			{
				if (a[j]->else_loop_number < a[i]->else_loop_number)
				{
					a.insert(a.begin() + i, a[j]);
					a.erase(a.begin() + j + 1);
				}

			}
			
		}
	}
}


 


