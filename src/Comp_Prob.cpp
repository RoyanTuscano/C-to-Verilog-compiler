#include<iostream>
#include<string>

#include "List.h"
#include "Total_forces.h"


void Compute_Probability(List *Roy, int latency)
{
	List::node *current;
	current = Roy->head;
	current = current->next_vertex;
	//initialize all the vectors to 0
	Roy->MULTI_Prob.assign(latency,0);
	Roy->LOGIC_Prob.assign(latency,0);
	Roy->ALU_Prob.assign(latency,0);
	Roy->DIVMOD_Prob.assign(latency,0);
	//cout << endl << " counting probability " << endl;

	float* p;
	while (current != Roy->tail)
	{
		int i = 1;
		current->Prob.assign(latency, 0);
		float temp=(current->time_cycle_start_ALAP)-(current->time_cycle_start)+1;
		int start = current->time_cycle_start;
		
		while (i <=latency)
		{
			if (i < start)
			{
				current->Prob[i-1]=0;
			}
			else if (i >= start && i < start + temp)
			{
				current->Prob[i-1]=(1 /temp);
			}
			else 
			{
				current->Prob[i-1] = 0;
			}	
			i++;
		}
		check_operation(Roy, current);
		//check the type of operation and add the calculated probabilities to the main Register finction ie to ALU,MULtiplier etc
		current = current->next_vertex;
	}
//print the probilities of every vertex
	/*cout << "Printing Probability for every vertex " << endl;
	current = Roy->head;
	current = current->next_vertex;
	while (current != Roy->tail)
	{
		int i = 0;
		cout << "vertex No : "<<current->vertex_name<<"  ";
		while (i < latency)
		{
			cout << current->Prob[i] << " | ";
			i++;
		}
		cout << endl;
		current = current->next_vertex;
	}
	//print the total probabiliies of Every functions
	int i = 0;
	cout << " Printing the Probability of the every operation registers " << endl;
	while (i < 4)
	{
		if (i == 0)
		{
			cout << "MULTIPLIER  ";
			 p = Roy->MULTI_Prob.data();
		}
		else if (i == 1)
		{
			cout << "ALU  ";
			 p = Roy->ALU_Prob.data();
		}
		else if (i == 2)
		{
			cout << "DIV MOD  ";
			 p = Roy->DIVMOD_Prob.data();
		}
		else
		{
			cout << "LOGIC  ";
			 p = Roy->LOGIC_Prob.data();
		}
		int j = 0;
		while (j < latency)
		{
			cout << " | " << p[j];
				j++;
		}
		cout << endl;
		i++;
	}*/
}

void check_operation(List *Roy, struct List::node *curren)
{
	List::node *current;
	current = curren;
	float *p;
	if (current->operation == "MULT")
	{
		p = Roy->MULTI_Prob.data();
	}
	else if (current->operation == "ALU")
	{
		p = Roy->ALU_Prob.data();
	}
	else if (current->operation == "Modulo/Divider")
	{
		p = Roy->DIVMOD_Prob.data();
	}
	else	//this is logical
	{
		p = Roy->LOGIC_Prob.data();
	}
	int i = 0;

	while (i<Roy->t_latency)
	{
		p[i] = p[i] + current->Prob[i];
		i++;
	}
}
 
