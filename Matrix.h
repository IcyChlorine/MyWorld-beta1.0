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
	// ��һά���鹹��������л�ʹdata[i*r+j]=M(i+1,j+1), 0<=i<r, 0<=j<c
	Matrix(int r, int c, const float* data);
	// �Ӷ�ά���鹹��������л�ʹdata[i][j]=M(i+1,j+1), 0<=i<r, 0<=j<c
	//Matrix(int r, int c, const float data[][]);
	// ��һάvector�������飬ʹdata[i*r+j]=M(i+1,j+1),0<=i<r,0<=j<c
	Matrix(int r, int c, const vector<float>& data);
	// �Ӷ�άvector�������飬ʹdata[i][j]=M(i+1,j+1),0<=i<r,0<=j<c
	Matrix(int r, int c, const vector<vector<float>>& data);

	bool singular();
	// ���ؾ�����������������������assetion failure
	Matrix reverse();
	// ���ؾ��������
	int row();
	// ���ؾ��������
	int	col();
	// ����ȷ������Ĵ�С
	void resize(int r_new, int c_new);
	//����M(i,j)��i��j��1��ʼ��
	float& operator ()(int i, int j);

	operator vector<float>();
	// ����ļӷ�
	Matrix operator +(Matrix m);
	// �������
	Matrix operator -(Matrix m);
	// ����ĳ˷�
	Matrix operator *(Matrix m);
	// ����ĳ��������õ�reverse()������
	Matrix operator /(Matrix m);
	// ���ؾ����Ƿ����������	
	
};
//���������
Matrix operator * (const int k, Matrix& m);
Matrix GetIdentityMatrix(int n);



