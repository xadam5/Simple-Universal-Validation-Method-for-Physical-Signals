import pandas as pd
import matplotlib.pyplot as plt

files = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550]

for file in files:
    df = pd.read_csv("original/sensor_" + str(file) + 'cmdata.csv', index_col=0)
    df_pdoa = df['pdoa']
    df_tdoa = df['tdoa']
    df.to_csv("sensor_" + str(file) + "_pdoa.csv", columns=['pdoa'])
    plt.plot(df_pdoa)
    plt.savefig("plot_" + str(file) + "_pdoa.png")
    plt.close()
    df.to_csv("sensor_" + str(file) + "_tdoa.csv", columns=['tdoa'])
    plt.plot(df_tdoa)
    plt.savefig("plot_" + str(file) + "_tdoa.png")
    plt.close()