#include <iostream>
#include <cmath>
#include <iomanip>
#include <GL/glut.h>
using namespace std;
int width = 750, height = 750, typ = 0;
double T[25][25], err, tar, ansT[25][25][25], tt = 33.0;
double s(double x,double y,int tp){
	if(x!=0.45||y!=0.55)return 0.0;
	return (tp==1)?5.0:50000.0;
}
void Boundary_condition(double x,double y,int n){
	for(int i=0;i<n;++i){
		T[i][0]=T[i][n]=x;//30.0
		T[0][i]=y;//20.0	
	}
}
void SOR(int n,double w,double h,int tp,double bx,double by,int ptp){
	for(int i=0;i<=n;++i)//init
		for(int j=0;j<=n;++j)
			T[i][j]=0;
	err = INT_MAX;
	tar = 0.00001;
	Boundary_condition(bx,by,n);
	int ct=0;//SOR
	for(h*=h;err>tar;++ct){
		err = 0.0;
		for(int i=1;i<=n;++i){
			for(int j=1;j<n;++j){
				double nxt;
				nxt=(i!=n)?T[i][j]+(w/4.0)*(s(0.05*(i+1),0.05*(j+1),tp)*h+T[i-1][j]+T[i+1][j]+T[i][j-1]+T[i][j+1]-4.0*T[i][j]):T[i][j-1];
				err=max(abs(T[i][j]-nxt),err);
				T[i][j]=nxt;
			}
		}
	}
	cout << "Iterator Times: " << ct << "\n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j <= n; ++j) {
			ansT[i][j][typ] = T[i][j];
			if (ptp)cout << fixed << setprecision(3) << T[i][j] << setw(8);
		}
		if (ptp)cout << "\n";
	}
	if(ptp)typ++;
}
void display() {
	cout << "img " << typ << "\n";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//畫格子
	glBegin(GL_LINES);
	glColor3f(0.8, 0.8, 0.8);
	for (float i = -10; i < 10; ++i) {
		glVertex2f(-1.0, i / 10);
		glVertex2f(1.0, i / 10);
		glVertex2f(i / 10, 1.0);
		glVertex2f(i / 10, -1.0);
	}
	glEnd();
	//點塗色
	glPointSize(30);    //點的大小
	glBegin(GL_POINTS); //選擇畫點
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			double v = ansT[i][j][typ];
			if (v > tt*2.0) 
				glColor3f(((v - tt*2.0) / tt), 0, 0);
			else if (v < tt) 
				glColor3f(0, 0, v / tt);
			else 
				glColor3f(0, (v - tt) / tt, 0);
			glVertex2f((double)(j-10) / 10, (double)(i-10) / 10);
		}
	}
	glEnd();
	glFlush();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == '+') 
		typ=(typ+1)%4;
	else if (key == '-') 
		typ=(typ+3)%4;
	else if (key == 'z') {
		tt += 11.0;
		if (tt >= 55.0)tt = 11.0;
	}
	display();
}
int main(int argc, char** argv) {
	cout << "Gauss-Seidol method:\n";
	SOR(20,1.0,0.05,1,30.0,20.0,1);
	cout << "SOR method with w=1.2\n";
	SOR(20,1.2,0.05,1,30.0,20.0,1);
	cout << "boundary condition with bx = by = 100.0:\n";
	SOR(20,1.2,0.05,1,100.0,100.0,1);
	cout << "source function with s = 50000.0:\n";
	SOR(20,1.2,0.05,2,30.0,20.0,1);
	for(double i=1.0;i<2.0;i+=0.05){
		cout << "w = " << i << " ";
		SOR(20,i,0.05,1,30.0,20.0,0);
	}
	typ = 0;
	glutInit(&argc, argv);
	glutInitWindowPosition(400, 500);	//窗口初始位置
	glutInitWindowSize(width, height);	//窗口初始大小
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);	//設定模式
	glutCreateWindow("window");
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	display();
	glutMainLoop();
} 
