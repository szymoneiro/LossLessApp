#include "../headers/main_widget.h"

/*  This is our main Widget, parent widget. Inside this class we create
    another widgets - sidebar, topbar, stackedWidget, logo widget etc. */
MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
    window_set_up();
    layout_set_up();

    left_widget = new LogoAndSideBarWidget(this);
    main_layout->addWidget(left_widget);

    right_widget = new TopBarAndStackedWidget(this);
    main_layout->addWidget(right_widget);
}

void MainWidget::window_set_up()
{
    this->setFixedSize(window_width, window_height);
}

void MainWidget::layout_set_up()
{
    /* Create layout */
    main_layout = new QHBoxLayout(this);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0,0,0,0);
}
