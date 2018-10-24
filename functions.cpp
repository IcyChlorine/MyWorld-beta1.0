#include"functions.h"
#include"Character.h"
#include"Map.h"
#include"render.h"
#include"WinMain.h"

int selected_item = 1;

void OnResize(GLsizei width, GLsizei height)				// 重置OpenGL窗口大小
{
	if (height == 0)								// 防止被零除
	{
		height = 1;								// 将Height设为1
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glLoadIdentity();							// 重置投影矩阵

	// 设置视口的大小
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// 选择模型观察矩阵
	glLoadIdentity();							// 重置模型观察矩阵

	extern int wnd_width;
	extern int wnd_height;
	wnd_width = width;
	wnd_height = height;
}

Character hero;
Map map;
extern float LightPosition[];
//float LightPositionMod[4];
//因为光源位置不会乘以模型观察矩阵，因此看上去光源在世界中的位置会不停乱动，和其他方块不一致
//因此要手动变换光源位置，将光源位置乘以模型观察矩阵，使其看上去相对于方块不动。
void ModifyLightPos()
{
	float LightPositionMod[4];
	//cord c(LightPosition[0], LightPosition[1], LightPosition[2]);
	
	float tmp[3];

	float theta = hero.GetTheta();
	float phi = hero.GetPhi();
	
	//cord r = GetRotationMatrix(DEGTORAD(theta), DEGTORAD(phi))*c;
	//模拟矩阵乘法
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
	
	glLightfv(GL_LIGHT1, GL_POSITION, LightPositionMod);			// 设置光源位置
	return;
}
void Act()
{
	/*//监视ESC键
	if (keys[VK_ESCAPE])				// ESC 按下了么?
	{
		cur_free = !cur_free;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowCursor(cur_free);
		keys[VK_ESCAPE] = false;

	}*/
	//ModifyLightPos();

	//人物移动
	float dx, dy, dz;
	//这里的x,z是指MC，不是WC(MC和WC的y方向一致)
	dx = (keys['D'] ? 0.025f : 0.0f) - (keys['A'] ? 0.025f : 0.0f);
	dy = (keys[VK_SPACE] ? 0.025f : 0.0f) - (keys[VK_CAPITAL] ? 0.025f : 0.0f);
	dz = (keys['W'] ? 0.025f : 0.0f) - (keys['S'] ? 0.025f : 0.0f);
	hero.MoveRight(dx);
	hero.MoveForward(dz);
	hero.MoveUp(dy);
	//碰撞体积检测，如果走过去发现有实体，不能走，那就走回来
	cord c = hero.GetCord();

	if ((map(c.x, c.y, c.z + 1) && ceil(c.z) - c.z < 0.25f) ||
		(map(c.x, c.y, c.z - 1) && c.z-floor(c.z) < 0.25f) ||
		(map(c.x, c.y - 1, c.z) && ceil(c.y) - c.y < 0.25f) ||
		(map(c.x, c.y +1, c.z) && c.y - floor(c.y) < 0.25f) ||
		(map(c.x-1, c.y, c.z) && ceil(c.x) - c.x < 0.25f) ||
		(map(c.x + 1, c.y, c.z) && c.x - floor(c.x) < 0.25f))
	{																//注意x和z不能这样做，因为x和z与WC中的x与z不重合
		hero.MoveRight(-dx);
		hero.MoveForward(-dz);
		hero.MoveUp(-dy);
	}
	//hero.MoveForward(-0.2f);//把+0.2f带来的影响消除
	//碰撞体积检测完成

	map.LoadAround(c.x, c.y, c.z);

	for (int i = 1; i <= 10; i++)//切换放置的方块
	{
		if (!i) i = 10;
		if (keys[i + '0'])
			selected_item = i;
	}
	if (!cur_free)//鼠标操作控制旋转
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
		//注意屏幕像素坐标和游戏坐标的y轴方向是反的
		//Character中，向左转theta为正，向上转phi为正

		SetCursorPos(200 - p.x, 200 - p.y);
	}
	
	return;	//程序继续运行
}
