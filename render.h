#pragma once

#include<Windows.h>
#include<glut.h>
#include<cmath>

void Cube(float x, float y, float z, float a, int id);
bool InitGame();
bool InitGL();
void Render();
void RenderGUI();
inline float PXLTOGL(int x)
{
	extern int wnd_height;
	static const float zNear = 0.1f;
	static const float ksi = 3.141592f / 4;
	return (float)x / wnd_height*zNear*tan(ksi / 2) * 2;
}//把以像素为单位的长度转化为以GL中的米为单位的长度
//前提是这是画在gluPerspective确定的zNear平面上的坐标（即z=0.1f）