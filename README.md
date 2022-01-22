# LossLessApp

## Engineer diploma project - *Training application development for playing on the stock exchange by means of REST API and Qt*

# Table of Contents
- [Project setup](#project-setup)
- [Project requirements](#project-requirements)
- [Current status](#current-status)
- [About me](#about-me)

# Project setup

## API

1. Generate secret key - [secret](https://docs.python.org/3/library/secrets.html) library might come in handy - 32 bytes (256 bits) should be enought, the longer you generate, more brute-force attack proof it will get. 
Save it inside [API](API) directory as *secret_key.txt*
2. Make sure to install all [requirements](#project-requirements).
3. Run [LossLessAppAPI.py](API/LossLessAppAPI.py) to create database file and keep API running.
4. Register at: [CoinMarketCapAPI](https://pro.coinmarketcap.com/) to get your **free** cryptocurrencies API key.
5. Save it inside [API](API) directory as *'coinmarketapi_key.txt'*.
6. Run [fill_database.py](API/fill_database.py) script.
7. At that moment your database contains 100 cryptocurrencies, 24 currencies and 10 stocks.
8. Now let's get their actual values - register at [free currencyapi](https://freecurrencyapi.net/) and save API key as *freecurrencyapi_key.txt* inside [API](API) folder. Do the same thing for [twelvedata](https://twelvedata.com/) and save it as *twelvedata_key.txt*.
9. To keep refreshing all records values (by default every 10 minutes), run [API_value_update.py](API/API_value_update.py).\
\* To get full functionality between client and API uncomment lines 89-101 and delete lines 103-108 in [API file](API/LossLessAppAPI.py). If you want to keep refreshing values, make sure to add *x-access-token* to patch requests, obtained by logging into admin account.

## Qt Client
Download repository. Open project using [Qt Creator](https://www.qt.io/product/development-tools) or install [VS tools for Qt](https://doc.qt.io/qtvstools/qtvstools-getting-started.html).

## Where can I run this project?
Project has been created on Windows, but should compile fine on Linux and macOS.\
If you find any problems with running this project - [contact me](#about-me).

# Project requirements

## API:
Save below libraries as file, for example "requirements.txt"
```
aniso8601==9.0.1
certifi==2021.10.8
charset-normalizer==2.0.8
click==8.0.3
colorama==0.4.4
Flask==2.0.2
Flask-RESTful==0.3.9
Flask-SQLAlchemy==2.5.1
greenlet==1.1.2
idna==3.3
itsdangerous==2.0.1
Jinja2==3.0.3
MarkupSafe==2.0.1
PyJWT==2.3.0
pytz==2021.3
requests==2.26.0
schedule==1.1.0
six==1.16.0
SQLAlchemy==1.4.27
urllib3==1.26.7
Werkzeug==2.0.2
```
then run in console:
```
pip install -r requirements.txt
```
## Client:

Qt version **5.15**

# Current status
I've realized all functionalities needed for my diploma topic. Project is in beta version and there's still plenty of features to add and few bugs to fix.

# About me
Szymon Wojtach, IT Student, currently finishing engineering diploma\
*email:* **szymonwojtach99@gmail.com** \
*Telegram:* **https://t.me/swojtach** \
*LinkedIn:* **https://tinyurl.com/SWojtach** 
