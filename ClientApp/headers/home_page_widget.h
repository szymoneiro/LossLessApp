#ifndef HOME_PAGE_WIDGET_H
#define HOME_PAGE_WIDGET_H

#include "scroll_widget.h"

class HomePageWidget : public ScrollWidget
{
    Q_OBJECT
private:
    QLabel *scrollAreaLabels[4];
    QStringList scrollLabelsText = {
        "Cryptocurrency name",
        "Quantity",
        "Price",
        "Current price"
    };

private slots:
    void onUserLogin();
    void onCryptocurrenciesTabClick();
    void onCurrenciesTabClick();
    void onStocksTabClick();

public:
    explicit HomePageWidget(QWidget *parent = nullptr);
    void scrollAreaCreate();
    void obtainUserRecords(recordType type);
};

#endif // HOME_PAGE_WIDGET_H
