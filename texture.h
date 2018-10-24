#pragma once
#include<glut.h>
#include<GLAUX.H>
#include<string>
using namespace std;

typedef GLuint HTEX;
//给texture的标识起一个好听一点的名字

AUX_RGBImageRec *LoadBMP(string Filename)					// 载入位图图象
{
	FILE *File = NULL;							// 文件

	if (Filename.empty())								// 确保文件名已提供
		return NULL;							// 如果没提供，返回 NULL

	File = fopen(Filename.c_str(), "r");						// 尝试打开文件

	if (File)								// 文件存在么?
	{
		fclose(File);							// 关闭句柄
		return auxDIBImageLoad(Filename.c_str());		// 载入位图并返回指针

	}

	return NULL;
}

HTEX LoadGLTexture(string FileName)								// 载入位图(调用上面的代码)并转换成纹理
{
	HTEX hTex;
	bool Status = false;								// 状态指示器
	AUX_RGBImageRec *TextureImage=NULL;					// 创建纹理的存储空间
	// 将指针设为 NULL

	// 载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage = LoadBMP(FileName))
	{
		Status = true;
		glGenTextures(1, &hTex);					// 创建纹理

		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, hTex);

		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// MC式滤波（马赛克的感觉）
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (TextureImage)							// 纹理是否存在
	{
		if (TextureImage->data)					// 纹理图像是否存在
		{
			free(TextureImage->data);				// 释放纹理图像占用的内存
		}
		free(TextureImage);						// 释放图像结构
	}
	if (!Status) hTex = 0;
	return hTex;								// 返回 hTex
}
