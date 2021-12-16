#include "../headers/top_bar_and_stacked_widget.h"
#include "../headers/main_widget.h"
#include <QNetworkReply>

Q_GLOBAL_STATIC(QNetworkAccessManager, appNetworkManager)

TopBarAndStackedWidget::TopBarAndStackedWidget(QWidget *parent) : QWidget(parent)
{
    createLayouts();
    createBalanceLabel();
    createStackedWidget();
    createButtons();

    /* Connect functions to buttons */
    connect(topBarButtons[1], &QPushButton::clicked,
            this, &TopBarAndStackedWidget::setSignInPage);
    connect(topBarButtons[2], &QPushButton::clicked,
            this, &TopBarAndStackedWidget::onExitButtonClicked);

    /* Add widgets to layouts */
    rightWidgetLayout->addWidget(topBarWidget);
    rightWidgetLayout->addWidget(stackedWidget);

    for (int i = 0; i < 3; ++i)
        topBarLayout->addWidget(topBarButtons[i]);

    connect(signUpPage, SIGNAL(signInClicked()), SLOT(setSignInPage()));
    connect(signInPage, SIGNAL(signUpClicked()), SLOT(setSignUpPage()));

    connect(signInPage, SIGNAL(userLoggedIn()), SLOT(setHomePage()));
    connect(signInPage, SIGNAL(userLoggedIn()),
            homePage, SLOT(onUserLogin()));

    /* Obtain sidebar widget to connect its signals to change widgets in StackedWidget */
    MainWidget *parentWidget = qobject_cast<MainWidget*>(this->parent());
    QHBoxLayout *parentLayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
    sideBarWidget = qobject_cast<LogoAndSideBarWidget*>(parentLayout->itemAt(0)->widget());

    connect(sideBarWidget, SIGNAL(homePageClicked()),
            this, SLOT(setHomePage()));
    connect(sideBarWidget, SIGNAL(buyPageClicked()),
            this, SLOT(setBuyPage()));
    connect(sideBarWidget, SIGNAL(sellPageClicked()),
            this, SLOT(setSellPage()));

    stackedWidget->setCurrentIndex(0);
    createShadows();
}

QNetworkAccessManager *TopBarAndStackedWidget::getNetworkManager()
{
    return appNetworkManager;
}

void TopBarAndStackedWidget::onExitButtonClicked()
{
    QApplication::quit();
}

void TopBarAndStackedWidget::setSignUpPage()
{
    stackedWidget->setCurrentWidget(signUpPage);
}

void TopBarAndStackedWidget::setSignInPage()
{
    stackedWidget->setCurrentWidget(signInPage);
}

void TopBarAndStackedWidget::setHomePage()
{
    stackedWidget->setCurrentWidget(homePage);
}

void TopBarAndStackedWidget::setBuyPage()
{
    stackedWidget->setCurrentWidget(buyPage);
}

void TopBarAndStackedWidget::setSellPage()
{
    stackedWidget->setCurrentWidget(sellPage);
}

void TopBarAndStackedWidget::createLayouts()
{
    rightWidgetLayout = new QVBoxLayout(this);
    rightWidgetLayout->setSpacing(0);
    rightWidgetLayout->setContentsMargins(0, 0, 0, 0);

    /* Create top bar widget to make it topBarLayouts' parent */
    topBarWidget = new QWidget(this);
    topBarWidget->setStyleSheet("background-color: #FFFFFF");

    topBarLayout = new QHBoxLayout(topBarWidget);
    topBarLayout->setSpacing(26);
    topBarLayout->setContentsMargins(712, 12, 26, 12);
}

void TopBarAndStackedWidget::createStackedWidget()
{
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(1200, 640);
    stackedWidget->setStyleSheet("background-color: #FAFAFA");

    /* Initialize all widgets pinned to stacked widget and connect them to it. */
    signInPage = new SignInWidget(stackedWidget);
    signUpPage = new SignUpWidget(stackedWidget);
    homePage = new HomePageWidget(stackedWidget);
    buyPage = new BuyWidget(stackedWidget);
    sellPage = new SellWidget(stackedWidget);

    stackedWidget->addWidget(signInPage);
    stackedWidget->addWidget(signUpPage);
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(buyPage);
    stackedWidget->addWidget(sellPage);
}

void TopBarAndStackedWidget::createButtons()
{
    iconsDir.setPath("../ClientApp/icons");
    for (int i = 0; i < 3; ++i) {
        topBarButtons[i] = new QPushButton(topBarWidget);
        topBarButtons[i]->setFixedSize(button_size, button_size);
        topBarButtons[i]->setStyleSheet("background-color:" + buttonColours[i] + ";"
                                        "border-radius: 15px;"
                                        "border: 2px solid #000000");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        topBarButtons[i]->setIcon(QIcon(icon_path));
        topBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}

void TopBarAndStackedWidget::createBalanceLabel()
{
    accountBalance = new QLabel("0 $", this);
    accountBalance->setFixedSize(216, 56);
    accountBalance->setStyleSheet("border-radius: 10px;"
                                  "border: 2px solid #000000;"
                                  "background-color: #C4C4C4;"
                                  "font-weight: bold;"
                                  "font-size: 16px");
    accountBalance->setAlignment(Qt::AlignCenter);
    accountBalance->setFont(QFont("Lato"));

    topBarLayout->addWidget(accountBalance);
}

void TopBarAndStackedWidget::createShadows()
{
    QGraphicsDropShadowEffect *labelShadow = new QGraphicsDropShadowEffect(this);
    labelShadow->setBlurRadius(5);
    /* 0.25 * 255 ~ 64 */
    labelShadow->setColor(QColor(0, 0, 0, 64));
    labelShadow->setOffset(2, 2);
    accountBalance->setGraphicsEffect(labelShadow);

    QGraphicsDropShadowEffect *buttonShadows[3];

    for (int i = 0; i < 3; ++i) {
        buttonShadows[i] = new QGraphicsDropShadowEffect(this);
        buttonShadows[i]->setBlurRadius(5);
        /* 0.25 * 255 ~ 64 */
        buttonShadows[i]->setColor(QColor(0, 0, 0, 64));
        buttonShadows[i]->setOffset(2, 2);
        topBarButtons[i]->setGraphicsEffect(buttonShadows[i]);
    }
}
