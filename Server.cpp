#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#define sizeoftext 256
#pragma warning(disable: 4996)
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::getline;

SOCKET Connections[100];
int Counter = 100;

class Open_mesage
{
public:
	char text[sizeoftext];
	Open_mesage() {}
};

class Module_message : public Open_mesage
{
public:
	int* m;
	char from[sizeoftext];
};

class User
{
public:
	char login[sizeoftext];
	char password[sizeoftext];
	Module_message in_msg[10];
	int letter_counter;
	User(char *login_n, char *password_n)
	{
		strcpy(login,login_n);
		strcpy(password,password_n);
		letter_counter = 0;
	}
	User() { letter_counter = 0; };
};
char login1[sizeoftext] = "Username1";
char password1[sizeoftext] = "qwerty";
char login2[sizeoftext] = "Username2";
char password2[sizeoftext] = "asdfg";
User User1(login1, password1);
User User2(login2, password2);
User current_user;

int AUTH(int index)
{
	int b = 1;
	char msg1[sizeoftext];
	char msg2[sizeoftext];
	recv(Connections[index], msg1, sizeof(msg1), NULL);
	recv(Connections[index], msg2, sizeof(msg2), NULL);
	if (strcmp(msg1, login1) == 0 && strcmp(msg2, password1) == 0)
	{
		current_user = User1;
		strcpy(msg1, "Welcome '");
		strcat(msg1, login1);
		strcat(msg1, "'!");
		send(Connections[index], msg1, sizeof(msg1), NULL);
		b = 0;
		return b;
	}
	if (strcmp(msg1, login2) == 0 && strcmp(msg2, password2) == 0)
	{
		current_user = User2;
		strcpy(msg1, "Welcome '");
		strcat(msg1, login2);
		strcat(msg1, "'!");
		send(Connections[index], msg1, sizeof(msg1), NULL);
		b = 0;
		return b;
	}
	if (b != 0)
	{
		return b;
	}
}
void WorkWithClient(int index)
{
	char msg1[sizeoftext];
	if (AUTH(index) != 0)
	{
		strcpy(msg1, "Wrong login or password!");
		send(Connections[index], msg1, sizeof(msg1), NULL);
	}
	else
	{
		for (int q = 0; q < 20; q++) 
		{
			recv(Connections[index], msg1, sizeof(msg1), NULL); 
			char comand1[sizeoftext] = "recive_msg";
			char comand2[sizeoftext] = "return_msg";
			if (strcmp(comand1, msg1) == 0)
			{
				Module_message Message2;
				char adresat_user[sizeoftext];
				recv(Connections[index], adresat_user, sizeof(adresat_user), NULL);
				if (strcmp(User1.login, adresat_user) != 0 && strcmp(User2.login, adresat_user) != 0)
				{
					strcpy(msg1, "Wrong address");
					send(Connections[index], msg1, sizeof(msg1), NULL);
				}
					strcpy(msg1, "OK");
					send(Connections[index], msg1, sizeof(msg1), NULL);
					recv(Connections[index], Message2.text, sizeof(Message2.text), NULL);
					char symbol[2];
					Message2.m = new int[sizeoftext];
					for (int i = 0; i < sizeoftext; i++)
					{
						recv(Connections[index], symbol, sizeof(symbol), NULL);
						Message2.m[i] = (int)symbol[0];
					}
					strcpy(Message2.from,current_user.login);
					if (strcmp(adresat_user, User1.login) == 0)
					{
						User1.in_msg[User1.letter_counter] = Message2;
						User1.letter_counter++;
						if (User1.letter_counter == 10)
						{
							User1.letter_counter = 0;
						}
					}
					if (strcmp(adresat_user, User2.login) == 0)
					{
						User2.in_msg[User2.letter_counter] = Message2;
						User2.letter_counter++;
						if (User2.letter_counter == 10)
						{
							User2.letter_counter = 0;
						}
					}
			}
			if (strcmp(comand2, msg1) == 0)
			{
				char symbol[2];
				symbol[0] = (char)current_user.letter_counter;
				send(Connections[index], symbol, sizeof(symbol), NULL);
				for (int j = 0; j < current_user.letter_counter; j++)
				{
					send(Connections[index], current_user.in_msg[j].from, sizeof(current_user.in_msg[j].from), NULL);
					send(Connections[index], current_user.in_msg[j].text, sizeof(current_user.in_msg[j].text), NULL);
					for (int i = 0; i < sizeoftext; i++)
					{
						symbol[0] = (char)current_user.in_msg[j].m[i];
						send(Connections[index], symbol, sizeof(symbol), NULL);
					}
				}
			}
		}
	}
}
int main()
{
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)& addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	SOCKET newConnection;
	for (int i = 0; i < Counter; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)& addr, &sizeofaddr);

		if (newConnection == 0)
		{
			cout << "Error"<<endl;
		}
		else
		{
			cout << "Client Connected!\n";
			char msg[256] = "Hello!";
			send(newConnection, msg, sizeof(msg), NULL);
			Connections[i] = newConnection;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkWithClient, (LPVOID)(i), NULL, NULL);
		}
	}
	system("pause");
	return 0;
}