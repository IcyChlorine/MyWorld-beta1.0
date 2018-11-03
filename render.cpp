#include"render.h"
#include"functions.h"
#include"Map.h"
#include"Character.h"
#include"texture.h"
#include"WinMain.h"
//#include<Windows.h>

extern Character hero;
extern Map map;
void Render()
{
	//extern float LightPositionMod[];
	ModifyLightPos();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度缓存
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE);

	hero.Transform();
	map.RenderAround(hero.GetCord());

	RenderGUI();
	glLoadIdentity();
	

	SwapBuffers(hDC);
	/*
	HBRUSH Purple_Brush = CreateSolidBrush(RGB(102, 0, 204));
	HBRUSH White_Brush = CreateSolidBrush(RGB(255, 255, 255));
	//HDC hdc;
	PAINTSTRUCT ps;

	//hdc = BeginPaint(hWnd, &ps);
	//1.选择紫色的画刷
	SelectObject(hDC, Purple_Brush);
	//2.画一个矩形       
	Rectangle(hDC, 50, 50, 450, 450);

	//3.选择白色的画刷
	SelectObject(hDC, White_Brush);
	//4.画一个圆
	Ellipse(hDC, 100, 100, 300, 300);
	//5.释放紫色画刷和白色画刷
	DeleteObject(Purple_Brush);
	DeleteObject(White_Brush);

	EndPaint(hWnd, &ps);*/
	return;								//  一切 OK

}

const int ITEM_BOX_WIDTH = 80;

void RenderGUI()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);

	glLoadIdentity();
	glBegin(GL_LINES);//画十字准星
	glVertex3f(0.05f, 0.0f, -0.5f);	
	glVertex3f(-0.05f, 0.0f, -0.5f);
	glVertex3f(-0.0f, -0.05f, -0.5f);
	glVertex3f(0.0f, 0.05f, -0.5f);
	glEnd();

	extern int wnd_height;
	extern int wnd_width;
	//glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(-wnd_width / 2, wnd_width / 2, -wnd_height / 2, wnd_height / 2);
	
	glBegin(GL_QUADS);//画物品栏
	glColor3f(0.7f, 0.7f, 0.7f);
	/*glVertex3f(-5 * ITEM_BOX_WIDTH, -wnd_height/2,0.0f);
	glVertex3f(5 * ITEM_BOX_WIDTH, -wnd_height/2,0.0f);
	glVertex3f(5 * ITEM_BOX_WIDTH, -wnd_height / 2+ITEM_BOX_WIDTH,0.0f);
	glVertex3f(-5 * ITEM_BOX_WIDTH, -wnd_height / 2 + ITEM_BOX_WIDTH,0.0f);*/
	glVertex3f(PXLTOGL(-5 * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2), -0.1f);
	glVertex3f(PXLTOGL(5 * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2), -0.1f);
	glVertex3f(PXLTOGL(5 * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH), -0.1f);
	glVertex3f(PXLTOGL(-5 * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH), -0.1f);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	//OnResize(wnd_width, wnd_height);
	extern int selected_item;
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(PXLTOGL((-6+selected_item) * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2), -0.1f);
	glVertex3f(PXLTOGL((-6 + selected_item+1) * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2), -0.1f);
	glVertex3f(PXLTOGL((-6 + selected_item + 1) * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH), -0.1f);
	glVertex3f(PXLTOGL((-6 + selected_item) * ITEM_BOX_WIDTH), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH), -0.1f);
	glEnd();

	extern HTEX texture[11][3];
	glEnable(GL_TEXTURE_2D);
	for (int i = 1; i <= 7; i++)
	{
		HTEX cur_tex = texture[i][1] ? texture[i][1] : texture[i][0];
		glBindTexture(GL_TEXTURE_2D, cur_tex);	//选择纹理

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(PXLTOGL((-6 + i) * ITEM_BOX_WIDTH+16), PXLTOGL(-wnd_height / 2+16), -0.1f);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(PXLTOGL((-6 + i + 1) * ITEM_BOX_WIDTH-16), PXLTOGL(-wnd_height / 2+16), -0.1f);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(PXLTOGL((-6 + i + 1) * ITEM_BOX_WIDTH-16), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH-16), -0.1f);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(PXLTOGL((-6 + i) * ITEM_BOX_WIDTH+16), PXLTOGL(-wnd_height / 2 + ITEM_BOX_WIDTH-16), -0.1f);
		glEnd();
		//-16和+16是为了让方块比格子缩进去一圈
	}
	return;
}

