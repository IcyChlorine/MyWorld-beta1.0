#include "Object.h"



Object::Object(unsigned int ID,string name):ID(ID),name(name){}


Object::~Object()
{
}


// ����Object��ID
unsigned int Object::GetID()
{
	return ID;
}


// ����Object��name
string Object::GetName()
{
	return name;
}


// ����Object��ID
void Object::SetID(int ID_new)
{
	ID = ID_new;
}
