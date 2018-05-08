#include "read_write.h"
#include "allnodes.h"
#include "List.h"
#include "creategraph.h"
#include "Total_forces.h"
#include "ToVerilog.h"

void print(vector<string> &c)
{
	bool firstC = true;
	for (int i = 0; i < c.size(); i++)
	{
		cout << c[i];
		if (c[i] == ")" && firstC == true)
		{
			cout << endl;
			firstC = false;
		}
		//cout << " " << c[i];
		if (c[i] == ";")
			cout << endl;
	}
}


void printv(vector<string> &c)
{

	for (int i = 0; i < c.size(); i++)
	{
		cout <<i<<" | " <<c[i];
		cout << endl;
	}
}
string remove_txt(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '.')
			return str.substr(0, i);
	}
	try {
		throw 1;
	}
	catch (int x) {
		cout << "Incorrect file format";
	}
	return "0";
}

int main(int argc, char*argv[]) {
	vector<string> b;//read vector
	vector<string> c;
	readvector(b, argv[1]);
	cout << endl << "-----------------------------------" << endl;
	//choose your latency here
	int latency = stoi(argv[2]);
	datanode node[60];
	int nsize = findallnodes(b, node);
	//printNode(node, nsize);			//print the node..can be removed
	cout << endl<<endl;

	List l;
	List *Roy=&l;
	Roy->t_latency = latency;

	creategraphy(Roy,b,node);

	bool error=	check_for_errors(Roy, node);
	
	if (error == true)
	{
		return 0;
	}
	
	cout << endl << endl << endl;
	//find the total probability for every vertex and then for every ALU,MULTI,LOGIC and DIVIDE
	//Compute_Probability(Roy, latency);
	//find the self force, predecessor force, successor force
	selfforce(Roy,latency);
	To_verilog(Roy, b,c, node);
	print_to_veri(Roy, b, c, node);                        
	cout << endl << endl;
	writevector(c,argv[3]);
	//cout << endl << endl;
	//cout << endl << "The End";
	return 0;
}
 
