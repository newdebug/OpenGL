#include "widgetgl.h"
#include "ui_widgetgl.h"
#include "qgl.h"
#include <GL/GLU.h>
#include <QDebug>

const GLfloat PI = 3.1415926536f;

WidgetGL::WidgetGL(QWidget *parent, bool fullscreen) :
    QGLWidget(parent),
    ui(new Ui::WidgetGL)
{
    rTri = 0.0;
    rQuad = 0.0;
    xRot = 0.0;
    yRot = 0.0;
    zRot = 0.0;
    zoom = 0.0;
    xpan = 0.0;
    ypan = 0.0;
    blend = false;
    bAnimate = false;

    left = 0.0;
    right = 0.0;
    bottom = 0.0;
    top = 0.0;

    m_fullscreen = fullscreen;
    this->setGeometry(10, 30, 640, 480);
    this->setWindowTitle(tr("Macai's OpenGL Framework demo4"));
    if(m_fullscreen)
        showFullScreen();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
}

WidgetGL::~WidgetGL()
{
    delete ui;
}

QSize WidgetGL::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize WidgetGL::sizeHint() const
{
    return QSize(200, 200);
}

void WidgetGL::initializeGL()
{
    glEnable(GL_TEXTURE_2D);//允许采用2D纹理技术
    /*启用smooth shading阴影平滑,置阴影平滑模式 */
    glShadeModel(GL_SMOOTH);

    /*设置清除屏幕时所用的颜色*/
    glClearColor(0.0f, 0.0f, 0.128f, 0.0f);//色彩范围从0.0到1.0,1.0为最亮情况,最后一个参数为Alpha值

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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);//源像素因子采用alpha通道值，目标像素因子采用1.0

    //将数组点连线
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0, starPoint);
}

void WidgetGL::paintGL()
{
    /*清除屏幕和深度缓存*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*重置当前的模型观察矩阵*/
    glLoadIdentity();   //将当前点移到了屏幕中心，X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外
    glTranslatef( -1.0, 0.0, zoom-6.0 );
    //glRotatef( Angle, Xvector, Yvector, Zvector )负责让对象绕某个轴旋转,
    //Angle 通常是个变量代表对象转过的角度,
    //Xvector，Yvector和Zvector三个参数则共同决定旋转轴的方向.
    glRotatef( xRot,  1.0,  1.0,  1.0 );//绕着Y轴从左向右旋转
    glRotatef( yRot,  1.0,  1.0,  1.0 );//绕着Y轴从左向右旋转
    glRotatef( zRot,  1.0,  1.0,  1.0 );//绕着Y轴从左向右旋转
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

    createFiveStar();
    /*绘制五角星*/
    glTranslatef( 1.5,  0.0, zoom-6.0  );
    glRotatef( xRot,  1.0,  1.0,  1.0 );
    glRotatef( yRot,  1.0,  1.0,  1.0 );
    glRotatef( zRot,  1.0,  1.0,  1.0 );
    //glColor3f( 1.0, 0.0, 0.0 );//一次性全部着同一色
    glBegin( GL_LINE_LOOP );
    glColor3f( 1.0, 0.0, 1.0 );
    glArrayElement(1);
    glColor3f( 1.0, 0.0, 0.0 );
    glArrayElement(4);
    glColor3f( 1.0, 1.0, 0.0 );
    glArrayElement(2);
    glColor3f( 0.0, 1.0, 0.0 );
    glArrayElement(0);
    glColor3f( 0.0, 0.0, 1.0 );
    glArrayElement(3);
    glEnd();

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
    //int side = qMin(w, h);
    //glViewport((w - side) / 2, (h - side) / 2, side, side);

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

void WidgetGL::resets()
{
    xRot = 0.0;
    yRot = 0.0;
    zRot = 0.0;

    zoom = 0.0;
    left = 0.0;
    ypan = 0.0;
    blend = false;
    bAnimate = false;

    left = 0.0;
    right = 0.0;
    bottom = 0.0;
    top = 0.0;

    glLoadIdentity();
    updateGL();
}

void WidgetGL::fullScreen(bool bFull /*bFull = false*/)
{
    if(bFull)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
    }
}

void WidgetGL::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
    case Qt::Key_Space:
        {
            resets();
        }
        break;
    case Qt::Key_F:
        m_fullscreen = !m_fullscreen;
        fullScreen(m_fullscreen);
        updateGL();
        break;
    case Qt::Key_L://是否使用色彩混合,高亮
        {
            blend = !blend;
            if(blend)
            {
                glEnable(GL_BLEND);//色彩融合和深度缓存不能同时开启
                glDisable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
            }
            updateGL();
        }
        break;
    case Qt::Key_Z:
        {
            bAnimate = !bAnimate;
            if( bAnimate )
                timer->start(40);
            else
                timer->stop();
        }
        break;
    case Qt::Key_Escape:
        close();
    }
}

void WidgetGL::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void WidgetGL::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        m_fullscreen = !m_fullscreen;
        fullScreen(m_fullscreen);
    }
}

void WidgetGL::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        rotateBy(1 * dy, 1 * dx, 0);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        rotateBy(1 * dy, 0, 1 * dx);
    }
    else if( event->buttons() & Qt::MidButton)
    {
        left = -dx;
        right = dx;
        bottom = -dy;
        top = dy;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D( left, right,  bottom, top);
        glMatrixMode(GL_MODELVIEW);
        updateGL();
    }
    lastPos = event->pos();
}

void WidgetGL::mouseReleaseEvent(QMouseEvent *event)
{
    //emit clicked();
}

void WidgetGL::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    //演示鼠标滚轮上下滚动时的变化
    zoom += numSteps;
    updateGL();
}

void WidgetGL::rotateOneStep()
{
    this->rotateBy(+2 * 1, +2 * 1, -2 * 1);
}

void WidgetGL::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;

    updateGL();
}

void WidgetGL::createFiveStar()
{
    short angle = 18;
    for( short i = 0; i < 5; ++i)
    {
        starPoint[i][0] = cos(angle * PI / 180);
        starPoint[i][1] = sin(angle * PI / 180);
        starPoint[i][2] = 0.0;      // z
        angle += 72;
    }
}

