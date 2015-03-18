/*
 *project:demoGL2
 *author:macai
 *date:2013-3-4
 *email:12319597@qq.com
 *desc: �������¼�:����,��ת,ɫ�ʻ��
 *�̳���Դ��:http://www.qiliang.net/
 */

#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QtCore>
#include <QtOpenGL>
#include <QMouseEvent>

namespace Ui {
class WidgetGL;
}

class WidgetGL : public QGLWidget
{
    Q_OBJECT
    
public:
    explicit WidgetGL(QWidget *parent = 0, bool fullscreen = false);
    ~WidgetGL();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);  //��ת

protected:
    /*QGLWidget����������OpenGL�Ĵ���
     *ͨ����initializeGL,paintGL,resizeGL����
     *����ʵ��.
    */
    void initializeGL();/*��ʼ��OpenGL���ڲ���*/
    void paintGL();/*����OpenGL�Ĵ���,ֻҪ�и��·�������������ͻᱻ����*/
    void resizeGL(int w, int h);/*�����ڴ�С�仯,���������Զ�ˢ����Ļ*/
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    //void clicked();

public slots:
    void rotateOneStep();

private:
    Ui::WidgetGL *ui;
    bool m_fullscreen;

    GLfloat rTri;   //rTri��������ת������
    GLfloat rQuad; //rQuad ��ת�ı���

    bool blend;//�Ƿ�ʹ��ɫ�ʻ��
    double zoom;
    GLdouble panH, panW;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    GLuint textures[6];
};

#endif // WIDGETGL_H
