// #include <stdafx.h>

#include <iostream>
#include<GL/glut.h>
#include<math.h>
using namespace std;
#define A1 0.25 
#define B1 0.25
#define C1 0.25

#define D 30

#define K 0.08  //�˶�����������ϵ��


#define K3 2.f//����ѹǿ��س���
#define U4 20.f//ճ��ϵ��

#define U1 15.f  //ˮ������������
#define K4 2.f  //ˮ�����浯��ϵ��

#define PI 3.14159
#define H 0.75    //�⻬�˰뾶

#define M 1.f //��������
#define G -9.8 //��������

double t = 0.01; //ʱ�䲽��

#define bool int
#define false 0
#define true 1


				 //double scale = 1.f;

int  winWidth, winHeight;



float eye[3] = { 0,-0, 5 };  //�ӵ�
float at[3] = { 0,0, 0 };  //�۲��
float up[3] = { 0,1000, 0 };  //��������


typedef struct {
	double x, y, z;
	double vx, vy, vz;
	double pvx, pvy, pvz;
	double fx, fy, fz;
	double ve;  //�ܶ�
	double p;   //ѹ��
}Mess;
Mess mess[100][100];

float x1 = -2, x2 = 2, y11 = -1, y22 = 2, z1 = -2, z2 = 2;


#define PARTICLE_COUNTS			1000000

#define  GLUT_WHEEL_UP 3           //������ֲ���
#define  GLUT_WHEEL_DOWN 4

GLuint cubemap_tex;


int start, tend;
double angle = 0;
double rate = 0.03;
double tranlate = 0;
const float A = 1.0;
const float B = 1.0;
float C = 1;
float tranx = 0.0f;
float trany = 0.0f;
float tranz = 0.0f;

double anger_x = 0;
double anger_y = 0;
double anger_move_x = 0;
double anger_move_y = 0;

double shift_x = 0;
double shift_y = 0;
double shift_move_x = 0;
double shift_move_y = 0;
double scale = 1.0f;

int wndWidth = 500;
int wndHeight = 500;

float gl_ambient_ffuse[] = { 0.5, 0.5, 0.5, 0.3 };
void Init()
{
	glClearColor(1.0, 1.0, 1, 1.0);//���ñ�����ɫ
	glClearDepth(1.0);//������Ȼ���
	glEnable(GL_NORMALIZE); //���λ��
	glEnable(GL_DEPTH_TEST);//����Ȳ���
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);                        // ������Ӱƽ��

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//͸������

	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);   //����ӳ��

	double b = 0;
	for (int j = 0; j<D; j++) {
		double c = 1, a = 1;

		for (int i = 0; i<100; i++)
		{

			if (i % 10 == 0)
			{
				a = a - A1;
				c = 1;
			}


			mess[j][i].x = a; mess[j][i].y = b; mess[j][i].z = c;
			mess[j][i].fx = 0; mess[j][i].fy = G; mess[j][i].fz = 0;//����
			mess[j][i].ve = 0;
			mess[j][i].p = 0;

			c = c - C1;
		}
		b = b + B1;
	}

}

double ABS(double a)
{
	return a>0 ? a : -a;
}

void TimerFunction(int value)
{
	//printf("%d\n",value);
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunction, 1);
}

