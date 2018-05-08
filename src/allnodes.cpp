#include "allnodes.h"
int nodepointer = 0;


int findallnodes(vector<string> &b,struct datanode *node)		// to check the the vector for all inputs, output, wire and reg
{
	int i = 0;
	while (i<b.size()-7)
	{
		if (b[i] == "input" || b[i] == "output" || b[i] == "wire" || b[i] == "register"	|| b[i]=="variable")
		{
			i = definenode(b, i, node);				//to find all the nodes in the repective type of IOWG
		}
		else
			i++;

	}
	//printNode(node, nodepointer);
	return nodepointer;
}



int definenode(vector<string>&b,int i,struct datanode *node) {
	i++;
	string str=b[i];
	int l = str.length();
	int type;
	
	if (str.substr(0, 1) == "U")		//check if unsigned
	{
		if (l < 6)
			type = stoi(str.substr(l - 1));		//extracts the width of the inout or output etc
		else
			type = stoi(str.substr(l - 2));
	}
	else
	{
		if (l < 5)
			type = stoi(str.substr(l - 1));
		else
			type = stoi(str.substr(l - 2));

	}
	
	 i=define_till_no_keyword(b, i, node, type);	//to find all the nodes in respective inpit ouptut,etc
	return i;
}

int define_till_no_keyword(vector<string>&b, int i, struct datanode *node, int width) {

string IOWG = b[i - 1];
string type = b[i];
while (i < b.size()-5)
{
	i++;

			if (b[i] == "input" || b[i] == "output" || b[i] == "wire" || b[i] == "register" || b[i] == "variable" || b[i + 1] == "=")
			{
				return i;
			}
			else
			{
				if (b[i].find(",") != string::npos)
				{
					node[nodepointer].name = b[i].substr(0, b[i].length() - 1);
				}
				else
					node[nodepointer].name = b[i];
				
				node[nodepointer].typeofIOWG = IOWG;
				node[nodepointer].type = type;
				node[nodepointer].width = width;
				node[nodepointer].delay = 0.0;
				nodepointer++;
			}
			
}
return i;
}


void printNode(struct datanode *node, int dsize)
{
	for (int i = 0; i < dsize; i++)
	{
		cout << "Node Name : " << node[i].name << " delay : " << node[i].delay << " Width : " << node[i].width <<" Type of Connection : "<<node[i].typeofIOWG<<" Type of if signed or unsigned :"<<node[i].type<<endl;
	}

}

