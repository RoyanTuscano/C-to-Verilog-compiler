#include "read_write.h"
bool check_string(string stra)
{
	for (int i = 0; i < stra.length(); i++)
	{
		if (stra.at(i) != ' ')
		{
			return true;
		}
	}
	return false;
}
void readvector(vector<string>& c,string filename)
{
	//string filename;
	string str,stra;
	int ignore=0;
	//int begin = 0;
	int l=0;
	int i = 0;
	int j = 0;
	ifstream b;
	b.open(filename);
	if (!b.is_open()) { cout << "\nfailure to read file" << endl; }
	while (getline(b, str))
	{
		int begin = 0;
		l = 0;
		//cout << str << endl;
		if (str.find("//") != string::npos)
		{
			str = str.substr(0, str.find("//"));
		}
	
		if (check_string(str)==true)
		{
			for (i = 0; i < str.length(); i++)
			{
				if (i == 0)
				{
					while (str.at(i) == ' ' || str.at(i) == '	')
					{
						str.erase(0, 1);
					}
				}

				l++;
				if (str.at(i) == ' ')
				{
					stra = str.substr(begin, l - 1);
					if (check_string(stra)) {
						c.push_back(stra);
						begin = i + 1;
						l = 0;
					}
				}

			}
			stra = str.substr(begin);
			if (check_string(stra))
			{
				c.push_back(stra);
			}

		}
		
		
		
	}
	b.close();

	return;
}
void writevector(vector<string> &c, string filename)
{
	ofstream myfile;
	myfile.open(filename);
	if (!myfile.is_open()) { cout << "\nfailure to write file" << endl; }
	for (int i = 0; i < c.size(); i++)
	{
		myfile << c[i];
		if (c[i] == " ;")
			myfile<< endl;
		else if (c[i] == "begin")
			myfile << endl;
		else if (c[i] == " )")
			myfile << endl;
		else if (c[i] == "else")
			myfile << endl;
		else if (c[i] == "end")
			myfile << endl;
		else if (c[i] == "endcase")
			myfile << endl;
	}
	myfile.close();

}


