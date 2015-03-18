#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bFullscreen(false)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    m_glViewer = new GLWidget(this);
    this->setCentralWidget(m_glViewer);

    this->installEventFilter(this);

    connect( this, SIGNAL(updatePosText(QString)),
            m_glViewer, SLOT(updatePosText(QString)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    QMainWindow::mouseMoveEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    if( event->type() == QEvent::HoverMove)
    {
        QHoverEvent* hover = static_cast<QHoverEvent*>(event);
        QString strPos = tr("viewport x:%1 y:%2")
                            .arg(hover->pos().x())
                            .arg(hover->pos().y());
        emit updatePosText(strPos);

//        qDebug() << strPos;
        m_glViewer->update();
        m_glViewer->winposToViewpos(hover->pos());
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F:
        this->m_bFullscreen = !this->m_bFullscreen;
        if( m_bFullscreen )
        {
            this->showFullScreen();
            m_glViewer->showFullScreen();
            this->menuBar()->hide();
            this->raise();
            m_glViewer->raise();
        }
        else
        {
            this->showNormal();
            m_glViewer->showNormal();
            this->menuBar()->show();
        }
        break;
    case Qt::Key_Escape:
        this->close();
        break;

    default:
        break;
    }


    QMainWindow::keyPressEvent(event);
}
