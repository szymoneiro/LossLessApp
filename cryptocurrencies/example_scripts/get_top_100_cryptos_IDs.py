import os
from requests import Session
import json
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

    url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/map'

    # Get 100 top ranked cryptocurrencies IDs
    parameters = {
        'start': '1',
        'limit': '100',
        'sort': 'cmc_rank'}

    headers = {
        'Accepts': 'application/json',
        'X-CMC_PRO_API_KEY': f.read()}

    f.close()

    session = Session()
    session.headers.update(headers)

    try:
        response = session.get(url, params=parameters)
        data = json.loads(response.text)

        # Set path where we want to save our top 100 cryptocurrencies file.
        top_100_file_path = move_backwards(
            current_location) + '\example_files' + '\\top_100_cryptocurrencies_IDs.txt'
        with open(top_100_file_path, 'w') as text:
            text.write(str(data))
        text.close()

    except (ConnectionError, Timeout, TooManyRedirects) as e:
        print(e)
        exit(-1)
