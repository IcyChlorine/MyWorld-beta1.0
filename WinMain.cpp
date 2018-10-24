#include<Windows.h>
#include<string>
#include<thread>
#include<gl/GL.h>
#include<glut.h>			// ����OpenGLʵ�ÿ�

#include"WinMain.h"
#include"functions.h"
#include"render.h"
#include"Map.h"
using namespace std;

#ifdef FPS_TEST
time_t t1, t2;
long long tot = 0;
long long gttot = 0;
float fps = 60.0f;
float gtfps = 120.0f;
#endif
namespace game_thread//�����߳�ͨ�ŵı���
{
	bool entried{ false };
	bool finished{ false };//�߳��Ƿ�ʼ���Ƿ�����ı�־

	bool calculate_flag{ false };//�Ƿ�Ҫcalculate
	bool render_flag{ false };//�Ƿ�Ҫrender
}

//ȫ�ֱ����Ķ���
MOUSEMSG mouse;									// �����Ϣ
bool	keys[256];										// ������̰���������		

bool active = true;										// ���ڵĻ��־��ȱʡΪTRUE
bool fullscreen = true;									// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
bool cur_free = false;									// ����Ƿ����ɣ�ȱʡΪfalse
int wnd_width = 0;									// �����Ⱥ͸߶�
int wnd_height = 0;

HGLRC			hRC = NULL;						// ������ɫ��������
HDC				hDC = NULL;						// OpenGL��Ⱦ��������
HWND			hWnd = NULL;						// �������ǵĴ��ھ��
HINSTANCE		hInstance;							// ��������ʵ��

int WINAPI WinMain(HINSTANCE	hInstance,				// ��ǰ����ʵ��
							HINSTANCE	hPrevInstance,			// ǰһ������ʵ��
							LPSTR		lpCmdLine,				// �����в���
							int				nCmdShow)				// ������ʾ״̬
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
	MSG		msg;								// Windowsx��Ϣ�ṹ

	// ��ʾ�û�ѡ������ģʽ
	if (MessageBox(NULL, "������ȫ��ģʽ������ô��", "����ȫ��ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;						// FALSEΪ����ģʽ
	}
	// ����OpenGL����
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	if (!CreateGLWindow("NeHe's OpenGL������", width,height, 16, fullscreen))
	{
		return 0;							// ʧ���˳�
	}
	
	//load texture
	if (!InitGL())								// ��ʼ���½���GL����
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "��ʼ��OpenGLʧ��", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!InitGame())								// ��ʼ���½���GL����
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "��ʼ����Ϸ����ʧ��", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

#ifdef FPS_TEST
	t1 = time(NULL);
#endif

	ShowCursor(false);		//�������
	while (true)	//Main Loop							
	{
		static float render_threshold = 1;
		static float render_flag = 1;
		static int sleep_gap = 0;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))			// ������Ϣ��������Ϣ
		{
			if (msg.message == WM_QUIT)				// �յ��˳���Ϣ?
				break;
			else							// ���ǣ���������Ϣ
			{
				TranslateMessage(&msg);				// ������Ϣ
				DispatchMessage(&msg);				// ������Ϣ
			}
		}
		else								// ���û����Ϣ
		{
			//�������ⰴ�����
			if (keys[VK_F1])							//�� F1������ȫ���ʹ���ģʽ֮���л�
			{
				keys[VK_F1] = FALSE;			// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE
				DestroyGLWindow();				// ���ٵ�ǰ�Ĵ���
				fullscreen = !fullscreen;			// �л� ȫ�� / ���� ģʽ
														// �ؽ� OpenGL ����
				if (!CreateGLWindow("NeHe's OpenGL ������", 640 * 1.5, 480 * 1.5, 16, fullscreen))
				{
					return 0;				// �������δ�ܴ����������˳�
				}
			}
			if (keys[VK_ESCAPE])				// ESC ������ô?
			{
				cur_free = !cur_free;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				ShowCursor(cur_free);
				keys[VK_ESCAPE] = false;
			}

			//��Ϸʱ�����滯
			t2 = time(NULL);
			if ((t2 - t1) >= 1)//�����Լ���/������Ϸ֡�ʺ�֡��
			{
				gtfps = (float)gttot / (t2 - t1);
				fps = (float)tot / (t2 - t1);
				t1 = t2;
				tot = 0;
				gttot = 0;
				if (gtfps > 110)
				{
					if (render_threshold > 1)
						render_threshold -= 0.1f;
					else
						sleep_gap++;
				}
				if (gtfps < 90)
				{
					if (sleep_gap) sleep_gap--;
					else if (render_threshold <= 3)render_threshold += 0.1;
				}
			}
			//for (int i = 0; i < 1500000; i++);
			Act();gttot++;
			if (sleep_gap) Sleep(sleep_gap);
			render_flag += 1;
			if (render_flag >= render_threshold)
			{
				Render();tot++;
				render_flag -= render_threshold;
			}
			
			
		}
	}
		
	
