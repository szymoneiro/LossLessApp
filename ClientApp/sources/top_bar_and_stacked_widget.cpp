#include "../headers/top_bar_and_stacked_widget.h"
#include <QNetworkReply>

TopBarAndStackedWidget::TopBarAndStackedWidget(QWidget *parent) : QWidget(parent)
{
    createLayouts();
    createStackedWidget();
    createButtons();

    /* Connect functions to buttons */
    connect(topBarButtons[2], SIGNAL(clicked()), SLOT(onExitButtonClicked()));
    connect(topBarButtons[0], SIGNAL(clicked()), SLOT(onClickGet()));

    /* Add widgets to layouts */
    rightWidgetLayout->addWidget(topBarWidget);
    rightWidgetLayout->addWidget(stackedWidget);

    for (int i = 0; i < 3; ++i)
        topBarLayout->addWidget(topBarButtons[i]);

    /* Initialize network manager */
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(managerFinished(QNetworkReply*)));
}

void TopBarAndStackedWidget::onExitButtonClicked()
{
    QApplication::quit();
}

void TopBarAndStackedWidget::onClickGet()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:5000/cryptocurrencies"));
    request.setRawHeader("x-access-token", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJwdWJsaWNfaWQiOiJiZTFjNDQ0My0zNGRlLTRiMTQtODc3Zi02NDBiMDFmNTU1YmMiLCJleHAiOjE2MzQ2NDU0MDZ9.KHU1AzzSQBokSoty9K7BP_8YdcMh-F38iPVCqQtEm_Y");
    manager->get(request);
}

void TopBarAndStackedWidget::managerFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    qDebug() << reply->readAll();
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
    topBarLayout->setContentsMargins(954, 12, 26, 12);
}

void TopBarAndStackedWidget::createStackedWidget()
{
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(1200, 640);
    stackedWidget->setStyleSheet("background-color: #FAFAFA");

    /* Initialize all widgets pinned to stacked widget and connect them to it. */
    signUpPage = new SignUpWidget(stackedWidget);
    stackedWidget->addWidget(signUpPage);
}

void TopBarAndStackedWidget::createButtons()
{
    iconsDir.setPath("../ClientApp/icons");
    for (int i = 0; i < 3; ++i) {
        topBarButtons[i] = new QPushButton(topBarWidget);
        topBarButtons[i]->setFixedSize(button_size, button_size);
        topBarButtons[i]->setStyleSheet("background-color:" + buttonColours[i] + ";"
                                          "border-radius: 15px");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        topBarButtons[i]->setIcon(QIcon(icon_path));
        topBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}
