#ifndef ToVerilogs
#define ToVerilogs
#include<iostream>
#include<vector>
#include<string>
#include "List.h"


void To_verilog(List *Roy, vector<string> &b, vector<string> &c, struct datanode *node);
void sort_loop(vector<List::node*> &if_hold_loops, string if_else);
void add_simple(vector<List::node*> &a, List::state* curren);
void find_next_states(List* Roy,List::state* current);
void print_to_veri(List *Roy, vector<string>&b, vector<string> &c, struct datanode *node);
List::state* successor_state(List* Roy, List::state* current, int time_cycle, int if_loop_number, int else_loop_number);
void execute_operation(struct List::node *curren, vector<string>&c);
string determine_nextstate(struct List::node *curren);
void check_if_IfStatement(List *Roy, struct List::node *curren, vector<string> &c);
#endif
