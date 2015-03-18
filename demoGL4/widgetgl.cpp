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
    glEnable(GL_TEXTURE_2D);//�������2D������
    /*����smooth shading��Ӱƽ��,����Ӱƽ��ģʽ */
    glShadeModel(GL_SMOOTH);

    /*���������Ļʱ���õ���ɫ*/
    glClearColor(0.0f, 0.0f, 0.128f, 0.0f);//ɫ�ʷ�Χ��0.0��1.0,1.0Ϊ�������,���һ������ΪAlphaֵ

    /*
     *�������б�������depth buffer��Ȼ���,��Ȼ�������Ϊ��Ļ����Ĳ�.��Ȼ��治�ϵĶ����������Ļ�ڲ���
     *������и���.��Ļ����ʾ��3D����OpenGL����ʹ����Ȼ���,��������ĸ������Ȼ�.
     */
    /*������Ȼ���*/
    glClearDepth(1.0f);
    /*������Ȳ���*/
    glEnable(GL_DEPTH_TEST);
    /*��Ȳ��Ե�����*/
    glDepthFunc(GL_LEQUAL);
    /*������ϸ��͸������*/
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);//Դ�������Ӳ���alphaͨ��ֵ��Ŀ���������Ӳ���1.0

    //�����������
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0, starPoint);
}

void WidgetGL::paintGL()
{
    /*�����Ļ����Ȼ���*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*���õ�ǰ��ģ�͹۲����*/
    glLoadIdentity();   //����ǰ���Ƶ�����Ļ���ģ�X������������ң�Y������������ϣ�Z�������������
    glTranslatef( -1.0, 0.0, zoom-6.0 );
    //glRotatef( Angle, Xvector, Yvector, Zvector )�����ö�����ĳ������ת,
    //Angle ͨ���Ǹ������������ת���ĽǶ�,
    //Xvector��Yvector��Zvector����������ͬ������ת��ķ���.
    glRotatef( xRot,  1.0,  1.0,  1.0 );//����Y�����������ת
    glRotatef( yRot,  1.0,  1.0,  1.0 );//����Y�����������ת
    glRotatef( zRot,  1.0,  1.0,  1.0 );//����Y�����������ת
    /*��ʼ����������*/
    glBegin(GL_TRIANGLES);
    glColor3f( 1.0, 0.0, 0.0 );//��ɫ,Ϊÿһ������
    glVertex3f(  0.0,   1.0, 0.0 );//�϶���
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0,  -1.0,  0.0 );//���¶���
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0,  -1.0, 0.0 );//���¶���
    glEnd();

    //��������һ��glLoadIdentity()���á�Ŀ����Ϊ������ģ�͹۲�������û�����ã�ֱ�ӵ���glTranslate�Ļ������������֮��Ľ����
    glLoadIdentity();

    createFiveStar();
    /*���������*/
    glTranslatef( 1.5,  0.0, zoom-6.0  );
    glRotatef( xRot,  1.0,  1.0,  1.0 );
    glRotatef( yRot,  1.0,  1.0,  1.0 );
    glRotatef( zRot,  1.0,  1.0,  1.0 );
    //glColor3f( 1.0, 0.0, 0.0 );//һ����ȫ����ͬһɫ
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
     *������һ����ʵ��۵ĳ������˴�͸�Ӱ��ջ��ڴ��ڿ�Ⱥ͸߶ȵ�45���ӽ�������.
     *0.1��100.0�������ڳ��������ܻ�����ȵ������յ�
     */

    /*��ֹheightΪ0*/
    if( h == 0 )
    {
        h = 1;
    }
    //int side = qMin(w, h);
    //glViewport((w - side) / 2, (h - side) / 2, side, side);

    /*����ͶӰ����*/
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode( GL_PROJECTION );//ѡ��ͶӰ����
    glLoadIdentity();//����ͶӰ����

    /*����͸��ͶӰ����*/
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
    /*ѡ��ģ�͹۲����*/
    glMatrixMode( GL_MODELVIEW );
    /*����ģ�͹۲����*/
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
    case Qt::Key_L://�Ƿ�ʹ��ɫ�ʻ��,����
        {
            blend = !blend;
            if(blend)
            {
                glEnable(GL_BLEND);//ɫ���ںϺ���Ȼ��治��ͬʱ����
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
    //��ʾ���������¹���ʱ�ı仯
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

