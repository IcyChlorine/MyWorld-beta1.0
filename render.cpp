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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
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
	//1.ѡ����ɫ�Ļ�ˢ
	SelectObject(hDC, Purple_Brush);
	//2.��һ������       
	Rectangle(hDC, 50, 50, 450, 450);

	//3.ѡ���ɫ�Ļ�ˢ
	SelectObject(hDC, White_Brush);
	//4.��һ��Բ
	Ellipse(hDC, 100, 100, 300, 300);
	//5.�ͷ���ɫ��ˢ�Ͱ�ɫ��ˢ
	DeleteObject(Purple_Brush);
	DeleteObject(White_Brush);

	EndPaint(hWnd, &ps);*/
	return;								//  һ�� OK

}

const int ITEM_BOX_WIDTH = 80;

void RenderGUI()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);

	glLoadIdentity();
	glBegin(GL_LINES);//��ʮ��׼��
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
	
	glBegin(GL_QUADS);//����Ʒ��
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
		glBindTexture(GL_TEXTURE_2D, cur_tex);	//ѡ������

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
		//-16��+16��Ϊ���÷���ȸ�������ȥһȦ
	}
	return;
}

HTEX texture[11][3];
//ÿ�������������������[0]���ϱ�������[1]�ǲ�������[2]�ǵ�������
//���У�[1] [2]Ϊ��������/����������ϱ�������һ��
void Cube(float x, float y, float z, float a, int id)
{
	if (!id) return;
	glBindTexture(GL_TEXTURE_2D, texture[id][0]);	//ѡ������
	glBegin(GL_QUADS);
	// ����
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + a, z + a);

	// ����
	if (texture[id][2])
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[id][2]);	//ѡ������
		glBegin(GL_QUADS);
	}
	
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + 0, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + 0, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);

	// ����
	if (texture[id][1] || texture[id][2])
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[id][1]);	//ѡ������
		glBegin(GL_QUADS);
	}

	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	// ǰ��
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + a);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + a);
	// ����
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + 0, y + a, z + a);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0, y + a, z + 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0, y + 0, z + 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + 0, y + 0, z + a);
	// ����
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + a, y + a, z + a);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + a, y + 0, z + a);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + a, y + 0, z + 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + a, y + a, z + 0);
	glEnd();

	return;
}

float fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };					// �����ɫ
float LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 				// ���������
float LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };					// ��������
float LightPosition[] = { -1.0f, 2.0f, 3.0f, 0.0f };				// ��Դλ��
float LightScale[] = { 1.0f,1.0f,1.0f };
bool InitGL()								// �˴���ʼ��OpenGL������������
{
	glShadeModel(GL_SMOOTH);					// ������Ӱƽ��

	glEnable(GL_TEXTURE_2D);						// ��������ӳ��

	glClearColor(0.2f, 0.35f, 0.8f, 0.0f);			// ��ɫ����			

	//��Ȳ���
	glClearDepth(1.0f);								// ������Ȼ���//����������
	glEnable(GL_DEPTH_TEST);						// ������Ȳ���
	glDepthFunc(GL_LEQUAL);						// ������Ȳ��Ե�����

	//͸������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ����ϵͳ��͸�ӽ�������

	//��
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);			// ���ñ�������ɫΪ��������ɫ
	glFogi(GL_FOG_MODE, GL_LINEAR);		// ����������ģʽ
	glFogfv(GL_FOG_COLOR, fogColor);			// ���������ɫ
	glFogf(GL_FOG_DENSITY, 0.05f);			// ��������ܶ�
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// ����ϵͳ��μ�������
	glFogf(GL_FOG_START, 10.0f);				// �����Ŀ�ʼλ��
	glFogf(GL_FOG_END, 16.0f);				// �����Ľ���λ��
	glEnable(GL_FOG);					// ʹ������

	//��������
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// ���������
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);			// ���ù�Դλ��
	glEnable(GL_LIGHT1);							// ����һ�Ź�Դ
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightScale);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LightScale);

	return true;								// ��ʼ�� OK
}

bool InitGame()
{
	memset(texture, 0, sizeof(HTEX) * 33);
	if (!(texture[1][0]=LoadGLTexture("texture\\dirt.bmp")))							// ������������������
		return false;							// ���δ�����룬����FALSE
	if (!(texture[2][0]=LoadGLTexture("texture\\grass_top.bmp")))
		return false;	
	if (!(texture[2][1] = LoadGLTexture("texture\\grass_side.bmp")))
		return false;
	texture[2][2] = texture[1][0];//�ݷ���ײ�������һ��
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

