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

    /* Layout */
    QHBoxLayout *mainLayout;

    /* Left-side and right-side widgets */
    LogoAndSideBarWidget *leftWidget;
    TopBarAndStackedWidget *rightWidget;

    /* Move borderless window */
    QPointF oldPos;

    /* Set window size and create main_layout object */
    void windowSetUp();
    void layoutSetUp();

    /* Moving application functions */
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};
#endif // MAIN_WIDGET_H
