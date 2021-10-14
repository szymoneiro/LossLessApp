#ifndef TOP_BAR_AND_STACKED_WIDGET_H
#define TOP_BAR_AND_STACKED_WIDGET_H

#include <QtWidgets>

class TopBarAndStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopBarAndStackedWidget(QWidget *parent = nullptr);
private slots:
    void on_exit_button_clicked();
private:
    QString icons_path = "C:\\Users\\szymo\\Desktop\\Stock market application\\Qt\\LossLessAppV2\\";
    QStringList icons_names = {
        "settings_icon.png",
        "profile_icon.png",
        "exit_icon.png"
    };
    QStringList buttons_colors = {
        "#4C3099",
        "#00FFA3",
        "#FF004C"
    };
    enum { button_size = 56, icon_size = 30 };

    /* Layouts */
    QVBoxLayout *right_widget_layout;
    QHBoxLayout *top_bar_layout;

    /* Widgets */
    QWidget *top_bar_widget;
    QLabel *stacked_widget;

    /* Buttons SETTINGS/USER/EXIT */
    QPushButton *top_bar_buttons[3];

};

#endif // TOP_BAR_AND_STACKED_WIDGET_H
