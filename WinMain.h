#pragma once

//调试开关
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
struct MOUSEMSG	// 鼠标信息结构体
{
	int x, y;
	bool LButton, RButton, MButton;
}; 
//为了避免重定义的链接错误，同时避免在每个cpp文件中都再extern一遍所有变量的苦恼，因而采取这样的方式：
//在WinMain.cpp中实际地定义变量，在头文件中extern一下变量，就相当于在所有cpp中都引用了一遍extern xx.
extern MOUSEMSG mouse;					// 鼠标信息
extern bool	keys[256];										// 保存键盘按键的数组		

extern bool active;										// 窗口的活动标志，缺省为TRUE
extern bool fullscreen;									// 全屏标志缺省，缺省设定成全屏模式
extern bool cur_free;									// 鼠标是否自由，缺省为false
extern int wnd_width;								// 窗体宽度和高度，初始化为0
extern int wnd_height;

extern HGLRC			hRC;							// 窗口着色描述表句柄，初始化为NULL
extern HDC				hDC;							// OpenGL渲染描述表句柄，初始化为NULL
extern HWND			hWnd;						// 保存我们的窗口句柄，初始化为NULL
extern HINSTANCE	hInstance;					// 保存程序的实例

//function prototypes
LRESULT CALLBACK WndProc(HWND hWnd,					// 窗口的句柄
										UINT	uMsg,							// 窗口的消息
										WPARAM wParam,				// 附加的消息内容
										LPARAM	 lParam);
//multi threads functions
void TimingThread();
void CalculateThread();
void RenderThread();


void DestroyGLWindow();
bool CreateGLWindow(string title, int width, int height, int bits, bool fullscreenflag);