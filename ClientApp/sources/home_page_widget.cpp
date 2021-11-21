#include "../headers/home_page_widget.h"
#include <QNetworkReply>

HomePageWidget::HomePageWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");

    buttonsCreate();
    createRecords();
    scrollAreaCreate();

    connectionManager = new QNetworkAccessManager(this);
    connect(connectionManager, SIGNAL(finished(QNetworkReply*)),
            SLOT(obtainingRecordsFinished*));
}

void HomePageWidget::buttonsCreate()
{
    const int button_width = 150;
    const int button_height = 46;
    const int spacing = 16;
    QStringList colours = {
        "00FFA3;",
        "C8C8C8;",
        "C8C8C8;"
    };

    for (int i = 0; i < 3; ++i) {
        tabButtons[i] = new QPushButton(buttonsNames[i], this);
        tabButtons[i]->setStyleSheet("background-color: #" + colours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
        tabButtons[i]->setGeometry(28 + i * (button_width + spacing),
                                   48,
                                   button_width,
                                   button_height);
        tabButtons[i]->setFont(QFont("Lato"));
    }
}

void HomePageWidget::createRecords()
{
    const int label_width = 198;
    const int label_height = 30;
    const int label_spacing = 32;

    for (int i = 0; i < 40; ++i) {
        records[i] = new QWidget(this);
        for (int j = 0; j < 4; ++j) {
            records_labels[i * j] = new QLabel(mock_data[j], records[i]);
            records_labels[i * j]->setStyleSheet("background-color: #FFFFFF;"
                                                 "border-radius: 10px;"
                                                 "border: 2px solid #000000;"
                                                 "font-size: 14px;"
                                                 "font-weight: bold");
            records_labels[i * j]->setFont(QFont("Lato"));
            records_labels[i * j]->setGeometry(32 + j*(label_spacing + label_width),
                                               10,
                                               label_width, label_height);
            records_labels[i * j]->setAlignment(Qt::AlignCenter);
        }
        records[i]->setStyleSheet("background-color: #C4C4C4;"
                                  "border-radius: 10px;"
                                  "border: 2px solid #000000");
        records[i]->setFixedSize(952, 50);
    }
}

void HomePageWidget::scrollAreaCreate()
{
    scrollArea = new QScrollArea(this);
    scrollWidget = new QWidget(scrollArea);
    scrollLayout = new QVBoxLayout(scrollWidget);

    scrollLayout->setSpacing(12);
    scrollLayout->setContentsMargins(72, 45, 136, 44);

    for (int i = 0; i < 40; ++i) {
        scrollLayout->addWidget(records[i]);
    }
    scrollWidget->setLayout(scrollLayout);
    scrollWidget->setStyleSheet("background-color: #4C3099");

    const int label_width = 198;
    const int label_height = 30;
    const int label_spacing = 32;
    for (int i = 0; i < 4; ++i) {
        scrollAreaLabels[i] = new QLabel(scrollLabelsText[i], scrollWidget);
        scrollAreaLabels[i]->setStyleSheet("background-color: none;"
                                           "font-size: 14px;"
                                           "color: #FFFFFF");
        scrollAreaLabels[i]->setFont(QFont("Lato"));
        scrollAreaLabels[i]->setGeometry(100 + i * (label_spacing + label_width),
                                         8,
                                         label_width, label_height);
        scrollAreaLabels[i]->setAlignment(Qt::AlignCenter);
    }

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setGeometry(20, 81, 1160, 511);
    scrollArea->setStyleSheet("border-radius: 10px;"
                              "border: 3px solid #000000");
}
