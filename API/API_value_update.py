import os
from requests import Session
import requests
from requests.exceptions import Timeout, TooManyRedirects
import json
import schedule
import time

def get_file_path(filename):
    current_script_path = os.path.abspath(__file__)
    return os.path.abspath(os.path.join(current_script_path, "..")) + "\\" + filename

API = "http://127.0.0.1:5000"
crypto_endpoint = "/cryptocurrencies"
currency_endpoint = "/currencies"
stock_endpoint = "/stocks"

most_popular_currencies = ["EUR", "JPY", "GBP", "AUD", "CAD", "CHF", "CNY", "SEK",
                           "MXN", "NZD", "SGD", "HKD", "NOK", "KRW", "TRY", "INR",
                           "RUB", "BRL", "ZAR", "DKK", "PLN", "TWD", "THB", "MYR"]

popular_companies = ["AAL", "AAPL", "AMD", "AMZN", "INTC", "IRBT", "MSFT", "NVDA", "SBUX", "TSLA"]

if __name__ == "__main__":
    # CoinMarket API
    coinmarket_key = get_file_path("coinmarketapi_key.txt")
    try:
        f = open(coinmarket_key, 'r')
        crypto_url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
        crypto_headers = {
            'Accepts': 'application/json',
            'X-CMC_PRO_API_KEY': f.read()}
        f.close()
    except FileNotFoundError:
        print("CoinMarketAPI key is missing!")
        exit(-1)

    # FreeCurrencyAPI
    freecurrency_key = get_file_path("freecurrencyapi_key.txt")
    try:
        f = open(freecurrency_key, 'r')
        currency_url = "https://freecurrencyapi.net/api/v2/latest?apikey=" + f.read() + "&base_currency=USD"
        f.close()
    except FileNotFoundError:
        print("FreeCurrencyAPI key is missing!")
        exit(-1)

    # twelvedataAPI
    twelvedata_key = get_file_path("twelvedata_key.txt")
    try:
        f = open(twelvedata_key, 'r')
    except FileNotFoundError:
        print("TwelveData API key is missing!")
        exit(-1)
    
    STOCK_KEY = f.read()
    f.close()

    crypto_params = {}
    IDs_file = get_file_path("IDS.txt")
    with open(IDs_file) as file:
        crypto_params = {
            "id": file.read()
        }
    file.close()

    crypto_session = Session()
    crypto_session.headers.update(crypto_headers)

    currency_session = Session()


    def crypto_update():
        try:
            response = crypto_session.get(crypto_url, params=crypto_params)
            data = json.loads(response.text)
            if data["status"]["error_code"] != 0:
                print("There is an error with received data!")
            else:
                for i in data["data"]:
                    response = requests.patch(API + crypto_endpoint + '/' + str(i),
                                              {"value": data["data"][i]["quote"]["USD"]["price"]})
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
                                          {"name": currency, "value": 1.0 / data["data"][currency]})
                print(response.json())
                i += 1
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)

    FIRST_HALF = True
    # Free limit let us update only 5 values at once, so we need to switch between half of them every call.
    def stock_update():
        global FIRST_HALF
        stock_params = ""
        for i in range(5):
            if FIRST_HALF:
                stock_params += popular_companies[i] + ","
            else:
                stock_params += popular_companies[i + 5] + ","
        # Remove comma at the end
        stock_params = stock_params[:-1]
        stock_url = f"https://api.twelvedata.com/price?symbol={stock_params}&dp=11&apikey={STOCK_KEY}"
        try:
            response = requests.get(stock_url).json()
            dumped_response = json.dumps(response)
            data = json.loads(dumped_response)
            if FIRST_HALF:
                i = 1
                companies = popular_companies[:5]
            else:
                i = 6
                companies = popular_companies[5:]
            for stock in companies:
                response = requests.patch(API + stock_endpoint + "/" + str(i),
                {"name": stock, "value": data[stock]["price"]})
                print(response.json())
                i += 1
            FIRST_HALF = not FIRST_HALF
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)



    schedule.every(10).minutes.do(crypto_update)
    schedule.every(10).minutes.do(currency_update)
    schedule.every(10).minutes.do(stock_update)

    while True:
        schedule.run_pending()
        time.sleep(1)
