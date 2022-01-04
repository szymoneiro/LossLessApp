QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    sources/buy_widget.cpp \
    sources/home_page_widget.cpp \
    sources/logo_and_side_bar_widget.cpp \
    sources/main_widget.cpp \
    sources/scroll_widget.cpp \
    sources/sell_widget.cpp \
    sources/sign_in_widget.cpp \
    sources/sign_up_widget.cpp \
    sources/top_bar_and_stacked_widget.cpp

HEADERS += \
    headers/buy_widget.h \
    headers/home_page_widget.h \
    headers/logo_and_side_bar_widget.h \
    headers/main_widget.h \
    headers/scroll_widget.h \
    headers/sell_widget.h \
    headers/sign_in_widget.h \
    headers/sign_up_widget.h \
    headers/top_bar_and_stacked_widget.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/basket_icon.png \
    icons/buy_icon.png \
    icons/exit_icon.png \
    icons/hide_icon.png \
    icons/home_icon.png \
    icons/profile_icon.png \
    icons/sell_icon.png \
    icons/settings_icon.png \
    icons/shop_icon.png \
    icons/show_icon.png \
    icons/stats_icon.png
