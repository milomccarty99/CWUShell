#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <filesystem>

//#include <future>
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
	cout<<"This is a shell for CS470 Operating Systems class written by Milo McCarty." <<endl;
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
void exit_shell(string* data)
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
		return 1; // unrecognized 

	}
}
// returns a boolean array corresponding to the string of switches provided
// example:
// switches are -abc and -bc are active, get_switches will return {0,1,1,0}
// note that the final entry in the array is for signaling improper switch usage
bool* get_switches(string* data, string switches_full, bool* active_switches)
{
	int active_switches_size = switches_full.size() + 1;
	string switches = "";
	for(int i = 1; i < data_token_len; i ++)
	{
		if(data[i][0] == '-')
		{
			for(int j = 1; j < data[i].size(); j++)
			{
				switches+= data[i][j];
			}
			//cout<< data[i] << endl;
		}
	}
	// convert the switches provided to a boolean array
	for(int i = 0; i < switches.size(); i ++)
	{
		char sw = switches[i];
		//cout << sw << " switch processing" << endl; 
		bool sw_used = false;
		for(int j = 0; j < switches_full.size(); j++)
		{
			if(sw == switches_full[j])
			{
				//cout << "using switch " << endl;
				active_switches[j] = true; 
				sw_used = true;
			}

		}
		//improper switch used
		if(!sw_used)
		{
			cout<< "Switch not recognized " << sw  << "" << endl;
			active_switches[active_switches_size -1] = true;
		}
	}
	
	return active_switches;
}
int fileinfo_command_exec(bool* switches, string filename)
{	
	//-i from stat filename -c %i "inode"
	//-t from stat filename -c %F
	//-m from stat filename -c %y "

	if(switches[3])
	{
		cout << "improper switch usage" << endl;
		return 1;
	}
	if(switches[0])
	{
		/*string inode_querry = (" echo Inode Number:  `stat "+filename + " -c %i`");
		(system(inode_querry.c_str()));*/
		int inode;
		//fd = open(filename);
		struct stat file_stat;
		int ret;
		ret = stat(filename.c_str(), &file_stat);

		inode = file_stat.st_ino;
		cout<< "Inode Number: "<< inode<< endl;
	}
	if(switches[1])
	{
		string filetype_querry = ("echo File type: `stat " + filename + " -c %F`");
		system(filetype_querry.c_str());
		//std::filesystem::status(filename).type();
		//cout<<fs::status(filename).type();
	}
	if (switches[2])
	{
		/*string filemod_querry = ("echo Last modified: `stat " + filename + " -c %y`");
		system(filemod_querry.c_str());*/

		//fd = open(filename);
		struct stat file_stat;
		int ret;
		ret = stat(filename.c_str(), &file_stat);
		cout<< "Last: "<< file_stat.st_mtime << endl;
	}
	return 0;
}
//fileinfo -i -t -m  filename 
//	(default is -itm or inode, type, modified date)
int fileinfo_command(string* data)
{
	string filename;
	bool cmd_switches[4] = {0};
	get_switches(data, "itm", cmd_switches);
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
				return 1; // bad exit code
			}
		}
	}
	fstream file_stream;
	file_stream.open(filename);
	if (file_stream.fail())
	{
		file_stream.close();
		cout<<"File does not exist." << endl;;
		return 1;
	}
	return fileinfo_command_exec(cmd_switches,filename);
}
//osinfo -T -b -s (or type of file system, number of blocks in the file system, maximum number of characters in a file name)
int osinfo_command(string* data)
{
	bool switches[4] = {0};
	get_switches(data,"Tbs", switches);
	for(int i = 1; i < data_token_len; i++)
	{
		if(data[i][0]!= '-')
		{
			cout<<"unexpected token " + data[i] << endl;
			return 2;
		}
	}
	if(switches[3])
	{
		cout << "improper switch usage" << endl;
		return 1;
	}
	//T
	if(switches[0])
	{
		string filesystem_querry = "echo File system type: `df -Th / | grep '/dev' | awk '{print$2}'`";
		system(filesystem_querry.c_str());
	}
	//b
	if(switches[1])
	{
		string numblocksystem_querry = "echo Number of blocks: `df / | grep '/dev' | awk '{print$2}'`";
		system(numblocksystem_querry.c_str());
	}
	//s
	if(switches[2])
	{
		/*string filenamemaxchar_querry = "echo Max characters in file name: `getconf -a | grep -m1 'NAME_MAX' | awk '{print$2}'`";
		system(filenamemaxchar_querry.c_str());*/

		cout << "Maximum allowed characters in a file name: "<< FILENAME_MAX << endl;
	}
	return 0;
}
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
			exit_shell(data);
			break;
		case Commands::prompt:
			return change_prompt(data);
			break;
		case Commands::fileinfo:
			return fileinfo_command(data);
			break;
		case Commands::osinfo:
			return osinfo_command(data);
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
	}
//exit

	return EXIT_SUCCESS;
}


