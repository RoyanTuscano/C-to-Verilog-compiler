#include "List.h"
#include "Total_forces.h"
#include "creategraph.h"
#include<iostream>
#include<string>

void imp_asap(List *Roy)
{
	int j = 0, start = 0, end = 0;
	List::node *current;
	current = Roy->head;
	current->time_cycle_start = 0;
	current->time_cycle_end = 0;
	current->ASAP_Scheduled = true;
	current = current->next_vertex;
	
	int temp;
	//	bool complete = false;
	while (j < 4)
	{
		current = Roy->head;
		current = current->next_vertex;
		while (current !=Roy->tail)
		{

			if (current->ASAP_Scheduled == false) {
				bool prev_unscheduled = true;
				 temp=1;
				if (current->prev_vertex.size() == 0)
				{
					Roy->ASAP_schedule(temp, temp + current->latency-1, true, current);
				}
				else
				{
					for (int i = 0; i < current->prev_vertex.size(); i++)
					{
						if (current->prev_vertex[i]->ASAP_Scheduled == true)
						{
							if (current->prev_vertex[i]->time_cycle_end >= temp)
							{
								temp = current->prev_vertex[i]->time_cycle_end;
							}
						}
						else
						{
							prev_unscheduled = false;
						}
					}
					if (prev_unscheduled == true)
					{
						Roy->ASAP_schedule(temp+1, temp + current->latency , true, current);
					}
				}
			}
			
			current = current->next_vertex;
		}
		j++;
		
	}
	//cout << endl << endl << "ASAP Answer" << endl;

	// Roy->PrintList();

}


