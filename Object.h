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
	// ����Object��ID
	unsigned int GetID();
	// ����Object��name
	string GetName();
	// ����Object��ID
	void SetID(int ID_new);
};

