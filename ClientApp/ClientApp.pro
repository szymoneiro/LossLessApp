QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    sources/logo_and_side_bar_widget.cpp \
    sources/main_body_stacked_widget.cpp \
    sources/main_widget.cpp \
    sources/top_bar_and_stacked_widget.cpp

HEADERS += \
    headers/logo_and_side_bar_widget.h \
    headers/main_body_stacked_widget.h \
    headers/main_widget.h \
    headers/top_bar_and_stacked_widget.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/buy_icon.png \
    icons/exit_icon.png \
    icons/home_icon.png \
    icons/profile_icon.png \
    icons/sell_icon.png \
    icons/settings_icon.png \
    icons/stats_icon.png
