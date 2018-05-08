#ifndef readvec
#define readvec
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

void readvector(vector<string>& c, string filename);
bool check_string(string stra);
void writevector(vector<string>&c, string filename);

#endif