#pragma once

//���Կ���
//#define MY_DEBUG
#ifdef MY_DEBUG
#include<vector>
#include"Matrix.h"
#else

#endif


#define FPS_TEST
#include<ctime>

#include<Windows.h>
#include<string>
using namespace std;

//global variables used for the whole application
struct MOUSEMSG	// �����Ϣ�ṹ��
{
	int x, y;
	bool LButton, RButton, MButton;
}; 
//Ϊ�˱����ض�������Ӵ���ͬʱ������ÿ��cpp�ļ��ж���externһ�����б����Ŀ��գ������ȡ�����ķ�ʽ��
//��WinMain.cpp��ʵ�ʵض����������ͷ�ļ���externһ�±��������൱��������cpp�ж�������һ��extern xx.
extern MOUSEMSG mouse;					// �����Ϣ
extern bool	keys[256];										// ������̰���������		

extern bool active;										// ���ڵĻ��־��ȱʡΪTRUE
extern bool fullscreen;									// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
extern bool cur_free;									// ����Ƿ����ɣ�ȱʡΪfalse
extern int wnd_width;								// �����Ⱥ͸߶ȣ���ʼ��Ϊ0
extern int wnd_height;

extern HGLRC			hRC;							// ������ɫ������������ʼ��ΪNULL
extern HDC				hDC;							// OpenGL��Ⱦ������������ʼ��ΪNULL
extern HWND			hWnd;						// �������ǵĴ��ھ������ʼ��ΪNULL
extern HINSTANCE	hInstance;					// ��������ʵ��

//function prototypes
LRESULT CALLBACK WndProc(HWND hWnd,					// ���ڵľ��
										UINT	uMsg,							// ���ڵ���Ϣ
										WPARAM wParam,				// ���ӵ���Ϣ����
										LPARAM	 lParam);
//multi threads functions
void TimingThread();
void CalculateThread();
void RenderThread();


void DestroyGLWindow();
bool CreateGLWindow(string title, int width, int height, int bits, bool fullscreenflag);