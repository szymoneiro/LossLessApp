import os
import json

if __name__ == "__main__":
    
    current_dir = os.path.abspath(__file__)
    top_100_cryptos = os.path.abspath(os.path.join(current_dir, '../../example_files/top_100_cryptocurrencies_IDs.txt'))
    f = open(top_100_cryptos, 'r')
    raw_text = f.read()
    raw_data = raw_text.replace("\'", "\"")
    raw_data = raw_data.replace("None", "\"None\"")
    records_data = json.loads(raw_data)
    f.close()


    IDs = []
    for id in records_data['data']:
        IDs.append(id["id"])
    IDs.sort()

    ID_file = open(os.path.abspath(os.path.join(current_dir, '../../IDs.txt' )), 'w')
    for i in range(len(IDs) - 1):
        ID_file.write(str(IDs[i]) + ',')
    ID_file.write(str(IDs[-1]))
    ID_file.close()