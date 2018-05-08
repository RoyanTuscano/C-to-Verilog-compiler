#include <math.h>
#include "List.h"
#include "Total_forces.h"



void selfforce(List *Roy, int latency)
{
	List::node *current;
	current = Roy->head;
	current = current->next_vertex;

	while (current != Roy->tail)
	{
		imp_asap(Roy);
		imp_alap(Roy, latency);

		Compute_Probability(Roy, latency);
		cout <<endl<< "Probability computed " << endl;
		int start_frame = current->time_cycle_start;
		int end_frame = current->time_cycle_start_ALAP;
		//by default scheduled at start frame
		
		int ntime = end_frame - start_frame;
		int i = 1 ,j=0;
		//start
		float preforce = 0;
		float succforce = 0;
		float selfforce = 0;
		cout << endl << "Operation on vertex No : " << current->vertex_name<<endl;
		float selforce = 0;
		float temp = 0;
			while (i <= latency)
			{
				selfforce= 0;
				preforce = 0;
				succforce = 0;
				cout << endl;
				cout << "LATENCY CYCLE : " << i;
					//will hold the lowest value of the Total force
									//this is scheduling for self_force at timecycle i
				if (i >= start_frame && i <= end_frame)
				{
					//this code is for calculating the selforce at time cycle i
					selfforce = find_self_force(Roy, current->time_cycle_start, current->time_cycle_start_ALAP, i, current);


					//this code is for calculating the predessor force

					preforce = find_pre_force(Roy, current, i);
					succforce = find_succ_force(Roy, current, i);
					std::cout.precision(4);
					cout << std::fixed;
					cout << endl << "-----------------------------------" << endl;
					cout << "self force : " << selfforce << endl;
					cout << "predecessor force : " << preforce << endl;
					cout << "succersor force : " << succforce << endl;
					cout << "HERE Comes the TOTAL FORCE for time Cycle: " << i << " : " << selfforce + preforce + succforce << endl;
					cout << "-----------------------------------" << endl;
					selfforce = selfforce + preforce + succforce;
					if (j != 0)
					{
						if (temp > selfforce)
						{
							//cout << endl << "Why am I here" << temp << " | " << selfforce << endl;
							temp = selfforce;
							current->schedulecycle = i;
							
						}
					}
					else
					{
						temp = selfforce;
						current->schedulecycle = current->time_cycle_start;
					}
					j++;
				}
				i++;
			}
		// Update the schedule cycle for the node scheduled
			
				current->time_cycle_start = current->schedulecycle;
				current->time_cycle_start_ALAP = current->schedulecycle;
				current->time_cycle_end = current->schedulecycle + current->latency - 1;
				current->time_cycle_end_ALAP = current->schedulecycle + current->latency - 1;
		//Now only let the current node be scheduled and its previous and unschedule all the remaining nodes for ASAP and ALAP
				Unschedule_ASAP_ALAP(Roy, current->next_vertex);
		current = current->next_vertex;
	}
	
	//print the scheduled nodes with their cycles
	Roy->curr = Roy->head;
	Roy->curr = Roy->curr->next_vertex;
	cout << endl << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
	while (Roy->curr != Roy->tail)
	{
		cout <<"Vertex: "<<Roy->curr->vertex_name<< " The start time: "<<Roy->curr->schedulecycle<<" End time : "<<Roy->curr->schedulecycle+Roy->curr->latency-1<<endl;
		Roy->curr = Roy->curr->next_vertex;
	}
}
float find_pre_force(List *Roy, struct List::node *curren,int i)
{
	
	List::node *current;
	current = curren;
	//cout << endl << "Predecessor Force " << current->vertex_name<<" | "<< current->prev_vertex1->time_cycle_end_ALAP<<" | "<<i;
	std::cout.precision(4);
	float Total_pre_force = 0;
	for (int k = 0; k < current->prev_vertex.size(); k++)
	{
		if (i <= current->prev_vertex[k]->time_cycle_end_ALAP)
		{
			float *t, *p;
			t = see_operation(Roy, current->prev_vertex[k]);
			p = current->prev_vertex[k]->Prob.data();

			int time_ref = current->prev_vertex[k]->time_cycle_start;
			int cycle_ref = i + current->latency - 1;
			float prob_at_prev_node = 1 / float(i-time_ref+ current->prev_vertex[k]->latency-1);
			while (time_ref <= i - current->prev_vertex[k]->latency)
			{
				float pre_force = find_succ_force(Roy, current->prev_vertex[k], time_ref);
				Total_pre_force = Total_pre_force + pre_force;
				Total_pre_force = Total_pre_force + t[time_ref - 1] * (prob_at_prev_node - p[time_ref - 1]);
				cout << current->prev_vertex[k]->vertex_name << " | TSF in p = "<< prob_at_prev_node << " : " << Total_pre_force << " | cycle number " << time_ref <<" | its succ force "<< pre_force<<endl;
				time_ref++;
			}
			while (time_ref <= current->prev_vertex[k]->time_cycle_end_ALAP)
			{
				Total_pre_force = Total_pre_force + t[time_ref - 1] * (0 - p[time_ref - 1]);
				cout <<current->prev_vertex[k]->vertex_name << " | TSF in p = 0 : " << Total_pre_force << " | cycle number " << time_ref << endl;
				time_ref++;

			}

			//cout << endl;
			//calculate the force
		}
	}
	

	return Total_pre_force;
}
float find_succ_force(List *Roy, struct List::node *curren, int i)
{

	List::node *current;
	current = curren;
	//cout << endl << "Succesor Force " << current->vertex_name << " | " << current->succ_vertex1->time_cycle_end_ALAP << " | " << i;
	std::cout.precision(4);
	float Total_succ_force = 0 ;
	for (int k = 0; k < current->succ_vertex.size(); k++)
	{


		if (i + current->latency - 1 >= current->succ_vertex[k]->time_cycle_start && i + current->latency - 1 <= current->succ_vertex[k]->time_cycle_start_ALAP && current->succ_vertex[k] != Roy->tail)
		{
			//int j = 1;
			//int j = current->succ_vertex[k]->time_cycle_start;
			float *t, *p;
			t = see_operation(Roy, current->succ_vertex[k]);
			p = current->succ_vertex[k]->Prob.data();

			int time_ref = current->succ_vertex[k]->time_cycle_start;
			int cycle_ref = i + current->latency-1;
			cout << endl;
			while (time_ref <= cycle_ref)
			{
				float inst = t[time_ref - 1] * (0 - p[time_ref - 1]);
				Total_succ_force = Total_succ_force +inst ;
				//cout <<current->succ_vertex[k]->vertex_name << " | TSF in p = 0 : " << Total_succ_force <<" force at inst: "<<inst<<" | cycle number " << time_ref << endl;
				time_ref++;
				
			}
			float prob_at_succ_node = 1 /float (1 + (current->succ_vertex[k]->time_cycle_start_ALAP-time_ref));
			//cout << " | " << time_ref << " | " << current->succ_vertex[k]->time_cycle_start_ALAP<<" | "<<prob_at_succ_node <<endl;
			while (time_ref <= current->succ_vertex[k]->time_cycle_start_ALAP)
			{
				float inst = t[time_ref - 1] * (prob_at_succ_node - p[time_ref - 1]);
				float succ_force = find_succ_force(Roy, current->succ_vertex[k], time_ref);
				Total_succ_force = Total_succ_force + succ_force;
				Total_succ_force=Total_succ_force+inst;
				//cout << current->succ_vertex[k]->vertex_name << " | TSF in p = "<< prob_at_succ_node << " : " << Total_succ_force <<" force at inst: " << inst << " | cycle number " << time_ref <<" | its succ force "<< succ_force<<endl;
				time_ref++;
			}
			cout << endl;
			//calculate the force
		}
	}
	
	
	//cout << endl<<"Total Successor force : " << succ_force1 + succ_force2 + succ_force3 + succ_force4<<endl;
	return Total_succ_force ;
}

