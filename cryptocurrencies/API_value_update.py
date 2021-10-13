import os
from requests import Session
import requests
from requests.exceptions import Timeout, TooManyRedirects
import json
import schedule
import time


# Obtain script location
def get_current_location():
    return os.path.abspath(__file__)

if __name__ == "__main__":
    os.system('cls')

    # Try to obtain secret key
    key_path = os.path.abspath(os.path.join(get_current_location(), '../coinmarketapi_key.txt'))
    
    try:
        f = open(key_path, 'r')
    except FileNotFoundError:
        print("API key is missing!")
        exit(-1)
    
    url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
    headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': f.read()}
    f.close()

    # Obtain IDs from file
    IDs_file_path = os.path.abspath(os.path.join(get_current_location(), '../IDs.txt'))
    IDs_file = open(IDs_file_path, 'r')
    parameters = {
        "id": IDs_file.read() 
        }
    IDs_file.close()


    session = Session()
    session.headers.update(headers)
    API = "http://127.0.0.1:5000"
    endpoint = "/cryptocurrencies"

    def job():
        try:
            response = session.get(url, params=parameters)
            data = json.loads(response.text)
            if data["status"]["error_code"] !=0 :
                print("There is an error with received data!")
            else:
                for i in data["data"]:
                    response = requests.patch(API + endpoint + '/' + str(i),
                    {"value": data["data"][i]["quote"]["USD"]["price"]})
                    print(response.json())
        except (ConnectionError, Timeout, TooManyRedirects) as e:
            print(e)
    
    schedule.every(15).minutes.do(job)

    while(True):
        schedule.run_pending()
        time.sleep(1)
