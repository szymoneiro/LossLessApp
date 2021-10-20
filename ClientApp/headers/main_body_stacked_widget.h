#ifndef MAINBODYSTACKEDWIDGET_H
#define MAINBODYSTACKEDWIDGET_H

#include <QtWidgets>

class MainBodyStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainBodyStackedWidget(QWidget *parent = nullptr);
private:
    QWidget *HomeWidget;
    QWidget *StatsWidget;
    QWidget *BuyWidget;
    QWidget *SellWidget;
    QWidget *SettingsWidget;
    QWidget *ProfileWidget;

};

#endif // MAINBODYSTACKEDWIDGET_H
