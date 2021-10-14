#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QtWidgets>
#include "logo_and_side_bar_widget.h"
#include "top_bar_and_stacked_widget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
private:
    enum { window_width = 1280, window_height = 720 };

    // Layout
    QHBoxLayout *main_layout;

    // Left-side and right-side widgets
    LogoAndSideBarWidget *left_widget;
    TopBarAndStackedWidget *right_widget;

    // Set window size and create main_layout object
    void window_set_up();
    void layout_set_up();

};
#endif // MAIN_WIDGET_H
