#include"functions.h"
#include"Character.h"
#include"Map.h"
#include"render.h"
#include"WinMain.h"

int selected_item = 1;

void OnResize(GLsizei width, GLsizei height)				// ����OpenGL���ڴ�С
{
	if (height == 0)								// ��ֹ�����
	{
		height = 1;								// ��Height��Ϊ1
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);		// ѡ��ͶӰ����
	glLoadIdentity();							// ����ͶӰ����

	// �����ӿڵĴ�С
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// ѡ��ģ�͹۲����
	glLoadIdentity();							// ����ģ�͹۲����

	extern int wnd_width;
	extern int wnd_height;
	wnd_width = width;
	wnd_height = height;
}

Character hero;
Map map;
extern float LightPosition[];
//float LightPositionMod[4];
//��Ϊ��Դλ�ò������ģ�͹۲������˿���ȥ��Դ�������е�λ�û᲻ͣ�Ҷ������������鲻һ��
//���Ҫ�ֶ��任��Դλ�ã�����Դλ�ó���ģ�͹۲����ʹ�俴��ȥ����ڷ��鲻����
void ModifyLightPos()
{
	float LightPositionMod[4];
	//cord c(LightPosition[0], LightPosition[1], LightPosition[2]);
	
	float tmp[3];

	float theta = hero.GetTheta();
	float phi = hero.GetPhi();
	
	//cord r = GetRotationMatrix(DEGTORAD(theta), DEGTORAD(phi))*c;
	//ģ�����˷�
	tmp[1] = LightPosition[1];
	tmp[2] = cos(DEGTORAD(-theta))*LightPosition[2]
		- sin(DEGTORAD(-theta))*LightPosition[0];
	tmp[0] = sin(DEGTORAD(-theta))*LightPosition[2]
		+ cos(DEGTORAD(-theta))*LightPosition[0];
	
	LightPositionMod[0] = tmp[0];
	LightPositionMod[1] = cos(DEGTORAD(-phi))*tmp[1]
		- sin(DEGTORAD(-phi))*tmp[2];
	LightPositionMod[2] = sin(DEGTORAD(-phi))*tmp[1]
		+ cos(DEGTORAD(-phi))*tmp[2];
	/*LightPositionMod[0] = r.x;
	LightPositionMod[1] = r.y;
	LightPositionMod[2] = r.z;*/
	LightPositionMod[3] = 0.0f;
	
	glLightfv(GL_LIGHT1, GL_POSITION, LightPositionMod);			// ���ù�Դλ��
	return;
}
void Act()
{
	/*//����ESC��
	if (keys[VK_ESCAPE])				// ESC ������ô?
	{
		cur_free = !cur_free;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowCursor(cur_free);
		keys[VK_ESCAPE] = false;

	}*/
	//ModifyLightPos();

	//�����ƶ�
	float dx, dy, dz;
	//�����x,z��ָMC������WC(MC��WC��y����һ��)
	dx = (keys['D'] ? 0.025f : 0.0f) - (keys['A'] ? 0.025f : 0.0f);
	dy = (keys[VK_SPACE] ? 0.025f : 0.0f) - (keys[VK_CAPITAL] ? 0.025f : 0.0f);
	dz = (keys['W'] ? 0.025f : 0.0f) - (keys['S'] ? 0.025f : 0.0f);
	hero.MoveRight(dx);
	hero.MoveForward(dz);
	hero.MoveUp(dy);
	//��ײ�����⣬����߹�ȥ������ʵ�壬�����ߣ��Ǿ��߻���
	cord c = hero.GetCord();

	if ((map(c.x, c.y, c.z + 1) && ceil(c.z) - c.z < 0.25f) ||
		(map(c.x, c.y, c.z - 1) && c.z-floor(c.z) < 0.25f) ||
		(map(c.x, c.y - 1, c.z) && ceil(c.y) - c.y < 0.25f) ||
		(map(c.x, c.y +1, c.z) && c.y - floor(c.y) < 0.25f) ||
		(map(c.x-1, c.y, c.z) && ceil(c.x) - c.x < 0.25f) ||
		(map(c.x + 1, c.y, c.z) && c.x - floor(c.x) < 0.25f))
	{																//ע��x��z��������������Ϊx��z��WC�е�x��z���غ�
		hero.MoveRight(-dx);
		hero.MoveForward(-dz);
		hero.MoveUp(-dy);
	}
	//hero.MoveForward(-0.2f);//��+0.2f������Ӱ������
	//��ײ���������

	map.LoadAround(c.x, c.y, c.z);

	for (int i = 1; i <= 10; i++)//�л����õķ���
	{
		if (!i) i = 10;
		if (keys[i + '0'])
			selected_item = i;
	}
	if (!cur_free)//������������ת
	{
		if (mouse.LButton)
		{
			cord vFwd = hero.GetForward();
			cord vPos = hero.GetCord();
			cord vTgt;
			for (float i = 0.0f; i < 5.0f; i += 0.05f)
			{
				vTgt.x = vPos.x + i*vFwd.x;
				vTgt.y = vPos.y + i*vFwd.y;
				vTgt.z = vPos.z + i*vFwd.z;
				if (map(vTgt.x, vTgt.y, vTgt.z))
				{
					map(vTgt.x, vTgt.y, vTgt.z) = 0;
					mouse.LButton = false;
					break;
				}
			}
		}
		if (mouse.RButton)
		{
			cord vFwd = hero.GetForward();
			cord vPos = hero.GetCord();
			cord vTgt = cord();
			for (float i = 0.0f; i < 5.0f; i += 0.05f)
			{
				vTgt.x = vPos.x + i*vFwd.x;
				vTgt.y = vPos.y + i*vFwd.y;
				vTgt.z = vPos.z + i*vFwd.z;
				if (map(vTgt.x, vTgt.y, vTgt.z))
				{
					vTgt.x -= 0.05f*vFwd.x;
					vTgt.y -= 0.05f *vFwd.y;
					vTgt.z -= 0.05f*vFwd.z;
					map(vTgt.x, vTgt.y, vTgt.z) = selected_item;
					mouse.RButton = false;
					break;
				}
			}

		}
		POINT p;
		p.x = 0, p.y = 0;
		ScreenToClient(hWnd, &p);
		hero.Rotate((200.0f - mouse.x)*0.03f, (200.0f - mouse.y)*0.03);
		//ע����Ļ�����������Ϸ�����y�᷽���Ƿ���
		//Character�У�����תthetaΪ��������תphiΪ��

		SetCursorPos(200 - p.x, 200 - p.y);
	}
	
	return;	//�����������
}
