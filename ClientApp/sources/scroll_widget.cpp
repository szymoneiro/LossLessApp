#include "../headers/scroll_widget.h"
#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"

ScrollWidget::ScrollWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 640);
    this->setStyleSheet("background-color: #FAFAFA");

    connectionManager = qobject_cast<TopBarAndStackedWidget*>(parent->parent())->getNetworkManager();
    accessToken = &qobject_cast<TopBarAndStackedWidget*>(this->parent()->parent())->x_access_token;

    createTabButtons();
    scrollAreaCreate();

    /* Create connection between sidebarWidget which emit signal to switch between screens. */
    MainWidget *parentWidget = qobject_cast<MainWidget*>(this->parent()->parent()->parent());
    QHBoxLayout *parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
    sideBarWidget = qobject_cast<LogoAndSideBarWidget*>(parentLayout->itemAt(0)->widget());

    /* Implement on child widgets:
     * connect(sidebarWidget, SIGNAL(_PageClicked()),
     *         this, SLOT(_ButtonClicked()));
     *   */

    /* Implement switching tabs on each child:
    connect(tabButtons[0], &QPushButton::clicked,
            this, &_Widget::onCryptocurrenciesTabClick);
    connect(tabButtons[1], &QPushButton::clicked,
            this, &_Widget::onCurrenciesTabClick);
    connect(tabButtons[2], &QPushButton::clicked,
            this, &_Widget::onStocksTabClick);
    */
}

void ScrollWidget::onCryptocurrenciesTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::cryptoRecord);
}

void ScrollWidget::onCurrenciesTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::currencyRecord);
}

void ScrollWidget::onStocksTabClick()
{
    clearScrollLayout();
    setActiveButton(recordType::stockRecord);
}

void ScrollWidget::scrollAreaCreate()
{
    scrollArea = new QScrollArea(this);
    scrollAreaWidget = new QWidget(scrollArea);
    scrollAreaWidgetLayout = new QVBoxLayout(scrollAreaWidget);

    scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
    scrollAreaWidget->setStyleSheet("background-color: #4C3099");

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    scrollArea->setStyleSheet("border-radius: 10px;"
                              "border: 3px solid #000000");
}

void ScrollWidget::createTabButtons()
{
    const int button_width = 150;
    const int button_height = 46;
    const int spacing = 16;

    QStringList buttonsColours = {
        "00FFA3;",
        "C8C8C8;",
        "C8C8C8;"
    };

    for (int i = 0; i < 3; ++i) {
        tabButtons[i] = new QPushButton(tabButtonsNames[i], this);
        tabButtons[i]->setStyleSheet("background-color: #" + buttonsColours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
        tabButtons[i]->setGeometry(46 + i * (button_width + spacing),
                                   48,
                                   button_width,
                                   button_height);
        tabButtons[i]->setFont(QFont("Lato"));
    }
}

void ScrollWidget::clearScrollLayout()
{
    QLayoutItem *child;
    QVBoxLayout *layoutToClear = this->scrollAreaWidgetLayout;
    while ((child = layoutToClear->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void ScrollWidget::setActiveButton(recordType type)
{
    QStringList colours;
    if (type == recordType::cryptoRecord) {
        colours = {
            "00FFA3;",
            "C8C8C8;",
            "C8C8C8;"
        };
    }
    else if (type == recordType::currencyRecord) {
        colours = {
            "C8C8C8;",
            "00FFA3;",
            "C8C8C8;"
        };
    }
    else {
        colours = {
            "C8C8C8;",
            "C8C8C8;",
            "00FFA3;"
        };
    }

    for (int i = 0; i < 3; ++i) {
        tabButtons[i]->setStyleSheet("background-color: #" + colours[i] +
                                     "border-radius: 10px;"
                                     "border: 3px solid #000000;"
                                     "font-weight: bold;"
                                     "font-size: 14px");
    }
}
