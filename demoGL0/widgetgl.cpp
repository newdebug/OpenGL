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
    /*����smooth shading��Ӱƽ��*/
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
}

void WidgetGL::paintGL()
{
    /*�����Ļ����Ȼ���*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*���õ�ǰ��ģ�͹۲����*/
    glLoadIdentity();
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
    /*����ͶӰ����*/
    glViewport(0, 0, (GLint)w, (GLint)h);

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
