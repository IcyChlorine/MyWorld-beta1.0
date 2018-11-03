#include<Windows.h>
#include<string>
#include<thread>
#include<gl/GL.h>
#include<glut.h>			// 包含OpenGL实用库

#include"WinMain.h"
#include"functions.h"
#include"render.h"
#include"Map.h"
using namespace std;

//全局变量的定义
MOUSEMSG mouse;									// 鼠标信息
bool	keys[256];										// 保存键盘按键的数组		

bool active{ false };										// 窗口的活动标志，缺省为TRUE
bool fullscreen{ true };									// 全屏标志缺省，缺省设定成全屏模式
bool cur_free{ false };									// 鼠标是否自由，缺省为false
int wnd_width{ 0 };									// 窗体宽度和高度
int wnd_height{ 0 };

HGLRC			hRC{ NULL };						// 窗口着色描述表句柄
HDC				hDC{ NULL };						// OpenGL渲染描述表句柄
HWND			hWnd{ NULL };						// 保存我们的窗口句柄
HINSTANCE		hInstance;							// 保存程序的实例

int WINAPI WinMain(HINSTANCE	hInstance,				// 当前窗口实例
							HINSTANCE	hPrevInstance,			// 前一个窗口实例
							LPSTR		lpCmdLine,				// 命令行参数
							int				nCmdShow)				// 窗口显示状态
{
#ifdef MY_DEBUG
	ofstream fout("debug_output.txt");
	ifstream fin("debug_input.txt");
	vector<float> data1 = { 1,1,0,1 };
	//vector<float> data2 = { 3,2,1 };

	Matrix m(2, 2, data1);
	Matrix E = GetIdentityMatrix(2);
	//Matrix n(1, 3, data2);
	for (int i = 0; i < 17; i++)
		E = E*m;
	fout << E;
	//fout << m+m<<m-n;
	//fout << m*n<<n*m;
	return 0;
#else
	// 游戏前准备：提示用户选择运行模式
	if (MessageBox(NULL, "你想在全屏模式下运行么？", "设置全屏模式", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;						// FALSE为窗口模式
	}
	// 游戏前准备：创建OpenGL窗口
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	if (!CreateGLWindow("NeHe's OpenGL程序框架", width,height, 16, fullscreen))
	{
		return 0;							// 失败退出
	}
	
	//游戏前准备：加载资源文件,i.e. load texture
	if (!InitGL())								// 初始化新建的GL窗口
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "初始化OpenGL失败", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	if (!InitGame())								// 初始化新建的GL窗口
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "初始化游戏数据失败", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	ShowCursor(false);		//游戏前准备：隐藏鼠标

	//计算帧率和游戏帧率所需要的变量。
	//每秒重新计算[t1,t2]时间段中的帧率和游戏帧率
	/*这里要引入一个新概念：游戏帧(game frame)。
	一个游戏帧是Act()模拟一次游戏运行的过程。
	重要的是，游戏帧率(gfps)应当保持不变：你不希望帧率是100时，相比帧率为50时，僵尸会以两倍的速度来攻击你;
	你也不希望当帧率是30时，你在游戏中走路的速度是帧率是60时的一半。
	这就要求普通帧（即渲染帧）不能完全与游戏帧同步。这也就是游戏时间正规化的目的。
	*/
	time_t t1, t2;
	int frm_cnt{ 0 };		//时间段中渲染的总帧数
	int gfrm_cnt{ 0 };		//~总游戏帧数
	float fps{ 60.0f };		//帧率，初始化为60
	float gfps{ 100.0f };	//游戏帧率，初始化为100，也就是预期gfps
	t1 = time(NULL);

	MSG		msg;			// Windowsx消息结构
	while (true)	//Main Loop							
	{
		static float render_threshold = 1;
		static float render_flag = 1;
		static int sleep_gap = 0;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))			// 若有消息，处理消息
		{
			if (msg.message == WM_QUIT)				// 收到退出消息?
				break;
			else							// 不是，处理窗口消息
			{
				TranslateMessage(&msg);				// 翻译消息
				DispatchMessage(&msg);				// 发送消息
			}
		}
		else								// 如果没有消息
		{
			//处理特殊按键情况
			if (keys[VK_F1])							//按 F1键可在全屏和窗口模式之间切换
			{
				keys[VK_F1] = FALSE;			// 若是，使对应的Key数组中的值为 FALSE
				DestroyGLWindow();				// 销毁当前的窗口
				fullscreen = !fullscreen;			// 切换 全屏 / 窗口 模式
														// 重建 OpenGL 窗口
				if (!CreateGLWindow("NeHe's OpenGL 程序框架", 640 * 1.5, 480 * 1.5, 16, fullscreen))
				{
					return 0;				// 如果窗口未能创建，程序退出
				}
			}
			if (keys[VK_ESCAPE])				// ESC 按下了么?
			{
				cur_free = !cur_free;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				ShowCursor(cur_free);
				keys[VK_ESCAPE] = false;
			}

			//游戏时间正规化
			t2 = time(NULL);
			if ((t2 - t1) >= 1)//周期性计算/更新游戏帧率和帧率
			{
				gfps = (float)gfrm_cnt / (t2 - t1);
				fps = (float)frm_cnt / (t2 - t1);
				t1 = t2;
				frm_cnt = 0;
				gfrm_cnt = 0;
				if (gfps > 110)
				{
					if (render_threshold > 1)
						render_threshold -= 0.1f;
					else
						sleep_gap++;
				}
				if (gfps < 90)
				{
					if (sleep_gap) sleep_gap--;
					else if (render_threshold <= 3)render_threshold += 0.1;
				}
			}
			//for (int i = 0; i < 1500000; i++);
			Act();gfrm_cnt++;//模拟游戏帧
			if (sleep_gap) Sleep(sleep_gap);
			render_flag += 1;
			if (render_flag >= render_threshold)
			{
				Render();frm_cnt++;//渲染
				render_flag -= render_threshold;
			}
		}
	}//end of Main Loop
		
