#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GLWidget;
class QEvent;
class QMouseEvent;
class QKeyEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void updatePosText(const QString &posText);

protected:
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;
    GLWidget*       m_glViewer;
    bool            m_bFullscreen;
};

#endif // MAINWINDOW_H
