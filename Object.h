#pragma once

#include<string>
using namespace std;

class Object
{
public:
	Object(unsigned int ID=0,string name="EmptyObject");
	~Object();
protected:
	unsigned int ID;
	string name;
public:
	// 返回Object的ID
	unsigned int GetID();
	// 返回Object的name
	string GetName();
	// 设置Object的ID
	void SetID(int ID_new);
};

