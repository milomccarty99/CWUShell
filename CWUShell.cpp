#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <sys/types.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <bits/stdc++.h>
using namespace std;

#define MAX_COMMAND_LEN 120
#define MAX_ARGS_LEN 120

int input_token_len = 0;
string prompt = "cwushell>";

// int exec(const char * command)
// {
// 	
// 	array<char, 128> buffer;
// 	string result;
// 	unique_ptr<FILE, decltype(&pclose)> pipe(popen(command,"r"),pclose);
// 	if (!pipe)
// 	{
// 		throw runtime_error("popen failed");
// 	}
// 	while(fgets(buffer.data(), buffer.size(), pipe.get())!=nullptr)
// 	{
// 		result+=buffer.data();
// 	}
// 	return system(command);
// 	//return nullptr;
// }
string* split(string str, char del, string* result)
{
	string temp = "";
	int pos = 0;
	for(int i = 0; i < str.size(); i++)
	{
		if(str[i] == del & temp != "")
		{
			result[pos] = temp;
			temp = "";
			pos++;
		}
		else
		{
			temp+=str[i];
		}
	}
	if(temp != "")
	{
		result[pos] = temp;
		pos ++;
	}
	
	input_token_len = pos;
	return result;
}
void cwushell_startup()
{
	system("clear");
	cout<<"This is my custom shell for CS470 Operating Systems class" <<endl;
}
//exit 

//prompt [new_prompt]

//fileinfo -i -t -m  filename 
//	(default is -itm or inode, type, modified date)

//osinfo -T -b -s (or type of file system, number of blocks in the file system, maximum number of characters in a file name)

//all other shell commands



int main(int argc, char *argv[])
{
//startup
	cwushell_startup();
	char delimeter = ' ';
	while(true)
	{
		//loop
		cout<<prompt;
		char input[ MAX_COMMAND_LEN ];
		cin.getline(input, MAX_COMMAND_LEN);
		string result[MAX_ARGS_LEN];

		string* data = split(input, delimeter, result);

		// for(int i = 0; i < input_token_len; i++)
		// {
		// 	cout<<data[i]<<endl;
		// }
	}
//exit

	return EXIT_SUCCESS;
}


