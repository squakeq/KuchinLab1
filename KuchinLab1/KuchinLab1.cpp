﻿#include <iostream>
using namespace std;
struct Pipe
{
	string name;
	int length = 0;
	int diameter = 0;
	bool is_repairing = false;
};
struct compress_Station
{
	string name;
	int shops_num;
	int busy_shops_num;
	int efficiency;
};
Pipe CreatePipe()
{
	Pipe new_pipe;
	cout << "Type name";
	cin >> new_pipe.name;
	cout << "Type length";
	cin >> new_pipe.length;
	cout << "Type diameter";
	cin >> new_pipe.diameter;
	return new_pipe;
}
void PrintPipeInfo(const Pipe& pipe)
{
	string repair_status = "False";
	if (pipe.is_repairing)
	{
		repair_status = "True";
	}
	cout << ".........." << "\n"
		<< "Pipe Name : " << pipe.name << "\n"
		<< "Pipe lenght:" << pipe.length << "\n"
		<< "Pipe deameter:" << pipe.length << "\n"
		<< "Is repairing:" << pipe.repair_status << "\n";
}
int main()
{
	Pipe new_pipe = CreatePipe();
	PrintPipeInfo(new_pipe);
}