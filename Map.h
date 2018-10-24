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
	int a;			//边长，即地图将在内存中加载a*a个区块
	int vOrigin[3];	//地图原点位置/偏移大小
public:
	Map(int _a=3);	//初始在(0,0,0)
	~Map(void);

	int& operator()(int x,int y,int z);
	int& operator()(float x, float y, float z);

	// 从文件中加载区块，若不存在则创建之
	Chunk LoadChunk(int x, int y, int z);
	bool InitChunk(Chunk c);
	// 保存一个区块
	bool SaveChunk(int x,int y,int z,Chunk c);

	//渲染以c为中心，半径为range的区域
	void RenderAround(cord c,int Range=16);
	// Set the location of central Chunk,Data[5]
	bool LoadAround(int x, int y, int z,bool SmartLoad=true);
	// 将一部分地图数据转化为八分树，八分树写到out中，用以减少数据冗余度
	void CodeOctTree(ostream& out, Chunk c, int x0, int y0, int z0, int a);
	// 与CodeOctTree相对应，用于解析一棵八分树，从in中读取，写到c中
	void DecodeOctTree(istream& in, Chunk c, int x0, int y0, int z0, int a);
	// 保存所有现在加载的区块
	void Save();
};

