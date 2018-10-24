#pragma once
#include"Character.h"
#include<iostream>
#include<fstream>
using namespace std;
typedef int (*Chunk)[256][16];

class Map
{
private:
	Chunk* Data;	//arraies that really save data
	int a;			//�߳�������ͼ�����ڴ��м���a*a������
	int vOrigin[3];	//��ͼԭ��λ��/ƫ�ƴ�С
public:
	Map(int _a=3);	//��ʼ��(0,0,0)
	~Map(void);

	int& operator()(int x,int y,int z);
	int& operator()(float x, float y, float z);

	// ���ļ��м������飬���������򴴽�֮
	Chunk LoadChunk(int x, int y, int z);
	bool InitChunk(Chunk c);
	// ����һ������
	bool SaveChunk(int x,int y,int z,Chunk c);

	//��Ⱦ��cΪ���ģ��뾶Ϊrange������
	void RenderAround(cord c,int Range=16);
	// Set the location of central Chunk,Data[5]
	bool LoadAround(int x, int y, int z,bool SmartLoad=true);
	// ��һ���ֵ�ͼ����ת��Ϊ�˷������˷���д��out�У����Լ������������
	void CodeOctTree(ostream& out, Chunk c, int x0, int y0, int z0, int a);
	// ��CodeOctTree���Ӧ�����ڽ���һ�ð˷�������in�ж�ȡ��д��c��
	void DecodeOctTree(istream& in, Chunk c, int x0, int y0, int z0, int a);
	// �����������ڼ��ص�����
	void Save();
};