HTEX texture[11][3];
//每个方块的纹理有三个，[0]是上表面纹理，[1]是侧面纹理，[2]是底面纹理
//其中，[1] [2]为零代表侧面/底面纹理和上表面纹理一样
void Cube(float x, float y, float z, float a, int id)
{
	if (!id) return;
	glBindTexture(GL_TEXTURE_2D, texture[id][0]);	//选择纹理
	glBegin(GL_QUADS);
	// 上面
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + a, z + a);

	// 下面
	if (texture[id][2])
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[id][2]);	//选择纹理
		glBegin(GL_QUADS);
	}
	
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + 0, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + 0, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);

	// 后面
	if (texture[id][1] || texture[id][2])
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[id][1]);	//选择纹理
		glBegin(GL_QUADS);
	}

	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	// 前面
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + a);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + a);
	// 左面
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + 0, y + a, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + 0, y + 0, z + a);
	// 右面
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + a, y + 0, z + a);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glEnd();

	return;
}

float fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };					// 雾的颜色
float LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 				// 环境光参数
float LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };					// 漫射光参数
float LightPosition[] = { -1.0f, 2.0f, 3.0f, 0.0f };				// 光源位置
float LightScale[] = { 1.0f,1.0f,1.0f };
bool InitGL()								// 此处开始对OpenGL进行所有设置
{
	glShadeModel(GL_SMOOTH);					// 启用阴影平滑

	glEnable(GL_TEXTURE_2D);						// 启用纹理映射

	glClearColor(0.2f, 0.35f, 0.8f, 0.0f);			// 蓝色背景			

	//深度测试
	glClearDepth(1.0f);								// 设置深度缓存//？？？？？
	glEnable(GL_DEPTH_TEST);						// 启用深度测试
	glDepthFunc(GL_LEQUAL);						// 所作深度测试的类型

	//透视修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 告诉系统对透视进行修正

	//雾
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);			// 设置背景的颜色为雾气的颜色
	glFogi(GL_FOG_MODE, GL_LINEAR);		// 设置雾气的模式
	glFogfv(GL_FOG_COLOR, fogColor);			// 设置雾的颜色
	glFogf(GL_FOG_DENSITY, 0.05f);			// 设置雾的密度
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// 设置系统如何计算雾气
	glFogf(GL_FOG_START, 10.0f);				// 雾气的开始位置
	glFogf(GL_FOG_END, 16.0f);				// 雾气的结束位置
	glEnable(GL_FOG);					// 使用雾气

	//光照设置
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// 设置漫射光
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);			// 设置光源位置
	glEnable(GL_LIGHT1);							// 启用一号光源
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightScale);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LightScale);

	return true;								// 初始化 OK
}

bool InitGame()
{
	memset(texture, 0, sizeof(HTEX) * 33);
	if (!(texture[1][0]=LoadGLTexture("texture\\dirt.bmp")))							// 调用纹理载入子例程
		return false;							// 如果未能载入，返回FALSE
	if (!(texture[2][0]=LoadGLTexture("texture\\grass_top.bmp")))
		return false;	
	if (!(texture[2][1] = LoadGLTexture("texture\\grass_side.bmp")))
		return false;
	texture[2][2] = texture[1][0];//草方块底部和泥土一样
	if (!(texture[3][0] = LoadGLTexture("texture\\stone.bmp")))
		return false;
	if (!(texture[4][0] = LoadGLTexture("texture\\bedrock.bmp")))
		return false;
	if (!(texture[5][0] = LoadGLTexture("texture\\planks_spruce.bmp")))
		return false;
	if (!(texture[6][0] = LoadGLTexture("texture\\log_spruce_top.bmp")))
		return false;
	if (!(texture[6][1] = LoadGLTexture("texture\\log_spruce_side.bmp")))
		return false;
	if (!(texture[7][0] = LoadGLTexture("texture\\leaves_spruce.bmp")))
		return false;
	hero.MoveUp(3.0f);
	return true;
}

