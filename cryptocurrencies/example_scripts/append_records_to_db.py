import os
import json
import requests


API = "http://127.0.0.1:5000"
endpoint = "/cryptocurrencies"

if __name__ == "__main__":

    current_dir = os.path.abspath(__file__)
    cryptos_file = os.path.abspath(os.path.join(current_dir, '../../example_files/top_100_cryptocurrencies_IDs.txt'))

    try:
        f = open(cryptos_file, 'r')
    except FileNotFoundError:
        print("File not found!")
        exit(-1)

    raw_text = f.read()
    raw_data = raw_text.replace("\'", "\"")
    raw_data = raw_data.replace("None", "\"None\"")
    data = json.loads(raw_data)
    f.close()

    records = {}
    for record in data["data"]:
        records[record["id"]] = {   "name": record["name"],
                                    "symbol": record["symbol"],
                                    "value": 0}
    
    for i in records:
        response = requests.put(API + endpoint + "/" + str(i),
                                {"name": records[i]["name"], "value": records[i]["value"]})
        print(response.json())   
