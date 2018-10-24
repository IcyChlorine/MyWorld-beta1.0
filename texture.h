#pragma once
#include<glut.h>
#include<GLAUX.H>
#include<string>
using namespace std;

typedef GLuint HTEX;
//��texture�ı�ʶ��һ������һ�������

AUX_RGBImageRec *LoadBMP(string Filename)					// ����λͼͼ��
{
	FILE *File = NULL;							// �ļ�

	if (Filename.empty())								// ȷ���ļ������ṩ
		return NULL;							// ���û�ṩ������ NULL

	File = fopen(Filename.c_str(), "r");						// ���Դ��ļ�

	if (File)								// �ļ�����ô?
	{
		fclose(File);							// �رվ��
		return auxDIBImageLoad(Filename.c_str());		// ����λͼ������ָ��

	}

	return NULL;
}

HTEX LoadGLTexture(string FileName)								// ����λͼ(��������Ĵ���)��ת��������
{
	HTEX hTex;
	bool Status = false;								// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage=NULL;					// ��������Ĵ洢�ռ�
	// ��ָ����Ϊ NULL

	// ����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage = LoadBMP(FileName))
	{
		Status = true;
		glGenTextures(1, &hTex);					// ��������

		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, hTex);

		// ��������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// MCʽ�˲��������˵ĸо���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (TextureImage)							// �����Ƿ����
	{
		if (TextureImage->data)					// ����ͼ���Ƿ����
		{
			free(TextureImage->data);				// �ͷ�����ͼ��ռ�õ��ڴ�
		}
		free(TextureImage);						// �ͷ�ͼ��ṹ
	}
	if (!Status) hTex = 0;
	return hTex;								// ���� hTex
}
