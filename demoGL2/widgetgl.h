/*
 *project:demoGL2
 *author:macai
 *date:2013-3-4
 *email:12319597@qq.com
 *desc: 如何将这些彩色对象绕着坐标轴旋转,增加两个变量来控制这两个对象的旋转。
 *      它们是浮点类型的变量，使得我们能够非常精确地旋转对象。
 *      浮点数包含小数位置，这意味着我们无需使用1、2、3...的角度
 *教程来源于:http://www.qiliang.net/
 */

#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QtCore>
#include <QtOpenGL>

namespace Ui {
class WidgetGL;
}

class WidgetGL : public QGLWidget
{
    Q_OBJECT
    
public:
    explicit WidgetGL(QWidget *parent = 0, bool fullscreen = false);
    ~WidgetGL();
    
protected:
    /*QGLWidget类已内置了OpenGL的处理
     *通过对initializeGL,paintGL,resizeGL三个
     *函数实现.
    */
    void initializeGL();/*初始化OpenGL窗口部件*/
    void paintGL();/*绘制OpenGL的窗口,只要有更新发生，这个函数就会被调用*/
    void resizeGL(int w, int h);/*处理窗口大小变化,处理完后会自动刷新屏幕*/
    void keyPressEvent(QKeyEvent *);


private:
    Ui::WidgetGL *ui;
    bool m_fullscreen;

    GLfloat rTri;   //rTri的用来旋转三角形
    GLfloat rQuad; //rQuad 旋转四边形
};

#endif // WIDGETGL_H
