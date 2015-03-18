#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    struct Viewpos
    {
        GLdouble vx;
        GLdouble vy;
        GLdouble vz;
    }viewpos;

    Viewpos winposToViewpos(const QPoint &pos);

signals:

public slots:
    void updatePosText(const QString& text);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void renderHelpText();
    void drawAxis();
    void drawLine();

private:
    QString     m_posText;

    GLint       m_viewport[4];//�ӿ�����
    GLdouble    m_modelview[16];//�Ӿ�������
    GLdouble    m_projection[16];
    GLfloat     m_srtMatrix[8];

    QPointF     m_start, m_end;

    GLdouble    m_zFar, m_zNear;

};

#endif // GLWIDGET_H