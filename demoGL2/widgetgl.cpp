#include "widgetgl.h"
#include "ui_widgetgl.h"
#include "qgl.h"
#include <GL/GLU.h>

WidgetGL::WidgetGL(QWidget *parent, bool fullscreen) :
    QGLWidget(parent),
    ui(new Ui::WidgetGL)
{
    rTri = 0.0;
    rQuad = 0.0;

    //ui->setupUi(this);
    m_fullscreen = fullscreen;
    this->setGeometry(10, 30, 640, 480);
    this->setWindowTitle(tr("Macai's OpenGL Framework demo2"));
    if(m_fullscreen)
        showFullScreen();
}

WidgetGL::~WidgetGL()
{
    delete ui;
}

void WidgetGL::initializeGL()
{
    /*启用smooth shading阴影平滑*/
    glShadeModel(GL_SMOOTH);
    /*设置清除屏幕时所用的颜色*/
    glClearColor(0.0f, 0.2f, 0.6f, 0.0f);//色彩范围从0.0到1.0,1.0为最亮情况,最后一个参数为Alpha值

    /*
     *以下三行必须做的depth buffer深度缓存,深度缓存设想为屏幕后面的层.深度缓存不断的对物体进入屏幕内部有
     *多深进行跟踪.屏幕上显示的3D场景OpenGL程序都使用深度缓存,排序决定哪个物体先画.
     */
    /*设置深度缓存*/
    glClearDepth(1.0f);
    /*启用深度测试*/
    glEnable(GL_DEPTH_TEST);
    /*深度测试的类型*/
    glDepthFunc(GL_LEQUAL);

    /*真正精细的透视修正*/
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void WidgetGL::paintGL()
{
    /*清除屏幕和深度缓存*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*重置当前的模型观察矩阵*/
    glLoadIdentity();   //将当前点移到了屏幕中心，X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外

    glTranslatef( -1.5, 0.0, -6.5 );
    //glRotatef( Angle, Xvector, Yvector, Zvector )负责让对象绕某个轴旋转,
    //Angle 通常是个变量代表对象转过的角度,
    //Xvector，Yvector和Zvector三个参数则共同决定旋转轴的方向.
    glRotatef( rTri,  0.0,  1.0,  0.0 );//绕着Y轴从左向右旋转
    /*开始绘制三角形*/
    glBegin(GL_TRIANGLES);
    glColor3f( 1.0, 0.0, 0.0 );//着色,为每一个顶点
    glVertex3f(  0.0,   1.0, 0.0 );//上顶点
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0,  -1.0,  0.0 );//左下顶点
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0,  -1.0, 0.0 );//右下顶点
    glEnd();

    //增加了另一个glLoadIdentity()调用。目的是为了重置模型观察矩阵。如果没有重置，直接调用glTranslate的话，会出现意料之外的结果。
    glLoadIdentity();

    /*绘制四边形*/
    glTranslatef( 1.5,  0.0, -6.0  );
    glRotatef( rQuad,  1.0,  0.0,  0.0 );
    glColor3f( 0.5, 0.5, 1.0 );//一次性全部着同一色
    glBegin( GL_QUADS );
    glVertex3f( -1.0,  1.0,  0.0 );//左上顶点
    glVertex3f(  1.0,  1.0,  0.0 );//右上顶点
    glVertex3f(  1.0, -1.0,  0.0 );//右下顶点
    glVertex3f( -1.0, -1.0,  0.0 );//左下顶点
    glEnd();

    //使用键盘处理旋转,在keyPressEvent事件中A键
    //rTri += 2.0;
    //rQuad -= 6.0;
}

void WidgetGL::resizeGL(int w, int h)
{
    /*
     *创建了一个现实外观的场景。此处透视按照基于窗口宽度和高度的45度视角来计算.
     *0.1，100.0是我们在场景中所能绘制深度的起点和终点
     */

    /*防止height为0*/
    if( h == 0 )
    {
        h = 1;
    }
    /*重置投影矩阵*/
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode( GL_PROJECTION );//选择投影矩阵

    glLoadIdentity();//重置投影矩阵

    /*建立透视投影矩阵*/
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);

    /*选择模型观察矩阵*/
    glMatrixMode( GL_MODELVIEW );

    /*重置模型观察矩阵*/
    glLoadIdentity();
}

void WidgetGL::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
    case Qt::Key_F:
        m_fullscreen = !m_fullscreen;
        if(m_fullscreen)
        {
            showFullScreen();
        }
        else
        {
            showNormal();
            setGeometry(10, 30, 640, 480);
        }
        updateGL();
        break;
    case Qt::Key_A:
        rTri += 4.0;
        rQuad -= 8.0;
        updateGL();
        break;
    case Qt::Key_Escape:
        close();
    }
}

