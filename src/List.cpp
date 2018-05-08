#include "List.h"

List::List() {
	head = NULL;
	curr = NULL;
	temp = NULL;
	prev = NULL;
	tail = NULL;
	state_head = NULL;
	state_tail = NULL;
	state_curr = NULL;
	no_of_states = 0;
}


List::state* List::AddOperation(vector<List::node*> &a,int start, int end,int cycle,string cond, int if_num, int else_num, string statenum)
{
	stateptr s = new state;
	s->next_state = NULL;
	if (state_head != NULL)
	{
		state_curr = state_head;
		while (state_curr->next_state != NULL)
		{
			state_curr = state_curr->next_state;
		}
		state_curr->next_state = s;
	}
	else
	{
		state_head = s;
	}
	s->cycle = cycle;
	s->if_number = if_num;
	s->else_number = else_num;
	s->type_condition.push_back(cond);
	s->if_branch = 0;
	s->state_num = statenum;
	s->next_if = NULL;
	s->next_else = NULL;
	s->succ_state = NULL;
	for (int i = start; i <= end; i++)
	{
		s->operation_ptr.push_back(a[i]);
	}
	return s;
}

void List::printstate()
{
	state_curr = state_head;
	while (state_curr!= NULL)
	{
		cout << endl<<state_curr << " | "<<state_curr->state_num << " | " << state_curr->cycle;
		for (int i = 0; i<state_curr->operation_ptr.size(); i++)
		{
			cout << " | " << state_curr->operation_ptr[i]->vertex_name;
		}
		for (int i = 0; i<state_curr->type_condition.size(); i++)
		{
			cout << " | " << state_curr->type_condition[i];
		}
		cout << " |  if-loop: " << state_curr->if_number;
		cout << " |  else-loop: " << state_curr->else_number;
		if (state_curr->succ_state != NULL)
		{
			cout << " |  next-state: " << state_curr->succ_state->state_num;
		}
		if (state_curr->next_if!=NULL)
		{
			cout << " |  if-next-state: " << state_curr->next_if->state_num;
		}
		if (state_curr->next_else!=NULL)
		{
			cout << " |  else-next-state: " << state_curr->next_else->state_num;
		}
		cout << "| if branch: " << state_curr->if_branch;
		


		state_curr = state_curr->next_state;
	}
	cout << endl;
}


 List::node* List::AddVertex(int data,int latency, string operation,string output,string input1,string input2,string input3, int num1, int num2,string argument) {
	nodePtr n = new node;
	n->next_vertex = NULL;
	n->previous_vertex = NULL;
	n->pre_force = 0;
	n->succ_force=0;
	n->self_force=0;
	n->latency = latency;
	n->output = output;
	n->input1 = input1;
	n->input2 = input2;
	n->input3 = input3;
	n->time_cycle_start = NULL;
	n->time_cycle_end = NULL;
	n->time_cycle_start_ALAP = NULL;
	n->time_cycle_end_ALAP = NULL;
	n->ASAP_Scheduled = false;
	n->ALAP_Scheduled = false;
	n->scheduled = false;
	n->vertex_name = data;
	n->sign_operation = operation;
	n->if_loop_number = num1;
	n->else_loop_number = num2;
	n->if_variable = 0;
	n->if_scheduled = false;
	if (head != NULL)
	{
		curr = head;
		while (curr->next_vertex != NULL)
		{
			curr = curr->next_vertex;
		}
		n->previous_vertex = curr;
		curr->next_vertex = n;
	}
	else
	{
		head = n;
	}
	//this is to store the operation
	if (operation == "*")
		n->operation="MULT";
	else if (operation == "+" || operation == "-")
		n->operation="ALU";
	else if (operation == "/" || operation == "%")
		n->operation= "Modulo/Divider";
	else if (operation == ">" || operation == "<" || operation == "==" || operation == ">>" || operation == "<<" || operation == "?")
		n->operation= "logical";
	else
		n->operation= "";
	no_of_states++;
	return n;
}

void List::PrintList() {
	curr = head;
	while (curr != NULL)
	{
		cout << curr;
		cout << " |" << curr->vertex_name << " |" << curr->operation << " | " << curr->latency;
		cout << " S&E ASAP : " << curr->time_cycle_start<<" | "<<curr->time_cycle_end;
		cout << " Predeccessor Pointer ";
		for (int i = 0; i < curr->prev_vertex.size(); i++)
		{
			cout << " | " << curr->prev_vertex[i]->vertex_name;
		}
		cout << " | " << curr->output << " | " << curr->input1 << " | " << curr->input2 << " | " << curr->input3;
		cout << " Successor Pointer ";
		for (int i = 0; i < curr->succ_vertex.size(); i++)
		{
			cout << " | " << curr->succ_vertex[i]->vertex_name;
		}
		
		cout << " S&E ALAP : " << curr->time_cycle_start_ALAP << " | " << curr->time_cycle_end_ALAP;
		cout << " | " << curr->ALAP_Scheduled << "  | " << curr->ASAP_Scheduled;
		cout<<" | if num: "<<curr->if_loop_number<<" | else num: "<<curr->else_loop_number<<" | if branch: "<<curr->if_variable<<endl;
		curr = curr->next_vertex;
	}
}
//ASAP timecycle
void List::Add_timecycle(int start, int end,struct node *current) {
	curr = current;
	curr->time_cycle_start = start;
	curr->time_cycle_end = end;
}

int List::ASAP_schedule(int start,int end,bool schd, struct node *current)
{
	
		current->time_cycle_start = start;
		current->time_cycle_end = end;
		current->ASAP_Scheduled = schd;
		return 0;
	
}
int List::ALAP_schedule(int start, int end, struct node *current,bool schd)
{

	current->time_cycle_start_ALAP= start;
	current->time_cycle_end_ALAP = end;
	current->ALAP_Scheduled = true;
	return 0;

}

void List::AddpreviousPtr(string input1, string input2, string input3,string input4,string if_else,int loop_ref)
{
	curr = head;
	while (curr != NULL)
	{
		temp = curr;
		curr = curr->next_vertex;
	}

	
	curr = temp;

	bool input01 = true;
	bool input02 = true;
	bool input03 = true;
	bool input04 = true;

	
	
	//make a pointer that point to the successor vertex and increment it when you find a previous vertex
	if (curr != head)
	{
		curr = curr->previous_vertex;
	}
	
	if (if_else == "if" || if_else == "none")
	{
		while (curr != head)
		{
			if (curr->output == input1 && input01 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				//enter the prev_vertex as a succerssor vertex to current
				input01 = false;
			}
			else if (curr->output == input2&& input02 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input02 = false;
			}
			else if (curr->output == input3 && input03 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input03 = false;
			}
			else if (curr->output == input4 && input03 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input04 = false;
			}
			curr = curr->previous_vertex;
		}
	}
	else if (if_else == "else")
	{

		while (curr != head)
		{
			if (curr->output == input1 && curr->if_loop_number != loop_ref && input01 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input01 = false;
			}
			else if (curr->output == input2  && curr->if_loop_number != loop_ref && input02 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input02 = false;
			}
			else if (curr->output == input3  && curr->if_loop_number != loop_ref && input03 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input03 = false;
			}
			else if (curr->output == input4  && curr->if_loop_number != loop_ref && input04 == true)
			{
				temp->prev_vertex.push_back(curr);
				curr->succ_vertex.push_back(temp);
				input04 = false;
			}
			curr = curr->previous_vertex;
		}
			
	}
	
}