/*
float find_self_force(List *Roy,int start, int end,int i,struct List::node *curren )
{
List::node *current;
current = curren;
float *t, *p;
t = see_operation(Roy,current);
p = current->Prob.data();
float selfforce=0;
int j = 0;
int temp = 0;
//this code is for calculating the selforce
while (j <= end-start)
{
if (i == (start + j))
{
temp = 1;
}
else
{
temp = 0;
}
selfforce = selfforce + t[start + j - 1] * (temp - p[start + j - 1]);
j++;
}
return selfforce;
}
*/
/*

float find_self_force(List *Roy,int start, int end,int i,struct List::node *curren )
{
List::node *current;
current = curren;
float *t, *p;
t = see_operation(Roy,current);
p = current->Prob.data();
float selfforce=0;
int j = start;
int temp = 0;
//this code is for calculating the selforce
while (j <= end)
{
temp = 0;
if (j == i)
{
temp = 1;
}
selfforce = selfforce + t[j - 1] * (temp - p[j - 1]);
j++;
}
return selfforce;


*/
float find_self_force(List *Roy,int start, int end,int i,struct List::node *curren )
{
	List::node *current;
	current = curren;
	float *t, *p;
	t = see_operation(Roy,current);
	p = current->Prob.data();
	float selfforce=0;
	int j = start;
	int temp = 0;
	//this code is for calculating the selforce
	while (j <= end)
	{
		temp = 0;
		if (j == i)
		{
			temp = 1;
		}
		selfforce = selfforce + t[j - 1] * (temp - p[j - 1]);
		j++;
	}
	return selfforce;
}

void Unschedule_ASAP_ALAP(List *Roy, struct List::node *curren)
{
	List::node *current;
	current = curren;
	while (current != Roy->tail)
	{
		current->ASAP_Scheduled = false;
		current->ALAP_Scheduled = false;
		current = current->next_vertex;
	}
}



float* see_operation(List *Roy, struct List::node *curren)
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
	return p;
}