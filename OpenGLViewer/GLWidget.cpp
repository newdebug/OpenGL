#include "GLWidget.h"

#include <qgl.h>
#include <GL/GLU.h>

#include <QMouseEvent>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_zFar(100.0),m_zNear(0.1)
{
    this->setMouseTracking(true);

    //value
    m_posText = tr("viewport x:0 y:0");
    m_list = glGenLists(3);
}

GLWidget::~GLWidget()
{
    // empty
}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 300);
}

// OpenGL operation functions
void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.128f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
//    glClearDepth( 1.0 );
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);    //�������2D������
    glPointSize(10);
    glShadeModel(GL_SMOOTH);
    glShadeModel(GL_POINT_SMOOTH);
    glShadeModel(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);//Դ�������Ӳ���alphaͨ��ֵ��Ŀ���������Ӳ���1.0
    glEnable(GL_BLEND);// ���û�ɫ
}

void GLWidget::paintGL()
{
    /*�����Ļ����Ȼ���*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();   //����ǰ���Ƶ���Ļ���ģ�X������������ң�Y������������ϣ�Z�������������
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);//������Ŀ��

    makeCurrent();
    drawAxis();
    renderImage();
    glLoadIdentity();
    renderHelpText();
    // 1
    //���Ҫ������һ���µ�ͼԪ������Ҫ�ٴε���glLoadIdentity()��Ŀ����Ϊ������ģ�͹۲����
    //���û�����ã�ֱ�ӵ���glTranslate�Ļ������������֮��Ľ����

    glLoadIdentity();
    drawLine();


//    glLoadIdentity();
//    drawPolygon();
}

void GLWidget::resizeGL(int w, int h)
{
    if( 0 == h) h = 1;
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*����͸��ͶӰ����*/
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, m_zNear, m_zFar);
//    glOrtho(0.0, w, h, 0.0, -m_zNear, m_zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // ��������ת����OpenGL����
//    glScalef(m_srtMatrix[0], m_srtMatrix[1], m_srtMatrix[2]);// ���š�ƽ�ơ���ת�任
//    glRotatef(m_srtMatrix[3], 1.0f, 0.0f, 0.0f);
//    glRotatef(m_srtMatrix[4], 0.0f, 1.0f, 0.0f);
//    glRotatef(m_srtMatrix[5], 0.0f, 0.0f, 1.0f);
//    glTranslatef(m_srtMatrix[6], m_srtMatrix[7], m_srtMatrix[8]);
    glGetIntegerv(GL_VIEWPORT, m_viewport); // �õ��������һ�������ӿڵĲ���:x,y,w,h
    glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    GLdouble x = winposToViewpos(event->pos()).vx;
    GLdouble y = winposToViewpos(event->pos()).vy;
    m_start = QPointF(x, y);
    if( m_end != m_start)
    {
        m_end = m_start;
        m_start = m_end;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        GLdouble x = winposToViewpos(event->pos()).vx;
        GLdouble y = winposToViewpos(event->pos()).vy;
        m_end = QPointF(x, y);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    GLdouble x = winposToViewpos(event->pos()).vx;
    GLdouble y = winposToViewpos(event->pos()).vy;
    m_end = QPointF(x, y);
    if( m_end != m_start)
    {
        updateGL();
    }
}

void GLWidget::renderImage()
{
//    makeCurrent();
//    glNewList(m_list, GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
        glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, 1.0f, 0.0f );
        glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f, 1.0f, 0.0f );
        glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, 0.0f );
//        glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 0.0f, 0.0f, -2.0f );
//        glTexCoord2f( 1.0f, 1.0f ); glVertex3f( m_width, 0.0, -2.0f );
//        glTexCoord2f( 1.0f, 0.0f ); glVertex3f( m_width, m_height, -2.0f );
//        glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 0.0, m_height, -2.0f );
    glEnd();
//    glEndList();
}

void GLWidget::updatePosText(const QString &text)
{
    m_posText = text;
}

void GLWidget::renderHelpText()
{
//    makeCurrent();
//    glNewList(m_list+1, GL_COMPILE);
    QString vpos = tr("%1 %2 %3").arg(viewpos.vx).arg(viewpos.vy).arg(viewpos.vz);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(0.0, 0.0, m_zNear, m_posText);
    glColor3f(1.0f, 0.5f, 0.0f);
    renderText(0.0, -0.1, m_zNear, "-FullScreen:F");
    renderText(0.0, -0.2, m_zNear, "-Exit:ESC");
    renderText(0.0, -0.3, m_zNear, vpos);
//    glEndList();
}

// ���������꾭������ת����OpenGL�ӿ�����
GLWidget::Viewpos GLWidget::winposToViewpos(const QPoint &pos)
{
    // ����������ת���Ӿ�������
    GLfloat winX, winY, winZ = 0.0;
    winX = pos.x();
    winY = m_viewport[3] - pos.y();

    // ��ת����OpenGL�ӿ�����
    GLdouble objX, objY, objZ;
    glReadPixels((int)winX, (int)winY, 1, 1,
                    GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, m_modelview, m_projection,
                    m_viewport, &objX, &objY, &objZ);

    viewpos.vx = objX;
    viewpos.vy = objY;
    viewpos.vz = objZ;

    return viewpos;
}

bool GLWidget::open(const QString &filename)
{
    QImage img;
    if ( !img.load(filename) )
    {
        qWarning("Cannot open the image...");
        return false;
    }

    QImage GL_formatted_image = convertToGLFormat(img);
    glGenTextures(1, &m_texture[0]);//����1�������ڴ棬����ָ��texture[0]
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);

    glTexImage2D( GL_TEXTURE_2D, 0, 3,
                GL_formatted_image.width(), GL_formatted_image.height(),
                0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits() );
//    m_width = GL_formatted_image.width();
//    m_height = GL_formatted_image.width();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );//GL_NEAREST �޾���ֲڣ��������

    updateGL();
    return true;
}

// ��x,y��
void GLWidget::drawAxis()
{
//    makeCurrent();
//    glNewList(m_list, GL_COMPILE);
    glBegin(GL_LINES);
        // x axis
        glLineWidth(8.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, m_zNear);
        glVertex3f(1.0f, 0.0f, m_zNear);
        // y axis
        glVertex3f(0.0f, 1.0f, m_zNear);
        glVertex3f(0.0f, -1.0f, m_zNear);
    glEnd();
//    glEndList();
}

void GLWidget::drawLine()
{
//    makeCurrent();
//    glNewList(m_list, GL_COMPILE);
    glBegin(GL_LINES);
        // x axis
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glVertex3f(m_start.x(), m_start.y(), -m_zFar);
        glVertex3f(m_end.x(), m_end.y(), -m_zFar);
    glEnd();
//    glEndList();
}

void GLWidget::drawPolygon()
{
//    makeCurrent();
    glPolygonMode(GL_BACK, GL_LINE);
    glBegin(GL_POLYGON);
        glVertex2f(m_start.x(), m_start.y());
        glVertex2f(m_start.x(), m_end.y());
        glVertex2f(m_end.x(), m_end.y());
        glVertex2f(m_end.x(), m_start.y());
    glEnd();
}
