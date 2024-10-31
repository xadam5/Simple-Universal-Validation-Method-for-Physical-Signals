import pandas as pd
import matplotlib.pyplot as plt

for i in range(52):
    sensor_num = str(i)
    if (i < 10):
        sensor_num = "0" + sensor_num
    file_name = f'sensor_{sensor_num}/output_15.csv'
    df = pd.read_csv(file_name, index_col=0)

    plt.plot(df)
    plt.savefig(f'sensor_{sensor_num}/output_15.png')
    plt.close()
    