#include "widgetgl.h"
#include "ui_widgetgl.h"
#include "qgl.h"
#include <GL/GLU.h>

WidgetGL::WidgetGL(QWidget *parent, bool fullscreen) :
    QGLWidget(parent),
    ui(new Ui::WidgetGL)
{
    //ui->setupUi(this);
    m_fullscreen = fullscreen;
    this->setGeometry(10, 30, 640, 480);
    this->setWindowTitle(tr("Macai's OpenGL Framework demo0"));
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
    glLoadIdentity();
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
    case Qt::Key_F2:
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
    case Qt::Key_Escape:
        close();
    }
}
