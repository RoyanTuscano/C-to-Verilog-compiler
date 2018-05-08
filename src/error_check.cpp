#include "List.h"
#include "allnodes.h"
#include "Total_forces.h"

bool check_for_errors(List *Roy, struct datanode *node)
{
	List::node* current;
	current = Roy->head;
	current = current->next_vertex;
	string error;
	bool output = false;
	bool input=false;
	string str="";
	while (current != Roy->tail)
	{
		//compare output
		error=compare_with_node(current->output, node);
		if (error != "")
		{

			str = error;
			output = true;
		}
		//compare input1
		if (current->input1 != "")
		{
			error = compare_with_node(current->input1, node);
			if (error != "")
			{
				if (error != "1")
				{
					str = error;
					input = true;
				}

			}
		}
		
		//compare input2
		if (current->input2 != "" )
		{
			error = compare_with_node(current->input2, node);
			if (error != "")
			{
				if (error != "1")
				{
					str = error;
					input = true;
				}
			
			}

		}
		
		//compare input3
		if (current->input3 != "" )
		{
			error = compare_with_node(current->input3, node);
			if (error != "")
			{
				str = error;
				input = true;
			}
		}	
		current = current->next_vertex;
	}
	bool decide_error = false;
	try
	{
		if (input ==false && output ==false)
		{

		}
		else if (input == true && output == true)
			throw string("variable");
		else if (input == true)
			throw string("input");
		else if (output == true)
			throw string("output");
	}
	catch (string &s)
	{
	cout << "The c code has missing " << str << " " << s;
	decide_error = true;
	}

	

	return decide_error;
}

string compare_with_node(string str,struct datanode *node)
{
	for (int i = 0; i < nodepointer; i++)
	{
		if (node[i].name == str)
		{
			return "";
		}
	}
	return str;

}
