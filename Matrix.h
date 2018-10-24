#pragma once

/*
	Written by StarSky
	2018.10.22
	files included:
		Matrix.h
		Matrix.cpp
	Version: Beta 1.0
	member functions and operators available:
	Constructor 
		(Default Constructor)
		(int r,int c)
		(int r,int c,float* data)
		(int r,int c)
		(int r,int c,vector<float> data)
		(int r,int c,vector<vector<float>> data)
	Deconstructor
		(Default Deconstructor)
	Operators
		vector<float>()
		($Matrix$Matrix) + - *
		($float$Matrix) *
		($ostream$Matrix) <<
	Functions Related
		GetIdentityMatrix(int tier)
		*/
		
#include<algorithm>
#include<vector>
#include<iostream>
#include<cstdio>
#include<iomanip>
#include<cassert>
using namespace std;

class Matrix
{
	int r, c;
	vector<float> data;
	friend ostream& operator <<(ostream& out,const Matrix& A);
	friend Matrix operator * (const int k, const Matrix& m);
public:
	~Matrix();
	Matrix() :r{ 0 }, c{ 0 } {}
	Matrix(int r,int c);
	// 从一维数组构造矩阵，其中会使data[i*r+j]=M(i+1,j+1), 0<=i<r, 0<=j<c
	Matrix(int r, int c, const float* data);
	// 从二维数组构造矩阵，其中会使data[i][j]=M(i+1,j+1), 0<=i<r, 0<=j<c
	//Matrix(int r, int c, const float data[][]);
	// 从一维vector构造数组，使data[i*r+j]=M(i+1,j+1),0<=i<r,0<=j<c
	Matrix(int r, int c, const vector<float>& data);
	// 从二维vector构造数组，使data[i][j]=M(i+1,j+1),0<=i<r,0<=j<c
	Matrix(int r, int c, const vector<vector<float>>& data);

	bool singular();
	// 返回矩阵的逆阵，如果遇到奇异矩阵会assetion failure
	Matrix reverse();
	// 返回矩阵的行数
	int row();
	// 返回矩阵的列数
	int	col();
	// 重新确定矩阵的大小
	void resize(int r_new, int c_new);
	//访问M(i,j)，i和j从1开始！
	float& operator ()(int i, int j);

	operator vector<float>();
	// 矩阵的加法
	Matrix operator +(Matrix m);
	// 矩阵减法
	Matrix operator -(Matrix m);
	// 矩阵的乘法
	Matrix operator *(Matrix m);
	// 矩阵的除法，会用到reverse()求逆阵
	Matrix operator /(Matrix m);
	// 返回矩阵是否是奇异矩阵	
	
};
//矩阵的数乘
Matrix operator * (const int k, Matrix& m);
Matrix GetIdentityMatrix(int n);



