#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

TopBarAndStackedWidget::TopBarAndStackedWidget(QWidget *parent) : QWidget(parent)
{
    /* Create top bar widget */
    top_bar_widget = new QWidget(this);
    top_bar_widget->setStyleSheet("background-color: #FFFFFF");

    /* Create layouts */
    right_widget_layout = new QVBoxLayout(this);
    right_widget_layout->setSpacing(0);
    right_widget_layout->setContentsMargins(0, 0, 0, 0);

    top_bar_layout = new QHBoxLayout(top_bar_widget);
    top_bar_layout->setSpacing(26);
    top_bar_layout->setContentsMargins(954, 12, 26, 12);


    /* Stacked widget section */
    stacked_widget = new QLabel(this);
    stacked_widget->setText("STACKED_WIDGET");
    stacked_widget->setStyleSheet("background-color: #FAFAFA;"
                                  "color: black;"
                                  "font: bold");
    stacked_widget->setAlignment(Qt::AlignCenter);
    stacked_widget->setFixedSize(1200, 640);
    right_widget_layout->addWidget(stacked_widget);

    /* Buttons */
    for (int i = 0; i < 3; ++i) {
        top_bar_buttons[i] = new QPushButton(top_bar_widget);
        top_bar_buttons[i]->setFixedSize(button_size, button_size);
        top_bar_buttons[i]->setStyleSheet("background-color:" + buttons_colors[i] + ";"
                                          "border-radius: 15px");
        QString icon_path = icons_path + icons_names[i];
        top_bar_buttons[i]->setIcon(QIcon(icon_path));
        top_bar_buttons[i]->setIconSize(QSize(icon_size, icon_size));
    }

    /* Connect function to button */
    connect(top_bar_buttons[2], SIGNAL(clicked()), SLOT(on_exit_button_clicked()));
    connect(top_bar_buttons[0], SIGNAL(clicked()), SLOT(on_click_get()));

    right_widget_layout->addWidget(top_bar_widget);
    right_widget_layout->addWidget(stacked_widget);

    for (int i = 0; i < 3; ++i)
        top_bar_layout->addWidget(top_bar_buttons[i]);

    // Initialize network manager
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(managerFinished(QNetworkReply*)));
}

void TopBarAndStackedWidget::on_exit_button_clicked()
{
    QApplication::quit();
}

void TopBarAndStackedWidget::on_click_get()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies/1"));
    manager->get(request);
}

void TopBarAndStackedWidget::managerFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    stacked_widget->setText(reply->readAll());
}
