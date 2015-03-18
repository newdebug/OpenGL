#include "widgetgl.h"
#include "ui_widgetgl.h"
#include "qgl.h"
#include <GL/GLU.h>
#include <QDebug>


WidgetGL::WidgetGL(QWidget *parent, bool fullscreen) :
    QGLWidget(parent),
    ui(new Ui::WidgetGL)
{
    rTri = 0.0;
    rQuad = 0.0;
    xRot = 0;
    panH=0;
    panW=0;
    zoom = -6;
    blend = false;

//    yRot = 0;
//    zRot = 0;
    //ui->setupUi(this);
    m_fullscreen = fullscreen;
    this->setGeometry(10, 30, 640, 480);
    this->setWindowTitle(tr("Macai's OpenGL Framework demo3"));
    if(m_fullscreen)
        showFullScreen();

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    //timer->start(20);

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
    glClearColor(0.0f, 0.2f, 0.6f, 0.0f);//ɫ�ʷ�Χ��0.0��1.0,1.0Ϊ�������,���һ������ΪAlphaֵ

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
}

void WidgetGL::paintGL()
{
    /*�����Ļ����Ȼ���*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*���õ�ǰ��ģ�͹۲����*/
    glLoadIdentity();   //����ǰ���Ƶ�����Ļ���ģ�X������������ң�Y������������ϣ�Z�������������
    glTranslatef( -1.5, 0.0, zoom );
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

    /*�����ı���*/
    glTranslatef( 1.5,  0.0, zoom  );
    glRotatef( xRot,  1.0,  1.0,  1.0 );
    glRotatef( yRot,  1.0,  1.0,  1.0 );
    glRotatef( zRot,  1.0,  1.0,  1.0 );
    glColor3f( 0.5, 0.5, 1.0 );//һ����ȫ����ͬһɫ
    glBegin( GL_QUADS );
    glVertex3f( -1.0,  1.0,  0.0 );//���϶���
    glVertex3f(  1.0,  1.0,  0.0 );//���϶���
    glVertex3f(  1.0, -1.0,  0.0 );//���¶���
    glVertex3f( -1.0, -1.0,  0.0 );//���¶���
    glEnd();

    //ʹ�ü��̴�����ת,��keyPressEvent�¼���A��
    //rTri += 2.0;
    //rQuad -= 6.0;
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

void WidgetGL::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
    case Qt::Key_Space:
        {
            xRot = 0;
            yRot = 0;
            zRot = 0;
            panH= 0;
            panW = 0;
            zoom = -6;
            updateGL();
        }
        break;
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
    case Qt::Key_Escape:
        close();
    }
}

void WidgetGL::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void WidgetGL::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        rotateBy(2 * dy, 2 * dx, 0);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        rotateBy(2 * dy, 0, 2 * dx);
    }
    lastPos = event->pos();
    //qDebug() << lastPos;
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
    this->rotateBy(+2 * 16, +2 * 16, -1 * 16);
}

void WidgetGL::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;

    updateGL();
}