void color_set(float *mat_diffuse)
{

	float gl_specular2[] = { 0, 0, 0, 1 };
	float gl_shininess2[] = { 30 };
	float gl_emission2[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_specular2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_shininess2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_emission2);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--------����͸��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRotatef((anger_x + anger_move_x) * 90, 1.0f, 0.0f, 0.0f);
	glRotatef((anger_y + anger_move_y) * 90, 0.0f, 1.0f, 0.0f);

	glTranslatef((shift_x + shift_move_x) * 0.005, 0.0f, 0.0f);
	glTranslatef(0.0f, (shift_y + shift_move_y) * 0.005, 0.0f);

	glScalef(scale, scale, scale);



	gluPerspective(75.0, (float)wndWidth / (float)wndHeight, 0.1, 200.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float gl_position[] = { 1,-2,4,1 };
	float gl_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float gl_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float gl_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gl_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gl_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gl_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, gl_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);


	glScalef(scale, scale, scale);
	float ambient_diffuse1[4] = { 0,0,0,1 };
	color_set(ambient_diffuse1);
	for (int j = 0; j<D; j++) {

		for (int i = 0; i<100; i++)
		{
			mess[j][i].fx = 0; mess[j][i].fy = 0; mess[j][i].fz = 0;
			mess[j][i].ve = 0; mess[j][i].p = 0;

			for (int b = 0; b<D; b++)
			{
				for (int a = 0; a<100; a++)
				{

					float d = (mess[j][i].x - mess[b][a].x)*(mess[j][i].x - mess[b][a].x) + (mess[j][i].y - mess[b][a].y)*(mess[j][i].y - mess[b][a].y) + (mess[j][i].z - mess[b][a].z)*(mess[j][i].z - mess[b][a].z);
					if (d<H*H)
					{

						mess[j][i].ve += M * 315 * (H*H - d)*(H*H - d)*(H*H - d) / (64 * PI*H*H*H*H*H*H*H*H*H);  //�ܶ�

					}

				}
			}


			mess[j][i].p = K3*(mess[j][i].ve - 64);

		}
	}


	for (int j = 0; j<D; j++)
	{

		for (int i = 0; i<100; i++)
		{


			double yalix = 0, niandux = 0, yaliy = 0, nianduy = 0, yaliz = 0, nianduz = 0;
			for (int b = 0; b<D; b++)
			{
				for (int a = 0; a<100; a++)
				{

					float d = (mess[j][i].x - mess[b][a].x)*(mess[j][i].x - mess[b][a].x) + (mess[j][i].y - mess[b][a].y)*(mess[j][i].y - mess[b][a].y) + (mess[j][i].z - mess[b][a].z)*(mess[j][i].z - mess[b][a].z);

					if (b == j&&a == i)
					{
						//printf("j=%d\t,i=%d\n",j,i);
					}
					else
					{
						float d = (mess[j][i].x - mess[b][a].x)*(mess[j][i].x - mess[b][a].x) + (mess[j][i].y - mess[b][a].y)*(mess[j][i].y - mess[b][a].y) + (mess[j][i].z - mess[b][a].z)*(mess[j][i].z - mess[b][a].z);
						if (d<H*H)
						{
							yaliz += (((mess[j][i].p + mess[b][a].p)* (mess[j][i].z - mess[b][a].z) * ((H - sqrt(d))*(H - sqrt(d))) / (sqrt(d)  * (2 * mess[j][i].ve*mess[b][a].ve))));
							nianduz += (mess[b][a].vz - mess[j][i].vz)  * (H - sqrt(d)) / (mess[j][i].ve*mess[b][a].ve);

							yalix += (((mess[j][i].p + mess[b][a].p)* (mess[j][i].x - mess[b][a].x) * ((H - sqrt(d))*(H - sqrt(d))) / (sqrt(d)  * (2 * mess[j][i].ve*mess[b][a].ve))));
							niandux += (mess[b][a].vx - mess[j][i].vx)  * (H - sqrt(d)) / (mess[j][i].ve*mess[b][a].ve);

							yaliy += (((mess[j][i].p + mess[b][a].p)* (mess[j][i].y - mess[b][a].y) * ((H - sqrt(d))*(H - sqrt(d))) / (sqrt(d)  * (2 * mess[j][i].ve*mess[b][a].ve))));
							nianduy += (mess[b][a].vy - mess[j][i].vy)  * (H - sqrt(d)) / (mess[j][i].ve*mess[b][a].ve);

							//printf("yaliz:%f\t,nianduz:%f;\tyalix:%f\t,niandux:%f\n",yaliz,nianduz,yalix,niandux);
							//getchar();

						}
					}
				}
			}

			mess[j][i].fz += M * 45 * yaliz / (PI*H*H*H*H*H*H) + M * 45 * U4*nianduz / (PI*H*H*H*H*H*H);
			mess[j][i].fx += M * 45 * yalix / (PI*H*H*H*H*H*H) + M * 45 * U4*niandux / (PI*H*H*H*H*H*H);
			mess[j][i].fy += M * 45 * yaliy / (PI*H*H*H*H*H*H) + M * 45 * U4*nianduy / (PI*H*H*H*H*H*H);

		}
	}


	//int n=0,m=0;
	float diff;
	float fx, fy, fz;
    // glBegin(GL_TRIANGLE_STRIP);
	for (int j = 0; j<D; j++)
	{
		for (int i = 0; i<100; i++)
		{
			fx = mess[j][i].fx;
			fy = mess[j][i].fy;
			fz = mess[j][i].fz;
			fy += G;


			mess[j][i].pvx = 0.5*(mess[j][i].vx + mess[j][i].vx + fx*t);
			mess[j][i].pvy = 0.5*(mess[j][i].vy + mess[j][i].vy + fy*t);
			mess[j][i].pvz = 0.5*(mess[j][i].vz + mess[j][i].vz + fz*t);

			//�߽����
			diff = mess[j][i].z - (x1);
			if (0.2 - diff>0)
			{
				fz = 0.4*fz + (K4*(0.5 - diff) + (-mess[j][i].pvz)*U1);
				//mess[j][i].vx=0.2*ABS(mess[j][i].vx);
			}

			diff = x2 - mess[j][i].z;
			if (0.2 - diff>0)
			{
				fz = 0.4*fz + (-K4*(0.5 - diff) + (-mess[j][i].pvz)*U1);
				//mess[j][i].vx=-0.2*ABS(mess[j][i].vx);
			}

			diff = mess[j][i].x - (z1);
			if (0.2 - diff>0)
			{
				fx = 0.4*fx + (K4*(0.5 - diff) + (-mess[j][i].pvx)*U1);
				//mess[j][i].vz=0.2*ABS(mess[j][i].vz);
				;
			}


			diff = z2 - mess[j][i].x;
			if (0.2 - diff>0)
			{
				fx = 0.4*fx + (-K4*(0.5 - diff) + (-mess[j][i].pvx)*U1);
				//mess[j][i].vz=-0.2*ABS(mess[j][i].vz);
			}

			diff = mess[j][i].y - (y11);
			if (0.5 - diff>0)
			{
				fy = K4*(0.5 - diff) + (-mess[j][i].pvy)*U1;
				//mess[j][i].vy=-0.3*ABS(mess[j][i].vy);
			}


			mess[j][i].vx = mess[j][i].vx + fx*t;
			mess[j][i].vy = mess[j][i].vy + fy*t;
			mess[j][i].vz = mess[j][i].vz + fz*t;


			mess[j][i].x += mess[j][i].pvx*t;
			mess[j][i].y += mess[j][i].pvy*t;
			mess[j][i].z += mess[j][i].pvz*t;


			//if(ABS(mess[j][i].x)>=1)
			//n++;
			//if(ABS(mess[j][i].z)>=1)
			//m++;


			glPointSize(10.0f);
			glColor3f(0.0,0.5,0.3);
            // bigger(mess[j][i].x, mess[j][i].y, mess[j][i].z);
            // cout<<mess[j][i].x<<" "<< mess[j][i].y<<" "<<mess[j][i].z<<endl;
			glBegin(GL_POINTS);
			glVertex3f(mess[j][i].x, mess[j][i].y, mess[j][i].z);
			glEnd();

		}
	}
    // glEnd();

	float ambient_diffuse2[4] = { 0.5,0.8,0.5,0.3 };
	color_set(ambient_diffuse2);
	static int list = 0;
	if (list == 0)
	{
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);

		//glColor3f(1.0,0.5,0.3);
		glBegin(GL_QUADS);
		glVertex3d(x1, y11, z1);
		glVertex3d(x2, y11, z1);
		glVertex3d(x2, y11, z2);
		glVertex3d(x1, y11, z2);
		glEnd();
		//glColor3f(0.0,0.0,1.0);
		glBegin(GL_QUADS);
		glVertex3d(x1, y11, z1);
		glVertex3d(x2, y11, z1);
		glVertex3d(x2, y22, z1);
		glVertex3d(x1, y22, z1);
		glEnd();
		//glColor3f(0.0,1.0,0.0);
		glBegin(GL_QUADS);
		glVertex3d(x1, y11, z1);
		glVertex3d(x2, y11, z1);
		glVertex3d(x2, y22, z1);
		glVertex3d(x1, y22, z1);
		glEnd();
		//glColor3f(0.1,0.2,0.3);
		glBegin(GL_QUADS);
		glVertex3d(x1, y11, z1);
		glVertex3d(x1, y11, z2);
		glVertex3d(x1, y22, z2);
		glVertex3d(x1, y22, z1);
		glEnd();
		//glColor3f(0.0,1.0,0.1);
		glBegin(GL_QUADS);
		glVertex3d(x2, y11, z1);
		glVertex3d(x2, y11, z2);
		glVertex3d(x2, y22, z2);
		glVertex3d(x2, y22, z1);
		glEnd();

		glEndList();
	}

	glCallList(list);
	//printf("nx:=%d,\tmz=%d\n",n,m);
	glutSwapBuffers();
}

