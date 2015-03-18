/*
 *project:demoGL1
 *author:macai
 *date:2013-3-4
 *email:12319597@qq.com
 *desc:如何创建三角形和四边形。使用GL_TRIANGLES来创建一个三角形，GL_QUADS来创建一个四边形,并着色
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
};

#endif // WIDGETGL_H