#ifdef FPS_TEST
	t2 = time(NULL);
	ofstream fout("fps_test_output.txt");

	fout << "avg fps: " << fps << endl;
	fout << "avg gtfps: " << gtfps << endl;
	fout.close();
#endif
	// �رճ���
	DestroyGLWindow();							// ���ٴ���
	return msg.wParam;							// �˳�����

#endif//DEBUG���
}

LRESULT CALLBACK WndProc(HWND hWnd,					// ���ڵľ��
										UINT	uMsg,							// ���ڵ���Ϣ
										WPARAM wParam,				// ���ӵ���Ϣ����
										LPARAM	 lParam)					// ���ӵ���Ϣ����
{
	switch (uMsg)								// ���Windows��Ϣ
	{
	case WM_ACTIVATE:						// ���Ӵ��ڼ�����Ϣ
		active =!HIWORD(wParam);			//һ�ֱȽϼ���д�� �����С��״̬
		return 0;									// ������Ϣѭ��

	case WM_SYSCOMMAND:						// ϵͳ�ж�����
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)						// ���ϵͳ����
		case SC_SCREENSAVE:				// ����Ҫ����?
		case SC_MONITORPOWER:				// ��ʾ��Ҫ����ڵ�ģʽ?
		return 0;					// ��ֹ����
		break;							// �˳�

	case WM_KEYDOWN:				// �м�����ô?
		keys[wParam] = true;			// ����ǣ���ΪTRUE
		return 0;								// ����
	case WM_KEYUP:					// �м�����ô?
		keys[wParam] = false;			// ����ǣ���ΪTRUE
		return 0;								// ����
	case WM_MOUSEMOVE:						//�����Ϣ
		mouse.x = LOWORD(lParam);			//������λ��
		mouse.y = HIWORD(lParam);
		return 0;
	case WM_LBUTTONDOWN:					//���������Ϣ�����Ҽ�
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

	case WM_SIZE:						// ����OpenGL���ڴ�С
		OnResize(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height
		return 0;						// ����
		
	case WM_CLOSE:							// �յ�Close��Ϣ?
		extern Map map;
		map.Save();
		PostQuitMessage(0);					// �����˳���Ϣ
		//game_thread::finished = true;
		return 0;						// ����
	}
	// �� DefWindowProc��������δ�������Ϣ��
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool CreateGLWindow(string title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;					// �������ƥ��Ľ��

	WNDCLASS	wc;								// ������ṹ

	DWORD		dwExStyle;						// ��չ���ڷ��
	DWORD		dwStyle;							// ���ڷ��

	RECT WindowRect;							// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left = (long)0;					// ��Left   ��Ϊ 0
	WindowRect.right = (long)width;			// ��Right  ��ΪҪ��Ŀ��
	WindowRect.top = (long)0;					// ��Top    ��Ϊ 0
	WindowRect.bottom = (long)height;		// ��Bottom ��ΪҪ��ĸ߶�

	wnd_width = width;							// ���ô洢���ڿ�ߵ�ȫ�ֱ���
	wnd_height = height;
	fullscreen = fullscreenflag;					// ����ȫ��ȫ����־

	hInstance = GetModuleHandle(NULL);							// ȡ�����Ǵ��ڵ�ʵ��
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc������Ϣ
	wc.cbClsExtra = 0;													// �޶��ⴰ������
	wc.cbWndExtra = 0;												// �޶��ⴰ������
	wc.hInstance = hInstance;										// ����ʵ��
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					// װ��ȱʡͼ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// װ�����ָ��
	wc.hbrBackground = NULL;										// GL����Ҫ����
	wc.lpszMenuName = NULL;										// ����Ҫ�˵�
	wc.lpszClassName = "OpenG";									// �趨������

	if (!RegisterClass(&wc))					// ����ע�ᴰ����
	{
		MessageBox(NULL, "ע�ᴰ��ʧ��", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// �˳�������FALSE
	}

	if (fullscreen)								// Ҫ����ȫ��ģʽ��?
	{
		DEVMODE dmScreenSettings;											// �豸ģʽ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth = width;								// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = height;							// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = bits;								// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬����
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�
			if (MessageBox(NULL, "ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��", "NeHe G", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;				// ѡ�񴰿�ģʽ(Fullscreen=FALSE)
			}
			else
			{
				// ����һ���Ի��򣬸����û��������
				MessageBox(NULL, "���򽫱��ر�", "����", MB_OK | MB_ICONSTOP);
				return FALSE;					//  �˳������� FALSE
			}
		}
	}
	if (fullscreen)								// �Դ���ȫ��ģʽ��?
	{
		dwExStyle = WS_EX_APPWINDOW;		// ��չ������
		dwStyle = WS_POPUP;						// ������
		ShowCursor(FALSE);							// �������ָ��
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// ��չ������
		dwStyle = WS_OVERLAPPEDWINDOW;									//  ������
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڴﵽ����Ҫ��Ĵ�С

	if (!(hWnd = CreateWindowEx(dwExStyle,				// ��չ������
		"OpenG",											// ������
		title.c_str(),										// ���ڱ���
		WS_CLIPSIBLINGS |							// ����Ĵ���������
		WS_CLIPCHILDREN |							// ����Ĵ���������
		dwStyle,											// ѡ��Ĵ�������
		0, 0,												// ����λ��
		WindowRect.right - WindowRect.left,		// ��������õĴ��ڿ��
		WindowRect.bottom - WindowRect.top,	// ��������õĴ��ڸ߶�
		NULL,												// �޸�����
		NULL,												// �޲˵�
		hInstance,										// ʵ��
		NULL)))											// ����WM_CREATE�����κζ���
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܴ���һ�������豸������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =			// /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR),			// ������ʽ�������Ĵ�С
		1,														// �汾��
		PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,							// ����֧��˫����
		PFD_TYPE_RGBA,									// ���� RGBA ��ʽ
		bits,													// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,										// ���Ե�ɫ��λ
		0,														// ��Alpha����
		0,														// ����Shift Bit
		0,														// ���ۼӻ���
		0, 0, 0, 0,											// ���Ծۼ�λ
		16,													// 16λ Z-���� (��Ȼ���)
		0,														// ���ɰ建��
		0,														// �޸�������
		PFD_MAIN_PLANE,								// ����ͼ��
		0,														// Reserved
		0, 0, 0												// ���Բ�����
	};

	if (!(hDC = GetDC(hWnd)))						// ȡ���豸��������ô?
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "����ȡ��hDC", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))				// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "�����������ظ�ʽ", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))	// �ܹ��������ظ�ʽô?
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "�����������ظ�ʽ", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))			// �ܷ�ȡ����ɫ������?
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܴ���OpenGL��Ⱦ������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))				// ���Լ���OpenGL��Ⱦ������
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "���ܼ��ǰ��OpenGL��Ȼ������", "����", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// ���� FALSE
	}

	ShowWindow(hWnd, SW_SHOW);				// ��ʾ����
	SetForegroundWindow(hWnd);					// ����������ȼ�
	SetFocus(hWnd);									// ���ü��̵Ľ������˴���
	OnResize(width, height);							// ����͸�ӷ�ʽ 

	return TRUE;										// �ɹ�
}
void DestroyGLWindow()							// �������ٴ���
{
	if (fullscreen)									// ���Ǵ���ȫ��ģʽ��?
	{
		ChangeDisplaySettings(NULL, 0);		// �ǵĻ����л�������
		ShowCursor(TRUE);						// ��ʾ���ָ��
	}
	if (hRC)											// ����ӵ��OpenGL��Ⱦ��������?
	{
		if (!wglMakeCurrent(NULL, NULL))	// �����ܷ��ͷ�DC��RC������?
		{
			MessageBox(NULL, "�ͷ�DC��RCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))				// �����ܷ�ɾ��RC?
		{
			MessageBox(NULL, "�ͷ�RCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;								// ��RC��Ϊ NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC))		// �����ܷ��ͷ� DC?
	{
		MessageBox(NULL, "�ͷ�DCʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;								// �� DC ��Ϊ NULL
	}
	if (hWnd && !DestroyWindow(hWnd))	// �ܷ����ٴ���?
	{
		MessageBox(NULL, "�ͷŴ��ھ��ʧ�ܡ�", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;								// �� hWnd ��Ϊ NULL
	}
	if (!UnregisterClass("OpenG", hInstance))	// �ܷ�ע����?
	{
		MessageBox(NULL, "����ע�������ࡣ", "�رմ���", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;							// �� hInstance ��Ϊ NULL
	}
}

void TimingThread()
{

	while (!(game_thread::entried)) Sleep(10);//�ȴ�ֱ���յ����̵߳�����
	while (!game_thread::finished)
	{
		//if (!active) Sleep(10);

		game_thread::calculate_flag = true;//��ʱ����ÿ8ms����һ��calculate��Ϣ
		Sleep(8);
	
	}
}
void CalculateThread()
{
	while (!game_thread::entried) Sleep(10);
	static int dt = 8;
	while (!game_thread::finished)
	{	
		/*if (!game_thread::calculate_flag) 
		{
			Sleep(10); 
			continue;
		}*/
		
		Sleep(8);
		Act();
#ifdef FPS_TEST
		gttot++;
#endif
		game_thread::calculate_flag = false;//��calculate��Ϣ���ڣ���ִ��һ��calculate
		game_thread::render_flag = true;//������render�߳���Ϣ�Ѿ����£��б�Ҫ������Ⱦ��
	}
}
void RenderThread()
{
	while (!game_thread::entried) Sleep(10);//�ȴ�ֱ���յ����̵߳�����

	/*ע�⣺OpenGL�����߳�Ϊ��λ�ģ�Ҳ����˵ÿһ���̻߳��в�ͬ��OpenGL��Ⱦ�����ġ�
	���ڴ����������߳��д����ģ����֮ǰ������HRC��OpenGL��Ⱦ�����ģ��Ǹ������߳��ߵ�
	�����Ҫ�����߳�����Ⱦ���ͱ��뽫�����߳��е�HRC��һЩͬ�������������߳��е�OpenGL��Ҫ
	��ĳЩ���ֵ����ٳ�ʼ��һ��
	/��ֱ�������߳��г�ʼ�������趨͸�䷽ʽ����������ȣ�
	����һЩ�ͷŹ���ҲҪ������߳�������һ��*/
	// ������Ⱦcontext
	HGLRC hRC_RTH = wglCreateContext(hDC);		//hRC for the Rendering THread
	wglMakeCurrent(hDC, hRC_RTH);
	// ������Դ
	wglShareLists(wglGetCurrentContext(), hRC_RTH);

	if (!InitGL())								// ��ʼ���½���GL����
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "��ʼ��OpenGLʧ��", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;							// ���� FALSE
	}
	if (!InitGame())								// ��ʼ���½���GL����
	{
		DestroyGLWindow();							// ������ʾ��
		MessageBox(NULL, "��ʼ����Ϸ����ʧ��", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;							// ���� FALSE
	}
	ShowWindow(hWnd, SW_SHOW);					// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);										// ���ü��̵Ľ������˴���
	OnResize(wnd_width, wnd_height);					// ����͸�Ӳ���
	//ShowCursor(false);		//�������

	while (!game_thread::finished)
	{
		if (!game_thread::render_flag)
		{
			Sleep(3);
			continue;
		}
		Render();//��calculate��Ϣ���ڣ���ִ��һ��render
		game_thread::render_flag = false;
#ifdef FPS_TEST
		tot++;
#endif
	}

	// �ͷű��̶߳��������opengl RC
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC_RTH);
}