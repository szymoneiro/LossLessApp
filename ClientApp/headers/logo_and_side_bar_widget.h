#ifndef LOGO_AND_SIDE_BAR_WIDGET_H
#define LOGO_AND_SIDE_BAR_WIDGET_H

#include <QtWidgets>

class LogoAndSideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoAndSideBarWidget(QWidget *parent = nullptr);
private:
    QString icons_path = "C:\\Users\\szymo\\Desktop\\Stock market application\\Qt\\LossLessAppV2\\";
    QStringList icons_names = {
        "home_icon.png",
        "stats_icon.png",
        "buy_icon.png",
        "sell_icon.png"
    };
    enum { button_size = 56, icon_size = 32 };

    /* Widgets layouts */
    QVBoxLayout *left_widget_layout;
    QVBoxLayout *side_bar_layout;

    /* Widgets */
    QLabel *logo_bar_label;
    QWidget *side_bar_widget;

    /* Buttons HOME/STATS/BUY/SELL */
    QPushButton *side_bar_buttons[4];

    /* Functions */
    void layout_set_up();
    void create_logo_bar();
    void create_side_bar();
    void create_buttons();

};

#endif // LOGO_AND_SIDE_BAR_WIDGET_H
