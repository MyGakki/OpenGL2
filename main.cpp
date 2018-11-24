#include "windows.h"
#include <glut.h>
#include <math.h>
typedef struct {
    float x,y;
}point;
void init()
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
}
int LineDDA(int x0,int y0,int x1,int y1,point pixels[])//获得要画的线的点集合
{

    int num,x,y,dx,dy;
    float k,xf,yf;
    dx = x1-x0;
    dy = y1-y0;
    num = 0;
    k = dy*1.0/dx;
    if(dx==0) //垂直线
    {
        for(y=y0;y<=y1;y++)
        {
            pixels[num].x = x0;
            pixels[num].y = y;
            num ++;
        }

    }
    else if(dy==0) //水平线
    {
        for(x=x0;x<=x1;x++)
        {
            pixels[num].x = x;
            pixels[num].y = y0;
            num ++;
        }
    }
    else if(fabs(k)<=1)  //斜率绝对值小于1
    {
        yf = y0;
        for(x=x0;x<=x1;x++)
        {
            pixels[num].x = x;
            pixels[num].y = yf;
            num ++;
            yf += k;
        }
    }
    else
    {
        xf = x0;
        for(y=y0;y<=y1;y++)
        {
            pixels[num].x = xf;
            pixels[num].y = y;
            num ++;
            xf += 1/k;
        }
    }
    return num;
}

void draw8Point(int x0, int y0, int x, int y)  //画一个点关于圆心对称的另外8个点
{
    glBegin(GL_POINTS);
    glVertex2f(x0 + x, y0 + y);
    glVertex2f(x0 - x, y0 + y);
    glVertex2f(x0 + x, y0 - y);
    glVertex2f(x0 - x, y0 - y);
    glVertex2f(x0 + y, y0 + x);
    glVertex2f(x0 - y, y0 + x);
    glVertex2f(x0 + y, y0 - x);
    glVertex2f(x0 - y, y0 - x);
    glEnd();
}
void drawCircle(int x0, int y0, int r) //中点画圆算法
{
    int x = 0, y = r, d = 1 - r;
    while(x < y) {
        draw8Point(x0, y0, x, y);
        if(d < 0) {
            d += 2 * x +3;
            x ++;
        } else {
            d += 2*(x - y) + 5;
            x ++;
            y --;
        }
    }
}
void drawLine(int x1,int y1,int x2,int y2) //利用点集合画线
{
    point pixels[100];
    int num;
    int i;
    num = LineDDA(x1,y1,x2,y2,pixels);
    glBegin(GL_POINTS);
    for(i=0;i<num;i++)
        glVertex2f(pixels[i].x,pixels[i].y);
    glEnd();
}
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f,0.0f,0.f);
    drawCircle(10,10,50);
    drawLine(10,10,80,80);
    glFlush();
}
void ChangeSize(GLsizei w,GLsizei h)
{
    GLfloat aspectRatio;
    if(h==0)
        h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspectRatio = (GLfloat)w/(GLfloat)h;
    if(w<=h)
        glOrtho(-100.0,100.0,-100.0/aspectRatio,100.0/aspectRatio,1.0,-1.0);
    else
        glOrtho(-100.0*aspectRatio,100.0*aspectRatio,-100.0,100.0,1.0,-1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int main()
{
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutCreateWindow("LineDDA");

    init();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);

    glutMainLoop();
    return 0;
}
