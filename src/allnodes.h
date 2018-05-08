#ifndef allnode
#define allnode
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
extern int nodepointer;

struct datanode {
	string name;
	float delay;
	int width;
	string typeofIOWG;
	string type;
};
int findallnodes(vector<string> &b, struct datanode *node);
int definenode(vector<string>&b, int i, struct datanode *node);
int define_till_no_keyword(vector<string>&b, int i, struct datanode *node, int width);
void printNode(struct datanode *node, int dsize);

#endif