#ifdef FPS_TEST
	ofstream fout("fps_test_output.txt");
	fout << "avg fps: " << fps << endl;
	fout << "avg gfps: " << gfps << endl;
	fout.close();
#endif
	// 关闭程序
	DestroyGLWindow();							// 销毁窗口
	return msg.wParam;							// 退出程序
#endif//DEBUG标记
}

LRESULT CALLBACK WndProc(HWND hWnd,					// 窗口的句柄
										UINT	uMsg,							// 窗口的消息
										WPARAM wParam,				// 附加的消息内容
										LPARAM	 lParam)					// 附加的消息内容
{
	switch (uMsg)								// 检查Windows消息
	{
	case WM_ACTIVATE:						// 监视窗口激活消息
		active =!HIWORD(wParam);			//一种比较简洁的写法 检查最小化状态
		return 0;									// 返回消息循环

	case WM_SYSCOMMAND:				// 系统中断命令
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)			// 检查系统调用
		case SC_SCREENSAVE:				// 屏保要运行?
		case SC_MONITORPOWER:			// 显示器要进入节电模式?
		return 0;									// 阻止发生
		break;										// 退出

	case WM_KEYDOWN:						// 有键按下么?
		keys[wParam] = true;					// 如果是，设为TRUE
		return 0;									// 返回
	case WM_KEYUP:							// 有键弹起么?
		keys[wParam] = false;					// 如果是，设为TRUE
		return 0;									// 返回
	case WM_MOUSEMOVE:					//鼠标消息
		mouse.x = LOWORD(lParam);		//获得鼠标位置
		mouse.y = HIWORD(lParam);
		return 0;
	case WM_LBUTTONDOWN:				//其他鼠标消息：左右键
		mouse.LButton = true;
		return 0;
	case WM_LBUTTONUP:
		mouse.LButton = false;
		return 0;
	case WM_RBUTTONDOWN:
		mouse.RButton = true;
		return 0;
	case WM_RBUTTONUP:
		mouse.RButton = false;
		return 0;

	case WM_SIZE:						// 调整OpenGL窗口大小
		OnResize(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height
		return 0;							// 返回
		
	case WM_CLOSE:					// 收到Close消息?
		extern Map map;
		map.Save();
		PostQuitMessage(0);			// 发出退出消息
		return 0;							// 返回
	}
	// 向 DefWindowProc传递所有未处理的消息
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool CreateGLWindow(string title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;					// 保存查找匹配的结果

	WNDCLASS	wc;								// 窗口类结构

	DWORD		dwExStyle;						// 扩展窗口风格
	DWORD		dwStyle;							// 窗口风格

	RECT WindowRect;							// 取得矩形的左上角和右下角的坐标值
	WindowRect.left = (long)0;					// 将Left   设为 0
	WindowRect.right = (long)width;			// 将Right  设为要求的宽度
	WindowRect.top = (long)0;					// 将Top    设为 0
	WindowRect.bottom = (long)height;		// 将Bottom 设为要求的高度

	wnd_width = width;							// 设置存储窗口宽高的全局变量
	wnd_height = height;
	fullscreen = fullscreenflag;					// 设置全局全屏标志

	hInstance = GetModuleHandle(NULL);							// 取得我们窗口的实例
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc处理消息
	wc.cbClsExtra = 0;													// 无额外窗口数据
	wc.cbWndExtra = 0;												// 无额外窗口数据
	wc.hInstance = hInstance;										// 设置实例
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					// 装入缺省图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// 装入鼠标指针
	wc.hbrBackground = NULL;										// GL不需要背景
	wc.lpszMenuName = NULL;										// 不需要菜单
	wc.lpszClassName = "OpenG";									// 设定类名字

	if (!RegisterClass(&wc))					// 尝试注册窗口类
	{
		MessageBox(NULL, "注册窗口失败", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 退出并返回FALSE
	}

	if (fullscreen)								// 要尝试全屏模式吗?
	{
		DEVMODE dmScreenSettings;											// 设备模式
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		// 确保内存清空为零
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth = width;								// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight = height;							// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel = bits;								// 每象素所选的色彩深度
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL, "全屏模式在当前显卡上设置失败！\n使用窗口模式？", "NeHe G", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;				// 选择窗口模式(Fullscreen=FALSE)
			}
			else
			{
				// 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL, "程序将被关闭", "错误", MB_OK | MB_ICONSTOP);
				return FALSE;					//  退出并返回 FALSE
			}
		}
	}
	if (fullscreen)								// 仍处于全屏模式吗?
	{
		dwExStyle = WS_EX_APPWINDOW;		// 扩展窗体风格
		dwStyle = WS_POPUP;						// 窗体风格
		ShowCursor(FALSE);							// 隐藏鼠标指针
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// 扩展窗体风格
		dwStyle = WS_OVERLAPPEDWINDOW;									//  窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小

	if (!(hWnd = CreateWindowEx(dwExStyle,				// 扩展窗体风格
		"OpenG",											// 类名字
		title.c_str(),										// 窗口标题
		WS_CLIPSIBLINGS |							// 必须的窗体风格属性
		WS_CLIPCHILDREN |							// 必须的窗体风格属性
		dwStyle,											// 选择的窗体属性
		0, 0,												// 窗口位置
		WindowRect.right - WindowRect.left,		// 计算调整好的窗口宽度
		WindowRect.bottom - WindowRect.top,	// 计算调整好的窗口高度
		NULL,												// 无父窗口
		NULL,												// 无菜单
		hInstance,										// 实例
		NULL)))											// 不向WM_CREATE传递任何东东
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能创建一个窗口设备描述表", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =			// /pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),			// 上述格式描述符的大小
		1,														// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,							// 必须支持双缓冲
		PFD_TYPE_RGBA,									// 申请 RGBA 格式
		bits,													// 选定色彩深度
		0, 0, 0, 0, 0, 0,										// 忽略的色彩位
		0,														// 无Alpha缓存
		0,														// 忽略Shift Bit
		0,														// 无累加缓存
		0, 0, 0, 0,											// 忽略聚集位
		16,													// 16位 Z-缓存 (深度缓存)
		0,														// 无蒙板缓存
		0,														// 无辅助缓存
		PFD_MAIN_PLANE,								// 主绘图层
		0,														// Reserved
		0, 0, 0												// 忽略层遮罩
	};

	if (!(hDC = GetDC(hWnd)))						// 取得设备描述表了么?
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能取得hDC", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))				// Windows 找到相应的象素格式了吗?
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能设置像素格式", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))	// 能够设置象素格式么?
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能设置像素格式", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))			// 能否取得着色描述表?
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能创建OpenGL渲染描述表", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))				// 尝试激活OpenGL渲染上下文
	{
		DestroyGLWindow();							// 重置显示区
		MessageBox(NULL, "不能激活当前的OpenGL渲然描述表", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// 返回 FALSE
	}

	ShowWindow(hWnd, SW_SHOW);				// 显示窗口
	SetForegroundWindow(hWnd);					// 略略提高优先级
	SetFocus(hWnd);									// 设置键盘的焦点至此窗口
	OnResize(width, height);							// 设置透视方式 

	return TRUE;										// 成功
}
void DestroyGLWindow()							// 正常销毁窗口
{
	if (fullscreen)									// 我们处于全屏模式吗?
	{
		ChangeDisplaySettings(NULL, 0);		// 是的话，切换回桌面
		ShowCursor(TRUE);						// 显示鼠标指针
	}
	if (hRC)											// 我们拥有OpenGL渲染描述表吗?
	{
		if (!wglMakeCurrent(NULL, NULL))	// 我们能否释放DC和RC描述表?
		{
			MessageBox(NULL, "释放DC或RC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))				// 我们能否删除RC?
		{
			MessageBox(NULL, "释放RC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;								// 将RC设为 NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC))		// 我们能否释放 DC?
	{
		MessageBox(NULL, "释放DC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;								// 将 DC 设为 NULL
	}
	if (hWnd && !DestroyWindow(hWnd))	// 能否销毁窗口?
	{
		MessageBox(NULL, "释放窗口句柄失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;								// 将 hWnd 设为 NULL
	}
	if (!UnregisterClass("OpenG", hInstance))	// 能否注销类?
	{
		MessageBox(NULL, "不能注销窗口类。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;							// 将 hInstance 设为 NULL
	}
}
