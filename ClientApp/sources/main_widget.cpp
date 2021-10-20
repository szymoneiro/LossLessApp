#include "../headers/main_widget.h"

/*  This is our main Widget, parent widget. Inside this class we create
    another widgets - sidebar, topbar, stackedWidget, logo widget etc. */
MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    windowSetUp();
    layoutSetUp();

    leftWidget = new LogoAndSideBarWidget(this);
    mainLayout->addWidget(leftWidget);

    rightWidget = new TopBarAndStackedWidget(this);
    mainLayout->addWidget(rightWidget);
}

void MainWidget::windowSetUp()
{
    this->setFixedSize(window_width, window_height);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint  | Qt::CustomizeWindowHint);
}

void MainWidget::layoutSetUp()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    oldPos = event->globalPosition();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    const QPointF delta = event->globalPosition() - oldPos;
    move(x()+delta.x(), y()+delta.y());
    oldPos = event->globalPosition();
}
