#ifndef HOME_PAGE_WIDGET_H
#define HOME_PAGE_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>

class HomePageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomePageWidget(QWidget *parent = nullptr);

private:
    QPushButton *tabButtons[3];
    QStringList buttonsNames = {
        "Cryptocurrencies",
        "Currencies",
        "Stocks"
    };

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;

    QLabel *scrollAreaLabels[4];
    QStringList scrollLabelsText = {
        "Cryptocurrency name",
        "Quantity",
        "Buy price",
        "Buy date"
    };

    QWidget *records[40];
    QLabel *records_labels[160];
    QStringList mock_data = {
        "Basic Attention Token",
        "1.25",
        "1.03343814063204",
        "20.11.2021 01:03:53"
    };

    QNetworkAccessManager *connectionManager;
    QNetworkReply *reply;

    void buttonsCreate();
    void createRecords();
    void scrollAreaCreate();
};

#endif // HOME_PAGE_WIDGET_H
