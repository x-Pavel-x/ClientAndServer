#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <conio.h>
#define sizeoftext 256
#pragma warning(disable: 4996)
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::getline;

class Open_mesage
{
public:
	char text[sizeoftext];
	Open_mesage() 
	{
	}
};
class Module_message : public Open_mesage
{
public:
	int* m;
	Module_message()
	{
		m = new int[sizeoftext]; 
	}
	void shifr()
	{
		for (int i = 0; i < sizeoftext; i++)
		{
			m[i] = 1 + rand() % 9;
			text[i] = text[i] ^ m[i];
		}
	}
	void deshifr()
	{
		for (int i = 0; i < sizeoftext; i++)
		{
			text[i] = text[i] ^ m[i];
		}
	}
	~Module_message() 
	{
		delete[] m;
	}
}message2;

void menu(char *name)
{
	cout <<"   _______Menu________		      ||"<<name<<endl
		 << "1. Send message" << endl
		 << "2. Receive message" << endl
		 << "3. Exit" << endl;
}
int main()
{
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) 
	{
		cout << "Error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)& addr, sizeof(addr)) != 0) 
	{
		cout << "Error: failed connect to server."<<endl;
		system("pause");
		return 1;
	}
	cout << "Connected to server!"<<endl;
	char msg[sizeoftext];
	recv(Connection, msg, sizeof(msg), NULL);
	cout << msg << endl;
	cout << "input ur login" << endl;
	cin.getline(msg, sizeoftext);
	send(Connection, msg, sizeof(msg), NULL);
	cout << "input ur password" << endl;
	cin.getline(msg, sizeoftext);
	send(Connection, msg, sizeof(msg), NULL);
	recv(Connection, msg, sizeof(msg), NULL);
	system("cls");
	if (strcmp(msg, "Wrong login or password!") == 0)
	{
		cout << msg<<endl;
		system("pause");
		exit(1);
	}
	char name[sizeoftext];
	strcpy(name, msg);
	for (int q = 0; q < 20; q++) 
	{
		menu(name);
		char f;
		f = _getch();
		switch (f)
		{			
		case '1':
		{
			strcpy(msg, "recive_msg");
			send(Connection, msg, sizeof(msg), NULL);
			cout << "Input address:" << endl;
			cin.getline(msg, sizeoftext);
			send(Connection, msg, sizeof(msg), NULL);
			recv(Connection, msg, sizeof(msg), NULL);
			if (strcmp(msg, "Wrong address") == 0)
			{
				system("cls");
				cout << msg <<endl;
				system("pause");
				exit(1);
			}
			cout << "Input message:" << endl;
			cin.getline(message2.text, sizeoftext);
			message2.shifr();
			send(Connection, message2.text, sizeof(message2.text), NULL);
			char symbol[2];
			for (int i = 0; i < sizeoftext; i++)
			{
				symbol[0] = (char)message2.m[i];
				send(Connection, symbol, sizeof(symbol), NULL);
			}
			system("cls");
			break;
		}
		case '2':
		{
			char symbol[2];
			char from[sizeoftext];
			strcpy(msg, "return_msg");
			send(Connection, msg, sizeof(msg), NULL);
			recv(Connection, symbol, sizeof(symbol), NULL);
			int letter_count;
			letter_count = (int)symbol[0];
			for (int j = 0; j < letter_count; j++)
			{
				recv(Connection, from, sizeof(from), NULL);
				recv(Connection, message2.text, sizeof(message2.text), NULL);
				for (int i = 0; i < sizeoftext; i++)
				{
					recv(Connection, symbol, sizeof(symbol), NULL);
					message2.m[i] = (int)symbol[0];
				}
				message2.deshifr();
				cout << "***********"<<endl<<"Message " << j+1 <<endl<<"From: "<<from << endl << message2.text <<endl;
			}
			if (letter_count == 0) 
			{
				cout << "No message!" << endl;
			}
			system("pause");
			system("cls");
			break;
		}
		case '3':
		{
			exit(0);
			break;
		}
		default:
		{
			cout << "Input 1 , 2 or 3!" << endl;
			break;
		}
		}
	}
	system("pause");
	return 0;
}