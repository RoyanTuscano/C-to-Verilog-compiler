#include "List.h"
#include "Total_forces.h"
#include "creategraph.h"
#include<iostream>
#include<string>

void imp_alap(List *Roy, int latency)
{
	int i = 0;
	List::node *current;
	current = Roy -> tail;
	current->ALAP_Scheduled = true;
	current->time_cycle_start_ALAP = latency+1;
	current->time_cycle_end_ALAP = latency;
	int temp = Roy->t_latency;
	while (i < 4)
	{
		current = Roy->tail;
		current = current->previous_vertex;
		while (current != Roy->head)
		{
			if (current->ALAP_Scheduled == false) {

				bool succ_unscheduled = true;
				temp = Roy->t_latency;
				if (current->succ_vertex.size() == 0)
				{
					Roy->ALAP_schedule(temp - current->latency+1, temp, current, true);
				}
				else
				{
					for (int i = 0; i < current->succ_vertex.size(); i++)
					{
						if (current->succ_vertex[i]->ALAP_Scheduled == true)
						{
							if (current->succ_vertex[i]->time_cycle_start_ALAP <= temp)
							{
								temp = current->succ_vertex[i]->time_cycle_start_ALAP;
							}
						}
						else
						{
							succ_unscheduled = false;
						}
					}
					if (succ_unscheduled == true)
					{
						Roy->ALAP_schedule(temp - current->latency, temp-1, current, true);
					}
				}
			}
			
			current = current->previous_vertex;
		}
		i++;
		
	}
	//cout << endl << endl << "ALAP Answer" << endl;

	//Roy->PrintList();

}
