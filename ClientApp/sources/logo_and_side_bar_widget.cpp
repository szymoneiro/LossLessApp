#include "../headers/logo_and_side_bar_widget.h"

LogoAndSideBarWidget::LogoAndSideBarWidget(QWidget *parent) : QWidget(parent)
{
    createSideBar();
    createLayouts();
    createLogoBar();
    createButtons();
    createShadows();

    iconWidgetLayout->addWidget(logoWidget);
    iconWidgetLayout->addWidget(sideBarWidget);

    sideBarWidget->setLayout(sideBarWidgetLayout);
    for (int i = 0; i < 4; ++i)
        sideBarWidgetLayout->addWidget(sideBarButtons[i]);

    connect(sideBarButtons[0], &QPushButton::clicked,
            this, &LogoAndSideBarWidget::onHomePageButtonClicked);

    connect(sideBarButtons[2], &QPushButton::clicked,
            this, &LogoAndSideBarWidget::onBuyPageButtonClicked);

    connect(sideBarButtons[3], &QPushButton::clicked,
            this, &LogoAndSideBarWidget::onSellPageButtonClicked);
}

void LogoAndSideBarWidget::createLayouts()
{
    iconWidgetLayout = new QVBoxLayout(this);
    iconWidgetLayout->setSpacing(0);
    iconWidgetLayout->setContentsMargins(0, 0, 0, 0);

    sideBarWidgetLayout = new QVBoxLayout(sideBarWidget);
    sideBarWidgetLayout->setSpacing(8);
    sideBarWidgetLayout->setContentsMargins(12, 16, 12, 376);
}

void LogoAndSideBarWidget::createLogoBar()
{
    logoWidget = new QLabel(this);
    logoWidget->setText("LOGO");
    logoWidget->setStyleSheet("background-color: #00FFA3;"
                            "color: #FFFFFF;"
                            "font: bold;");
    logoWidget->setAlignment(Qt::AlignCenter);
    logoWidget->setFixedSize(80, 80);
}

void LogoAndSideBarWidget::createSideBar()
{
    sideBarWidget = new QWidget(this);
    sideBarWidget->setStyleSheet("background-color: #4C3099");
}

void LogoAndSideBarWidget::createButtons()
{
    /* Set path to button icons */
    iconsDir.setPath("../ClientApp/icons");

    for (int i = 0; i < 4; ++i) {
        sideBarButtons[i] = new QPushButton(sideBarWidget);
        sideBarButtons[i]->setFixedSize(button_size, button_size);
        sideBarButtons[i]->setStyleSheet("background-color: white;"
                                         "border-radius: 15px;"
                                         "border: 2px solid #000000");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        sideBarButtons[i]->setIcon(QIcon(icon_path));
        sideBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}

void LogoAndSideBarWidget::createShadows()
{
    QGraphicsDropShadowEffect *buttonShadows[4];
    for (int i = 0; i < 4; ++i) {
        buttonShadows[i] = new QGraphicsDropShadowEffect(this);
        buttonShadows[i]->setBlurRadius(5);
        /* 0.25 * 255 ~ 64 */
        buttonShadows[i]->setColor(QColor(0, 0, 0, 64));
        buttonShadows[i]->setOffset(2, 2);
        sideBarButtons[i]->setGraphicsEffect(buttonShadows[i]);
    }
}

void LogoAndSideBarWidget::onHomePageButtonClicked()
{
    emit homePageClicked();
}

void LogoAndSideBarWidget::onBuyPageButtonClicked()
{
    emit buyPageClicked();
}

void LogoAndSideBarWidget::onSellPageButtonClicked()
{
    emit sellPageClicked();
}
