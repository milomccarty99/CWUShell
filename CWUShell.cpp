#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <sys/types.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <filesystem>
#include <future>
//#include <bits/stdc++.h>

using namespace std;

#define MAX_COMMAND_LEN 120
#define MAX_ARGS_LEN 120

int data_token_len = 0;
string shell_prompt = "cwushell>";
bool loop_shell = true;
int exit_code = 0;

enum Commands{
	exit_shell_command,
	prompt,
	fileinfo,
	osinfo,
	system_command
};

Commands convert_commands(string cmd)
{
	if(cmd == "exit") return Commands::exit_shell_command;
	else if(cmd == "prompt") return Commands::prompt;
	else if (cmd == "fileinfo") return Commands::fileinfo;
	else if (cmd == "osinfo") return Commands::osinfo;
	else return Commands::system_command;
}

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
	
	data_token_len = pos;
	return result;
}
void cwushell_startup()
{
	system("clear");
	cout<<"This is ashell for CS470 Operating Systems class" <<endl;
}

// //eww
// void exit_shell(string* params)
// {
// 	if (input_token_len ==1)
// 	{
// 		cout<<system("$?") << endl;

// 	}
// 	else if (input_token_len)
// 	cout<<param<<endl;
// 	std::exit(param);

// }

// we exit the shell outputting the exit_code and using the standard exit with exit_code
void exit_shell()
{
	cout<< exit_code << endl;
	std::exit(exit_code);
}

//prompt [new_prompt]
int change_prompt(string* data) {
	string new_prompt = "cwushell>";
	if(data_token_len == 1)
	{
		shell_prompt = new_prompt;
		return 1;
	}
	if(data_token_len == 2)
	{
		new_prompt = data[1];
		shell_prompt = new_prompt;
		return 1;
	}
	else
	{
		cout<<"Too many prompts, prompt left unchanged.";
		return 0; // unrecognized 

	}
}
string get_switches(string* data)
{
	string switches = "";
	for(int i = 1; i < data_token_len; i ++)
	{
		if(data[i][0] == '-')
		{
			for(int j = 1; j < data[i].size(); j++)
			{
				switches+= data[i][j];
			}
		}
	}
	return switches;
}
int fileinfo_command_exec(string switches, string filename)
{
	cout<< "displaying info for "+ filename + " using switchs: " + switches << endl;
	
	//-i from stat filename -c %i "inode"
	//-t from stat filename -c %F
	//-m from stat filename -c %y "

	cout << "Inode number: ";
	//wait();
	string inode_querry = ("stat "+filename + " -c %i");
	(system(inode_querry.c_str()));
	
	cout << "File type: ";
	string filetype_querry = ("stat " + filename + " -c %F");
	system(filetype_querry.c_str());

	cout<< "Last modified: ";
	string filemod_querry = ("stat " + filename + " -c %y");
	system(filemod_querry.c_str());

	return 0;
}
//fileinfo -i -t -m  filename 
//	(default is -itm or inode, type, modified date)
int fileinfo_command(string* data)
{
	string filename;
	string cmd_switches = get_switches(data);
	for(int i = 1; i < data_token_len; i++)
	{
		if(data[i][0] != '-')
		{
			if(filename.empty())
			{
				filename = data[i];
			}
			else
			{
				cout << "Too many file names provided. " + data[i] << endl;
				return 0; // bad exit code
			}
		}
	}
	return fileinfo_command_exec(cmd_switches,filename);
}
//osinfo -T -b -s (or type of file system, number of blocks in the file system, maximum number of characters in a file name)

//all other shell commands
int shell_command(string cmd)
{
	const char * command = cmd.c_str();
	return system(command);
}

//		handler for switching between commands, then switching between switches
// 			formatting parameters

int command_switch(string* data, string raw_input)
{
	if(data_token_len == 0)
	{
		return 0;
	}
	string cmd = data[0];
	Commands command = convert_commands(cmd);
	switch(command)
	{
		case Commands::exit_shell_command:
			exit_shell();
			break;
		case Commands::prompt:
			return change_prompt(data);
			break;
		case Commands::fileinfo:
			return fileinfo_command(data);
			break;
		case Commands::osinfo:
		//bloc
		break;
		case Commands::system_command:
			return shell_command(raw_input);
			break;
	}
	return 0;
}
//		buffer



int main(int argc, char *argv[])
{
//startup
	cwushell_startup();
	char delimeter = ' ';
	while(true)
	{
		//loop
		cout<<shell_prompt;
		char input[ MAX_COMMAND_LEN ];
		cin.getline(input, MAX_COMMAND_LEN);
		string result[MAX_ARGS_LEN];

		string* data = split(input, delimeter, result);
		exit_code = command_switch(data, input);
		// for(int i = 0; i < input_token_len; i++)
		// {
		// 	cout<<data[i]<<endl;
		// }
	}
//exit

	return EXIT_SUCCESS;
}


