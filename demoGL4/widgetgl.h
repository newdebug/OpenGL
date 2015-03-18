/*
 *project:demoGL2
 *author:macai
 *date:2013-3-4
 *email:12319597@qq.com
 *desc: 添加鼠标事件:绽放,旋转,色彩混合
 *教程来源于:http://www.qiliang.net/
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
    void rotateBy(int xAngle, int yAngle, int zAngle);  //旋转
    void createFiveStar();
    void resets();
    void fullScreen(bool bFull = false);

protected:
    /*QGLWidget类已内置了OpenGL的处理
     *通过对initializeGL,paintGL,resizeGL三个
     *函数实现.
    */
    void initializeGL();/*初始化OpenGL窗口部件*/
    void paintGL();/*绘制OpenGL的窗口,只要有更新发生，这个函数就会被调用*/
    void resizeGL(int w, int h);/*处理窗口大小变化,处理完后会自动刷新屏幕*/
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent * event);
signals:
    //void clicked();

public slots:
    void rotateOneStep();

private:
    Ui::WidgetGL *ui;
    bool m_fullscreen;
    QTimer *timer;

    GLfloat rTri;   //rTri的用来旋转三角形
    GLfloat rQuad; //rQuad 旋转四边形

    bool blend;//是否使用色彩混合
    bool bAnimate;

    // 平移
    double zoom;
    double xpan;
    double ypan;

    GLdouble left, right, bottom, top;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    GLuint textures[6];
    GLfloat starPoint[5][3];    // 五角星坐标
};

#endif // WIDGETGL_H
