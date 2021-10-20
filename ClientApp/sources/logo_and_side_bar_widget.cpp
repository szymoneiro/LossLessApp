#include "../headers/logo_and_side_bar_widget.h"

LogoAndSideBarWidget::LogoAndSideBarWidget(QWidget *parent) : QWidget(parent)
{
    createSideBar();
    createLayouts();
    createLogoBar();
    createButtons();

    leftWidgetLayout->addWidget(logoBarLayout);
    leftWidgetLayout->addWidget(sideBarWidget);

    sideBarWidget->setLayout(sideBarLayout);
    for (int i = 0; i < 4; ++i)
        sideBarLayout->addWidget(sideBarButtons[i]);
}

void LogoAndSideBarWidget::createLayouts()
{
    leftWidgetLayout = new QVBoxLayout(this);
    leftWidgetLayout->setSpacing(0);
    leftWidgetLayout->setContentsMargins(0, 0, 0, 0);

    sideBarLayout = new QVBoxLayout(sideBarWidget);
    sideBarLayout->setSpacing(8);
    sideBarLayout->setContentsMargins(12, 16, 12, 376);
}

void LogoAndSideBarWidget::createLogoBar()
{
    logoBarLayout = new QLabel(this);
    logoBarLayout->setText("LOGO");
    logoBarLayout->setStyleSheet("background-color: #00FFA3;"
                            "color: #FFFFFF;"
                            "font: bold;");
    logoBarLayout->setAlignment(Qt::AlignCenter);
    logoBarLayout->setFixedSize(80, 80);
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
                                  "border-radius: 15px");
        QString icon_path = iconsDir.absolutePath() + "/" + iconsNames[i];
        sideBarButtons[i]->setIcon(QIcon(icon_path));
        sideBarButtons[i]->setIconSize(QSize(icon_size, icon_size));
    }
}
