#include <fstream>
#include <iostream>
#include "../declaration/commands.h"
#include <Windows.h>
void irun(std::string file_name){
	std::string exe_name = "generated_out.exe";
	std::string tocompile = "g++ \"" + file_name+"\" -o \""+exe_name+"\" -mwindows 2>compile_erors.txt";
	int res = std::system(tocompile.c_str());
	if (res != 0) {
		std::string eror_cmd = "notepad compile_erors.txt";
		std::system(eror_cmd.c_str());
		return;
	}
	ShellExecuteA(NULL,"open",exe_name.c_str(), NULL, NULL, SW_SHOW);
}