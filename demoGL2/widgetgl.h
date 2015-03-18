/*
 *project:demoGL2
 *author:macai
 *date:2013-3-4
 *email:12319597@qq.com
 *desc: ��ν���Щ��ɫ����������������ת,�������������������������������ת��
 *      �����Ǹ������͵ı�����ʹ�������ܹ��ǳ���ȷ����ת����
 *      ����������С��λ�ã�����ζ����������ʹ��1��2��3...�ĽǶ�
 *�̳���Դ��:http://www.qiliang.net/
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
    /*QGLWidget����������OpenGL�Ĵ���
     *ͨ����initializeGL,paintGL,resizeGL����
     *����ʵ��.
    */
    void initializeGL();/*��ʼ��OpenGL���ڲ���*/
    void paintGL();/*����OpenGL�Ĵ���,ֻҪ�и��·�������������ͻᱻ����*/
    void resizeGL(int w, int h);/*�����ڴ�С�仯,���������Զ�ˢ����Ļ*/
    void keyPressEvent(QKeyEvent *);


private:
    Ui::WidgetGL *ui;
    bool m_fullscreen;

    GLfloat rTri;   //rTri��������ת������
    GLfloat rQuad; //rQuad ��ת�ı���
};

#endif // WIDGETGL_H
