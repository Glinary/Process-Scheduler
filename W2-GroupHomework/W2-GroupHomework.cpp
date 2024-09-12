/*
* CSOPESY - S11
* Group 13	: Abenoja, Amelia Joyce
*			: Cuales, Bianca Mari
*			: Culala, Mary Erika
*			: Uy, Gleezell Vina
*
* Week 2 - Group Homework - Setting up your OS emulator(VERSION 2.0)
* Last modified: 09/10/2024
*/


#include <iostream>
#include <windows.h>
#include "allfunctions.h"
using namespace std;


int main()
{
	HANDLE console_color;
	std::string command;

	ASCIITextHeader();
	std::cout << "";

	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 15);
		std::cout << "Enter a command: ";

	std::cin >> command;

	while (true) {
		if (isCommandValid(command)) {
			commandRecognize(command);
		}
		else {
			std::cout << "Command not recognized. \n";
		}
		// std::cout << "\n";
		std::cout << "Enter a command: ";
		std::cin >> command;
	}
	return 0;
}
