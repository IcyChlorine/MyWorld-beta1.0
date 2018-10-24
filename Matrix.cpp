#include "Matrix.h"



Matrix::Matrix(int r,int c):r(r),c(c)
{
	data.resize(r*c);
}


Matrix::~Matrix()
{
}


// 从一维数组构造矩阵，其中会使data[i*r+j]=M(i,j)
Matrix::Matrix(int r, int c, const float* data):r(r),c(c)
{
	Matrix::data.resize(r*c);
	for (int i = 0; i < r*c; i++)
		Matrix::data[i] = data[i];
}


// 从二维数组构造矩阵，会使data[i][j]=M(i,j) 
/*Matrix::Matrix(int r, int c, const float data[][]) :r(r), c(c)
{
	Matrix::data.resize(r*c);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < r; j++)
			Matrix::data[i*c + j] = data[i][j];
}*/


// 从一维vector构造数组，使data[i*r+j]=M(i+1,j+1),0<=i<r,0<=j<c
Matrix::Matrix(int r, int c, const vector<float>& data):r(r),c(c),data(data)
{

}


Matrix::Matrix(int r, int c, const vector<vector<float>>& data) : r(r), c(c)
{
	if (!r && !c)
	{
		r = data.size();
		c = data[0].size();
	}
	Matrix::data.resize(r*c);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < r; j++)
			Matrix::data[i*c + j] = data[i][j];
}


Matrix::operator vector<float>()
{
	return data;
}

float& Matrix::operator()(int i, int j)
{
	return data[(i - 1)*c + (j - 1)];
	// TODO: 在此处插入 return 语句
}

ostream & operator<<(ostream & out,const Matrix & A)
{
	for (int i = 0; i < A.r; i++)
	{
		for (int j = 0; j < A.c; j++)
			//out /*<< setprecision(3)*/ << A.data[i*A.c + j] << " ";
			out /*<< setprecision(3)*/ << A.data[i*A.r+j] << " ";
		out << "\n";
	}
	return out;
	// TODO: 在此处插入 return 语句
}


// 矩阵的加法
Matrix Matrix::operator +(Matrix m)
{
	assert(row() == m.row() && col() == m.col());
	Matrix n(r, c);
	for (int i = 1; i <= r; i++)
	{
		for (int j = 1; j <= c; j++)
		{
			n(i, j) = this->operator()(i, j) + m(i, j);
		}
	}
	return n;
}


// 矩阵减法
Matrix Matrix::operator -(Matrix m)
{
	assert(row() == m.row() && col() == m.col());
	Matrix n(r, c);
	for (int i = 1; i <= r; i++)
		for (int j = 1; j <= c; j++)
			n(i, j) = this->operator()(i, j) - m(i, j);
	return n;
}


// 矩阵的乘法
Matrix Matrix::operator *(Matrix m)
{
	assert(row() == m.col());
	Matrix n(r, m.col());
	for (int i = 1; i <= r; i++)
		for (int j = 1; j <= m.col(); j++)
			for (int k = 1; k <= c; k++)
				n(i, j) += this->operator()(i, k) * m(k, j);
	return n;
}


// 矩阵的除法，会用到reverse()求逆阵
Matrix Matrix::operator /(Matrix m)
{
	return Matrix();
}


// 求矩阵的逆阵，遇到奇异矩阵时会assertion failure
Matrix Matrix::reverse()
{
	return Matrix();
}


// 返回矩阵是否是奇异矩阵
bool Matrix::singular()
{
	return false;
}


Matrix operator * (const int k, Matrix& m)
{
	Matrix n(m.row(), m.col());
	for (int i = 1; i <= m.row(); i++)
		for (int j = 1; j <= m.col(); j++)
			n(i, j) = k*m(i, j);
	return n;
}

Matrix GetIdentityMatrix(int n)
{
	Matrix E(n, n);
	for (int i = 1; i <= n; i++)
		E(i, i) = 1;
	return E;
}


// 返回矩阵的行数
int Matrix::row()
{
	return r;
}


// 返回矩阵的列数
int Matrix::col()
{
	return c;
}


// 重新确定矩阵的大小
void Matrix::resize(int r_new, int c_new)
{
	assert(r_new >= 0 && c_new >= 0);
	r = r_new;
	c = c_new;
	data.resize(r*c);
	return;
}