bool left_tag = false;
bool right_tag = false;
int mouse_x = 0;
int mouse_y = 0;

void processMouse(int button, int state, int x, int y) {

	if (state == GLUT_UP &&  button == GLUT_WHEEL_UP)
	{
		printf("GLUT_WHEEL_UP");
		scale -= 0.005;
	}
	if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
		scale += 0.005;
	if (state == GLUT_DOWN)
	{
		mouse_x = x; mouse_y = y;

		if (button == GLUT_LEFT_BUTTON)
			left_tag = true;
		if (button == GLUT_RIGHT_BUTTON)
			right_tag = true;
		//	cout << "left down!" << endl;
	}

	if (state == GLUT_UP)
	{
		left_tag = false;
		right_tag = false;
		if (button == GLUT_LEFT_BUTTON)
		{
			anger_x += anger_move_x;
			anger_y += anger_move_y;
			anger_move_x = 0;
			anger_move_y = 0;
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			shift_x += shift_move_x;
			shift_y += shift_move_y;
			shift_move_x = 0;
			shift_move_y = 0;

		}
		//	cout << "left up!" << endl;
	}

}

void processMouseActiveMotion(int x, int y) {

	if (left_tag)
	{
		anger_move_x = (y - mouse_y) * 1.0 / 800;
		anger_move_y = (x - mouse_x) * 1.0 / 800;
		//	cout << anger_x << endl;
	}
	if (right_tag)
	{
		shift_move_x = x - mouse_x;
		shift_move_y = mouse_y - y;
	}
}

