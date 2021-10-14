#include "../headers/logo_and_side_bar_widget.h"

LogoAndSideBarWidget::LogoAndSideBarWidget(QWidget *parent) : QWidget(parent)
{
    create_side_bar();
    layout_set_up();
    create_logo_bar();
    create_buttons();

    left_widget_layout->addWidget(logo_bar_label);
    left_widget_layout->addWidget(side_bar_widget);

    side_bar_widget->setLayout(side_bar_layout);
    for (int i = 0; i < 4; ++i)
        side_bar_layout->addWidget(side_bar_buttons[i]);
}

void LogoAndSideBarWidget::layout_set_up()
{
    left_widget_layout = new QVBoxLayout(this);
    left_widget_layout->setSpacing(0);
    left_widget_layout->setContentsMargins(0, 0, 0, 0);

    side_bar_layout = new QVBoxLayout(side_bar_widget);
    side_bar_layout->setSpacing(8);
    side_bar_layout->setContentsMargins(12, 16, 12, 376);
}

void LogoAndSideBarWidget::create_logo_bar()
{
    logo_bar_label = new QLabel(this);
    logo_bar_label->setText("LOGO");
    logo_bar_label->setStyleSheet("background-color: #00FFA3;"
                            "color: #FFFFFF;"
                            "font: bold;");
    logo_bar_label->setAlignment(Qt::AlignCenter);
    logo_bar_label->setFixedSize(80, 80);
}

void LogoAndSideBarWidget::create_side_bar()
{
    side_bar_widget = new QWidget(this);
    side_bar_widget->setStyleSheet("background-color: #4C3099");
}

void LogoAndSideBarWidget::create_buttons()
{
    for (int i = 0; i < 4; ++i) {
        side_bar_buttons[i] = new QPushButton(side_bar_widget);
        side_bar_buttons[i]->setFixedSize(button_size, button_size);
        side_bar_buttons[i]->setStyleSheet("background-color: white;"
                                  "border-radius: 15px");
        QString icon_path = icons_path + icons_names[i];
        side_bar_buttons[i]->setIcon(QIcon(icon_path));
        side_bar_buttons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}
