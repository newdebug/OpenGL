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
    bool open(const QString &filename);

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
    void renderImage();
    void renderHelpText();
    void drawAxis();
    void drawLine();
    void drawPolygon();

private:
    QString     m_posText;

    GLuint      m_list;
    GLuint      m_texture[1];
    GLint       m_viewport[4];//视口坐标
    GLdouble    m_modelview[16];//视景体坐标
    GLdouble    m_projection[16];
    GLfloat     m_srtMatrix[8];

    QPointF     m_start, m_end;

    GLdouble    m_zFar, m_zNear;

};

#endif // GLWIDGET_H