/*void Keyboard(unsigned char key, int x, int y)     //���м�����Ϣʱ��ص��ú���

{

switch (key) {

case 'w':
{
scale += 0.01;
break;
}
case 's':
{
scale -= 0.01;
break;
}
}




}*/
void  Keyboard(unsigned char key, int x, int y)
{
	//int i;
	switch (key)
	{
	case 'w': case 'W':
		eye[1] -= 2;

		break;
	case 's': case 'S':
		eye[1] += 2;

		break;
	case 'a': case 'A':
		eye[0] += 2;

		break;
	case 'd': case 'D':
		eye[0] -= 2;

		break;

	case 't': case 'T':
		at[1] -= 2;
		break;
	case 'g': case 'G':
		at[1] += 2;
		break;
	case 'f': case 'F':
		at[0] += 2;
		break;
	case 'h': case 'H':
		at[0] -= 2;
		break;

	case 'i': case 'I':
		up[1] -= 2;
		break;
	case 'k': case 'K':
		up[1] += 2;
		break;
	case 'j': case 'J':
		up[0] += 2;
		break;
	case 'l': case 'L':
		up[0] -= 2;
		break;
	case 'o':
	{
		scale += 0.05;
		break;
	}
	case 'p':
	{
		scale -= 0.05;
		break;
	}
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wndWidth, wndHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("mass_spring");
	Init();

	glutDisplayFunc(Display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(20, TimerFunction, 1);
	glutMainLoop();
    return 0;
}

