# Built-in python modules
import csv
from pprint import pprint
from collections import OrderedDict

# External requirement (https://pandas.pydata.org/)
import pandas as pd
# Specify configuration file and data file
config = ('cache_config_rotation.csv', 'processed_data_cache_rotation.h5')

# Read the configuration file to get details on the data
with open(config[0], newline='') as f:
    reader = csv.reader(f, delimiter=';')
    files = list(reader)[1:]  # skip header

    log_files = OrderedDict()
    for entry in files:
        if not entry:
            continue
        # entry: (title, filename, description)
        log_files[entry[0]] = (entry[1], entry[2])

# Read data for each entry of the configuration file from the HDF Store
dataframes = {}
with pd.HDFStore(config[1], mode='r') as store:
    print('HDF Store keys:')
    pprint(store.keys())
    print()

    print('Reading data:')
    for title, (filename, description) in log_files.items():
        info_df = store.get('cache_'+title+'/info')
        df = store.get('cache_'+title+'/df')

        # Check if details in configuration and data file match
        if (title == info_df.title[0]
            and filename == info_df.file_name[0]):
            print(title, '=>', info_df.description[0])
        else:
            print(title, '=>', 'ERROR stored data does not match configuration')

        # Cache version is assumed to be the same for all dataframes in the file
        cache_version = info_df.version[0]

        # Print information of the data read
        df.info(memory_usage='deep')
        print()
        dataframes[title] = df

for key in dataframes.keys():
    dataframes[key].to_csv("sensor_" + key + "data.csv")