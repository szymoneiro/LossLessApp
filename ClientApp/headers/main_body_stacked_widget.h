#ifndef MAINBODYSTACKEDWIDGET_H
#define MAINBODYSTACKEDWIDGET_H

#include <QtWidgets>

class MainBodyStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainBodyStackedWidget(QWidget *parent = nullptr);
private:
    QWidget *home_widget;
    QWidget *stats_widget;
    QWidget *buy_widget;
    QWidget *sell_widget;
    QWidget *settings_widget;
    QWidget *profile_widget;

};

#endif // MAINBODYSTACKEDWIDGET_H
