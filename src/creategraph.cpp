#include<iostream>
#include<string>
#include<vector>
#include"creategraph.h"
#include "allnodes.h"
using namespace std;


int vsize = 0;
int if_loop_number_ref=0;
int else_loop_number_ref = 0;
int if_loops = 0;
int latency(string operation)
{
	if (operation == "*")
		return 2;
		//return 1;
	else if (operation == "+" || operation == "-" )
		return 1;
	else if (operation == "/" || operation == "%")
		return 3;
		//return 1;
	else if (operation == ">" || operation == "<" || operation == "==" || operation == ">>" || operation == "<<" || operation == "?")
		return 1;
	else
		return NULL;
}

int if_condition(List *Roy, vector<string> &b, struct datanode *node, int i)
{
	if_loop_number_ref++;
	else_loop_number_ref++;
	if_loops++;
	List::node *temp;
	temp = Roy->AddVertex(vsize++, 1, "=", b[i + 2], "1", b[i + 2], "", 0, 0, "");
	temp->if_variable = if_loop_number_ref;
	Roy->AddpreviousPtr(b[i + 2], "", "", "", "none", 0);
	string argument = b[i + 2];
	i++;
	while (b[i] != "}")
	{

		if (b[i] == "if")
		{
			i=if_condition(Roy, b, node, i);
			i = i - 2;
		}
		if (b[i] == "=")
		{
			if (b[i + 2] == "?")
			{
				Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3],b[i+5], if_loop_number_ref, 0,argument);
				Roy->AddpreviousPtr(b[i + 1], b[i + 3], b[i+5],argument, "if", 0);
			}
			else
			{
				Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3],"", if_loop_number_ref, 0,argument);
				Roy->AddpreviousPtr(b[i + 1], b[i + 3], argument,"", "if", 0);
			}
			
		}
		i++;
	}
	i++;
	if (b[i] == "else")
	{
		i++;
		while (b[i] != "}")
		{
			
			if (b[i] == "if")
			{
				i = if_condition(Roy, b, node, i);
				i - 2;
			}
			if (b[i] == "=")
			{
				if (b[i + 2] == "?")
				{
					Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3], b[i + 5], 0, else_loop_number_ref, argument);
					Roy->AddpreviousPtr(b[i + 1], b[i + 3], b[i + 5], argument, "else", if_loop_number_ref);
				}
				else
				{
					Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3], "", 0, else_loop_number_ref, argument);
					Roy->AddpreviousPtr(b[i + 1], b[i + 3], argument, "", "else", if_loop_number_ref);
				}
			}
			i++;
		}
	}
	return i;
}


void creategraphy(List *Roy, vector<string> &b, struct datanode *node)
{
	int i = 0, j = 0, k = 0;

	string output1, output2;
	
	Roy->AddVertex(vsize++, 0, "","input", "", "","",0,0,"");
	Roy->AddpreviousPtr("", "", "", "", "none", 0);
	Roy->curr = Roy->head;
	while (i < b.size())
	{
		if (b[i] == "if")
		{
			i=if_condition(Roy, b, node,i);
			
		}
		else if (b[i] == "=")
		{
			if (b[i + 2] == "?")
			{
				Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3], b[i + 5], 0, 0,"");
				Roy->AddpreviousPtr(b[i + 1], b[i + 3], b[i + 5],"", "none", 0);
			}
			else
			{
				Roy->AddVertex(vsize++, latency(b[i + 2]), b[i + 2], b[i - 1], b[i + 1], b[i + 3], "", 0, 0,"");
				Roy->AddpreviousPtr(b[i + 1], b[i + 3], "", "", "none", 0);
			}
		}
		
		i++;
	}
	Roy->AddVertex(vsize++, 0, "", "Done", "", "","",0,0,"");

	while (if_loops)
	{
		Roy->ifloops.push_back(false);
		if_loops--;
	}
	
	Roy->curr = Roy->head;
	while (Roy->curr != NULL)
	{
		Roy->tail = Roy->curr;
		Roy->curr=Roy->curr->next_vertex;
	}
	//cout << "tail "<< Roy->tail<<endl;
	Roy->tail->prev_vertex.push_back(Roy->head);

	//cout << endl << endl;
	//Roy->PrintList();
}
