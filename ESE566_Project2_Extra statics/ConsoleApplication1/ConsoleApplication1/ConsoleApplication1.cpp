// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int main()
{
	string s;
	
	getline(cin,s);
	istringstream ss(s);
	string str;
	ss >> str;
	cout << stoi(str)<<endl;
	ss >> str;
	string::size_type position;
	position = str.find("|");
	cout << position << endl;
	string s1 = str.substr(position+1);
	cout << s1 << endl;
	double i = 2.3;
	if (i == atof(s1.c_str()))
		cout << atof(s1.c_str()) << endl;
	ss >> str;	
	cout << stoi(str) << endl;
}
