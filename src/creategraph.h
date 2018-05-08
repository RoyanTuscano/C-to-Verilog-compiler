#ifndef creategraph
#define creategraph
#include<iostream>
#include<vector>
#include<string>
#include "List.h"

int search_node( struct datanode *node, string data);

void creategraphy(List *Roy,vector<string> &b,struct datanode *node);
string operation(string operation);
bool compare(string one, string two, string three);
int numOf_input_operations(vector<string> &b, struct datanode *node);

#endif