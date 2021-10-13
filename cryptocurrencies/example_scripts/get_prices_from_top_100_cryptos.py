import os
import json
from requests import Session
from requests.exceptions import ConnectionError, Timeout, TooManyRedirects


# Obtain script location
def get_current_location():
    return os.path.abspath(__file__)

# Function to move backwards, similar to cd ..
def move_backwards(path):
    return os.path.abspath(os.path.join(path, '../..'))

if __name__ == "__main__":

    # Obtain secret key
    current_location = get_current_location()
    key_path = move_backwards(
        current_location) + '/coinmarketapi_key.txt'

    try:
        f = open(key_path, 'r')
    except FileNotFoundError:
        print("API Key is missing!")
        exit(-1)
    headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': f.read()}

    f.close()

    # Go to the file containing top 100 IDs and format is properly
    id_path = move_backwards(get_current_location()) + '\example_files' + '\\top_100_cryptocurrencies_IDs.txt'
    try:
        id_file = open(id_path, 'r')
    except FileNotFoundError:
        print("Not found IDs file!")
        exit(-1)
    raw_text = id_file.read()
    raw_data = raw_text.replace("\'", "\"")
    raw_data = raw_data.replace("None", "\"None\"")
    data = json.loads(raw_data)
    id_file.close()

    url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest'
    parameters = {
        "id": ""
    }

    # Obtain IDs from our data
    for record in data["data"]:
        parameters["id"] += str(record["id"]) + ","

    # Remove comma at the end
    parameters["id"] = parameters["id"].rstrip(parameters["id"][-1])

    session = Session()
    session.headers.update(headers)
    
    try:
        response = session.get(url, params=parameters)
        data = json.loads(response.text)
        # Set path where we want to save cryptos value file.
        cryptos_values_path = move_backwards(current_location) + '\example_files' + '\\top_100_values.txt'
        with open(cryptos_values_path, 'w') as text:
            text.write(str(data))
        text.close()
    except (ConnectionError, Timeout, TooManyRedirects) as e:
        print(e)
        exit(-1)