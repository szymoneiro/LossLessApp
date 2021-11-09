import os
from requests import Session
import requests
from requests.exceptions import Timeout, TooManyRedirects
import json
from requests.sessions import session
import schedule
import time

API = "http://127.0.0.1:5000"
crypto_endpoint = "/cryptocurrencies"
currency_endpoint = "/currencies"


most_popular_currencies = ["EUR", "JPY", "GBP", "AUD", "CAD", "RUB", "CHF", "HKD", "NZD", "PLN"]

# Obtain script location
def get_current_location():
    return os.path.abspath(__file__)

x_access_key_path = os.path.abspath(os.path.join(get_current_location(), '../API_update_key.txt'))
try:
    f = open(x_access_key_path, 'r')
except FileNotFoundError:
    print("Cannot find x-access-token file!")
    exit(-1)
x_access_key = {"x-access-token": f.read()}
f.close()

if __name__ == "__main__":
    # CoinMarket API
    coinmarket_key_path = os.path.abspath(os.path.join(get_current_location(), '../../cryptocurrencies/coinmarketapi_key.txt'))
    try:
        f = open(coinmarket_key_path, 'r')
    except FileNotFoundError:
        print("CoinMarketAPI key is missing!")
        exit(-1)
    
    crypto_url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
    crypto_headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': f.read()}
    f.close()

    # FreeCurrencyAPI
    freecurrency_key_path = os.path.abspath(os.path.join(get_current_location(), '../freecurrencyapi_key.txt'))
    try:
        f = open(freecurrency_key_path, 'r')
    except FileNotFoundError:
        print("FreeCurrencyAPI key is missing!")
        exit(-1)

    currency_url = "https://freecurrencyapi.net/api/v2/latest?apikey=" + f.read() + "base_currency=USD"
    f.close()

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
            if data["status"]["error_code"] !=0 :
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
                {"name": currency, "value": data["data"][currency]}, headers=x_access_key)
                print(response.json())
                i += 1
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)
    
    schedule.every(15).minutes.do(crypto_update)
    schedule.every(15).minutes.do(currency_update)

    while(True):
        schedule.run_pending()
        time.sleep(1)
