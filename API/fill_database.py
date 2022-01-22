import os
import requests
from requests import Session
import json
from requests.exceptions import ConnectionError, Timeout, TooManyRedirects

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

    crypto_path = get_file_path("coinmarketapi_key.txt")
    try:
        f = open(crypto_path, 'r')
    except FileNotFoundError:
        print("Crypto API key is missing!")
        exit(-1)
    
    crypto_url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/map'
    crypto_params = {
        'start': '1',
        'limit': '100',
        'sort': 'cmc_rank'}
    crypto_headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': f.read()}
    f.close()

    session = Session()
    session.headers.update(crypto_headers)

    try:
        response = session.get(crypto_url, params=crypto_params)
        raw_data = json.loads(response.text)
        crypto_data = raw_data['data']
    except (ConnectionError, Timeout, TooManyRedirects) as e:
        print(e)
        exit(-1)
    
    crypto_ids_and_names = {}
    id_file_path = get_file_path("IDS.txt")
    with open(id_file_path, 'w') as file:
        for crypto in crypto_data:
            crypto_ids_and_names[crypto['id']] = crypto['name']
            file.write(str(crypto['id']) + ',')
    # And then remove last character of file (',')
        file.seek(file.tell() - 1, os.SEEK_SET)
        file.truncate()
    file.close()

    for id in crypto_ids_and_names:
        response = requests.post(API + crypto_endpoint + "/" + str(id),
                                {"name" : crypto_ids_and_names[id], "value" : 0})
        print(response.json())

    for id, currency in enumerate(most_popular_currencies):
        response = requests.post(API + currency_endpoint + "/" + str(id + 1),
                                {"name" : currency, "value" : 0})
        print(response.json())
    
    for id, stock in enumerate(popular_companies):
        response = requests.post(API + stock_endpoint + "/" + str(id + 1),
                                {"name" : stock, "value" : 0})
        print(response.json())
    

    
