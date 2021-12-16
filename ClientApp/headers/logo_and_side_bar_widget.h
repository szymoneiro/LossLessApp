#ifndef LOGO_AND_SIDE_BAR_WIDGET_H
#define LOGO_AND_SIDE_BAR_WIDGET_H

#include <QtWidgets>

class LogoAndSideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoAndSideBarWidget(QWidget *parent = nullptr);

signals:
    void homePageClicked();
    void buyPageClicked();
    void sellPageClicked();

private:
    QDir iconsDir;
    QStringList iconsNames = {
        "home_icon.png",
        "stats_icon.png",
        "buy_icon.png",
        "sell_icon.png"
    };
    enum { button_size = 56, icon_size = 32 };

    QLabel *logoWidget;
    QVBoxLayout *iconWidgetLayout;

    QWidget *sideBarWidget;
    QVBoxLayout *sideBarWidgetLayout;

    /* HOME/STATS/BUY/SELL */
    QPushButton *sideBarButtons[4];

    /* Functions */
    void createLayouts();
    void createLogoBar();
    void createSideBar();
    void createButtons();
    void createShadows();

private slots:
    void onHomePageButtonClicked();
    void onBuyPageButtonClicked();
    void onSellPageButtonClicked();
};

#endif // LOGO_AND_SIDE_BAR_WIDGET_H
