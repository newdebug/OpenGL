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
    glLoadIdentity();   //����ǰ���Ƶ�����Ļ���ģ�X������������ң�Y������������ϣ�Z�������������

    glTranslatef( -1.5, 0.0, -6.5 );
    //glRotatef( Angle, Xvector, Yvector, Zvector )�����ö�����ĳ������ת,
    //Angle ͨ���Ǹ������������ת���ĽǶ�,
    //Xvector��Yvector��Zvector����������ͬ������ת��ķ���.
    glRotatef( rTri,  0.0,  1.0,  0.0 );//����Y�����������ת
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
    glTranslatef( 1.5,  0.0, -6.0  );
    glRotatef( rQuad,  1.0,  0.0,  0.0 );
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

