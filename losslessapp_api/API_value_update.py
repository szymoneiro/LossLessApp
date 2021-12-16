import os
from requests import Session
import requests
from requests.exceptions import Timeout, TooManyRedirects
import json
import schedule
import time

API = "http://127.0.0.1:5000"
crypto_endpoint = "/cryptocurrencies"
currency_endpoint = "/currencies"
stock_endpoint = "/stocks"
FIRST_HALF = False

most_popular_currencies = ["EUR", "JPY", "GBP", "AUD", "CAD", "CHF", "CNY", "SEK",
                           "MXN", "NZD", "SGD", "HKD", "NOK", "KRW", "TRY", "INR",
                           "RUB", "BRL", "ZAR", "DKK", "PLN", "TWD", "THB", "MYR"]


# Obtain script location
def get_current_location():
    return os.path.abspath(__file__)


x_access_key_path = os.path.abspath(os.path.join(get_current_location(), '../API_update_key.txt'))
try:
    f = open(x_access_key_path, 'r')
    x_access_key = {"x-access-token": f.read()}
    f.close()
except FileNotFoundError:
    print("Cannot find x-access-token file!")
    exit(-1)

if __name__ == "__main__":
    # CoinMarket API
    coinmarket_key_path = os.path.abspath(os.path.join(get_current_location(),
                                                       '../../cryptocurrencies/coinmarketapi_key.txt'))
    try:
        f = open(coinmarket_key_path, 'r')
        crypto_url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
        crypto_headers = {
            'Accepts': 'application/json',
            'X-CMC_PRO_API_KEY': f.read()}
        f.close()
    except FileNotFoundError:
        print("CoinMarketAPI key is missing!")
        exit(-1)

    # FreeCurrencyAPI
    freecurrency_key_path = os.path.abspath(os.path.join(get_current_location(), '../freecurrencyapi_key.txt'))
    try:
        f = open(freecurrency_key_path, 'r')
        currency_url = "https://freecurrencyapi.net/api/v2/latest?apikey=" + f.read() + "&base_currency=USD"
        f.close()
    except FileNotFoundError:
        print("FreeCurrencyAPI key is missing!")
        exit(-1)

    # tveledataAPI
    api_path = os.path.abspath(os.path.join(os.path.abspath(__file__), '../stock_key.txt'))
    try:
        f = open(api_path, 'r')
    except FileNotFoundError:
        print("Stock API key is missing!")
        exit(-1)
    
    STOCK_KEY = f.read()
    f.close()
    first_half_parameter = "AAL,AAPL,AMD,AMZN,INTC"
    first_half_stocks = ["AAL", "AAPL", "AMD", "AMZN", "INTC"]

    second_half_parameter = "IRBT,MSFT,NVDA,SBUX,TSLA"
    second_half_stocks = ["IRBT", "MSFT", "NVDA", "SBUX", "TSLA"]

    # Obtain IDs from file
    IDs_file_path = os.path.abspath(os.path.join(get_current_location(), '../../cryptocurrencies/IDs.txt'))
    IDs_file = open(IDs_file_path, 'r')
    parameters = {
        "id": IDs_file.read()
    }
    IDs_file.close()

    crypto_session = Session()
    crypto_session.headers.update(crypto_headers)

    currency_session = Session()


    def crypto_update():
        try:
            response = crypto_session.get(crypto_url, params=parameters)
            data = json.loads(response.text)
            if data["status"]["error_code"] != 0:
                print("There is an error with received data!")
            else:
                for i in data["data"]:
                    response = requests.patch(API + crypto_endpoint + '/' + str(i),
                                              {"value": data["data"][i]["quote"]["USD"]["price"]}, headers=x_access_key)
                    print(response.json())
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)


    def currency_update():
        try:
            response = currency_session.get(currency_url)
            data = json.loads(response.text)
            i = 1
            for currency in most_popular_currencies:
                response = requests.patch(API + currency_endpoint + '/' + str(i),
                                          {"name": currency, "value": 1.0/data["data"][currency]}, headers=x_access_key)
                print(response.json())
                i += 1
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)

    def stock_update():
        global FIRST_HALF
        if FIRST_HALF:
            url = f"https://api.twelvedata.com/price?symbol={first_half_parameter}&dp=11&apikey={STOCK_KEY}"
        else:
            url = f"https://api.twelvedata.com/price?symbol={second_half_parameter}&dp=11&apikey={STOCK_KEY}"
        
        try:
            response = requests.get(url).json()
            dumped_response = json.dumps(response)
            data = json.loads(dumped_response)
            if FIRST_HALF:
                i = 1
                for stock in first_half_stocks:
                    print(stock)
                    response = requests.patch(API + stock_endpoint + "/" + str(i),
                    {"name": stock, "value": data[stock]["price"]}, headers=x_access_key)
                    print(response.json())
                    i += 1
            else:
                i = 6
                for stock in second_half_stocks:
                    response = requests.patch(API + stock_endpoint + "/" + str(i),
                    {"name": stock, "value": data[stock]["price"]}, headers=x_access_key)
                    print(response.json())
                    i += 1
            FIRST_HALF = not FIRST_HALF
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)



    schedule.every(1).minutes.do(crypto_update)
    schedule.every(1).minutes.do(currency_update)
    schedule.every(1).minutes.do(stock_update)

    while True:
        schedule.run_pending()
        time.sleep(1)
