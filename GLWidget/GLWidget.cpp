#include "GLWidget.h"

#include <qgl.h>
#include <GL/GLU.h>

#include <QMouseEvent>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_zFar(1.0),m_zNear(0.1)
{
    this->setMouseTracking(true);

    //value
    m_posText = tr("viewport x:0 y:0");
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
    glEnable(GL_TEXTURE_2D);    //�������2D������
    glShadeModel(GL_SMOOTH);
    glShadeModel(GL_POINT_SMOOTH);
    glShadeModel(GL_LINE_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);//Դ�������Ӳ���alphaͨ��ֵ��Ŀ���������Ӳ���1.0
}

void GLWidget::paintGL()
{
    /*�����Ļ����Ȼ���*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();   //����ǰ���Ƶ���Ļ���ģ�X������������ң�Y������������ϣ�Z�������������

    // ��ʼ����ͼԪ
    // 0
    drawAxis();

    glLoadIdentity();
    renderHelpText();
    // 1
    //���Ҫ������һ���µ�ͼԪ������Ҫ�ٴε���glLoadIdentity()��Ŀ����Ϊ������ģ�͹۲����
    //���û�����ã�ֱ�ӵ���glTranslate�Ļ������������֮��Ľ����
    glLoadIdentity();
    drawLine();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if( 0 == h) h = 1;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*����͸��ͶӰ����*/
//    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, m_zNear, m_zFar);

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
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLdouble x = winposToViewpos(event->pos()).vx;
    GLdouble y = winposToViewpos(event->pos()).vy;
    m_end = QPointF(x, y);
    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    GLdouble x = winposToViewpos(event->pos()).vx;
    GLdouble y = winposToViewpos(event->pos()).vy;
    m_end = QPointF(x, y);
}

void GLWidget::updatePosText(const QString &text)
{
    m_posText = text;
}

void GLWidget::renderHelpText()
{
    QString vpos = tr("%1 %2 %3").arg(viewpos.vx).arg(viewpos.vy).arg(viewpos.vz);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(0.0, 0.0, m_zNear, m_posText);
    glColor3f(1.0f, 0.5f, 0.0f);
    renderText(0.0, -0.1, m_zNear, "-FullScreen:F");
    renderText(0.0, -0.2, m_zNear, "-Exit:ESC");
    renderText(0.0, -0.3, m_zNear, vpos);
}

// ���������꾭������ת����OpenGL�ӿ�����
GLWidget::Viewpos GLWidget::winposToViewpos(const QPoint &pos)
{
    // ����������ת���Ӿ�������
    GLfloat winX, winY, winZ = 1.0;
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

// ��x,y��
void GLWidget::drawAxis()
{
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
}

void GLWidget::drawLine()
{
    glBegin(GL_LINES);
        // x axis
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glVertex3f(m_start.x(), m_start.y(), -m_zFar);
        glVertex3f(m_end.x(), m_end.y(), -m_zFar);
    glEnd();
}
