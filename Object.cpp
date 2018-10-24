#include "Object.h"



Object::Object(unsigned int ID,string name):ID(ID),name(name){}


Object::~Object()
{
}


// 返回Object的ID
unsigned int Object::GetID()
{
	return ID;
}


// 返回Object的name
string Object::GetName()
{
	return name;
}


// 设置Object的ID
void Object::SetID(int ID_new)
{
	ID = ID_new;
}
