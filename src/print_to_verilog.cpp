#include"ToVerilog.h"
#include "List.h"
#include "allnodes.h"

void print_to_veri(List *Roy, vector<string>&b, vector<string> &c, struct datanode *node)
{
	List::state* current_state;
	current_state = Roy->state_head;
	List::node* temp;

	c.push_back("module HLSM ( Clk, Rst, Start, Done, ");
	int i = 0;
	//declare the inputs and outputs
	while (i < nodepointer)
	{
		if (node[i].typeofIOWG == "input" || node[i].typeofIOWG == "output")
		{
			c.push_back(node[i].name);
			c.push_back(", ");
		}
		i++;
	}
	c.pop_back();
	c.push_back(")");
	c.push_back(", ");
	
	//write all inputs with respective width
	int wid =0;
	string sign="";

	i = 0;
	string typereg = "";
	while (i < nodepointer)
	{
		
			if (node[i].type == sign && node[i].width == wid && node[i].typeofIOWG==typereg)
			{
				
				c.push_back(node[i].name);
				c.push_back(", ");
			}
			else 
			{
				wid = node[i].width;
				typereg = node[i].typeofIOWG;
				sign = node[i].type;

				c.pop_back();
				c.push_back(" ;");
				if (node[i].typeofIOWG == "input")
				{
					c.push_back(" input ");
				}
				else if (node[i].typeofIOWG == "variable")
				{
					c.push_back(" variable reg ");
				}
				else if (node[i].typeofIOWG == "output")
				{
					c.push_back(" output reg ");
				}

				if (node[i].type.at(0) == 'I')
				{
					c.push_back(" signed ");
				}
			
				c.push_back(" [ " + to_string(wid - 1) + " : 0 ] ");
				c.push_back(node[i].name);
				c.push_back(", ");
			}
		i++;
	}
	c.pop_back();
	c.push_back(" ;");
	int ref = 1;
	int j = 1;
	//find the width of the state register
	while (ref)
	{
		ref = ref * 2;
		if (ref >= Roy->no_of_states)
		{
			break;
		}
		else
		{
			j++;
		}
	}
	c.push_back("parameter wait = 0");







//
List::state* currenty_state;
	currenty_state = Roy->state_head;
	currenty_state=currenty_state->next_state;
	int ref_num=1;
while (currenty_state != Roy->state_tail)
		{
			c.push_back(", "+ currenty_state->state_num +" = "+ to_string(ref_num++));
			currenty_state=currenty_state->next_state;
		}
	c.push_back(", done = " + to_string(i));
	c.push_back(" ;");
	c.push_back("reg [ " + to_string(j - 1) + " : 0 ] State");
		c.push_back(" ;");
	

	c.push_back("always @(posedge Clk)");
		c.push_back("begin");
		c.push_back("if ( Rst==1 ) ");
		c.push_back("begin");
		//declare the reg variable as 0 and the output reg as 0
		i = 0;
		while (i < nodepointer)
		{
			if (node[i].typeofIOWG == "variable" || node[i].typeofIOWG == "output")
			{
				c.push_back(node[i].name + " = ");
				c.push_back(to_string(node[i].width) + " ' d 0");
				c.push_back(" ;");
			}
			i++;
		}
		//push next state as wait
		c.push_back("State <= wait");
		c.push_back(" ;");
	c.push_back("end");
	c.push_back("else ");
	c.push_back("begin");
	c.push_back("case ( State");
	c.push_back(" )");
	while (current_state != NULL)
	{
		c.push_back(current_state->state_num+" :");
		c.push_back("begin");
		temp = NULL;
		for (int i = 0; i < current_state->operation_ptr.size(); i++)
		{
			//execute the operation
			if (current_state->operation_ptr[i] == Roy->head || current_state->operation_ptr[i] == Roy->tail || current_state->operation_ptr[i]->if_variable != 0)
			{
				temp = current_state->operation_ptr[i];
			}
			else
			{
				execute_operation(current_state->operation_ptr[i],c);
			}
		}

		// decide the next state
		if (current_state->if_branch != 0)
		{
			c.push_back("if( ");
			c.push_back(temp->output);
			c.push_back(" )");
			if (current_state->next_if != Roy->state_tail)
			{
				c.push_back("State <= " + current_state->next_if->state_num);
			}
			else
			{
				c.push_back("State <= Final");
			}
			
			c.push_back(" ;");
			if (current_state->next_else != NULL)
			{
				c.push_back("else");
				if (current_state->next_else != Roy->state_tail)
				{
					c.push_back("State <= " + current_state->next_else->state_num);
				}
				else
				{
					c.push_back("State <= Final");
				}
				
				c.push_back(" ;");
			}
			c.push_back("end");
		}
		else if (current_state == Roy->state_head)
		{
			c.push_back("if( ");
			c.push_back(temp->output);
			c.push_back(" )");
			c.push_back("State <= " + current_state->next_if->state_num);
			c.push_back(" ;");
			c.push_back("else");
			c.push_back("State <= " + current_state->next_else->state_num);
			c.push_back(" ;");
			c.push_back("end");
		}
		else if (current_state == Roy->state_tail)
		{
			c.push_back("Done <= 1");
			c.push_back(" ;");
			c.push_back("end");
		}
		else
		{
			if (current_state->succ_state != NULL)
			{
				c.push_back("State <= " + current_state->succ_state->state_num);
			}
			else if (current_state->next_if != NULL)
			{
				c.push_back("State <= " + current_state->next_if->state_num);
			}
			else if (current_state->next_else != NULL)
			{
				c.push_back("State <= " + current_state->next_else->state_num);
			}
			c.push_back(" ;");
			c.push_back("end");
		}
		current_state = current_state->next_state;
	}

	c.push_back("endcase");
	c.push_back("end");
	c.push_back("end");
	c.push_back("endmodule");

}


void execute_operation(struct List::node *curren, vector<string>&c)
{
	List::node *current;
	current = curren;
	if (current->sign_operation != "?")
	{
		c.push_back(current->output + "<=" + current->input1 + " " + current->sign_operation + " " + current->input2);
		c.push_back(" ;");
	}
	else
	{
		c.push_back(current->output + " <= " + current->input1 + " " + current->sign_operation + " " + current->input2 + " : " + current->input3);
		c.push_back(" ;");
	}
}
