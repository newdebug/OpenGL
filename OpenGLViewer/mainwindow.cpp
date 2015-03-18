#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QFileDialog>
#include <QMessageBox>
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
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(actionOpen()));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
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
    case Qt::Key_O:
        actionOpen();
        break;
    default:
        break;
    }


    QMainWindow::keyPressEvent(event);
}

void MainWindow::actionOpen()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                tr("Open File"), QDir::currentPath());
    if (!filename.isEmpty())
    {
        QImage image(filename);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("OpenGL Viewer"),
                                        tr("Cannot load %1.").arg(filename));
            return;
        }
        m_glViewer->open(filename);
    }
}
