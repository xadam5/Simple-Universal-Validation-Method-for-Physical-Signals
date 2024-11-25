import pandas as pd
import matplotlib.pyplot as plt
import sys

source_file = sys.argv[1]
# Load the CSV file, assuming the first column is the index
df = pd.read_csv(source_file + '.csv', index_col=0)

# Plot the data
plt.figure(figsize=(10, 6))

# plt.plot(df.index, df['Actual'], label='Actual')
plt.plot(df.index, df['Real Value'], label='Real Value')
plt.plot(df.index, df['Prediction'], label='Prediction')
plt.plot(df.index, df['Difference'], label='Difference')
plt.plot(df.index, df['Validation'], label='Validation')
plt.plot(df.index, df['Mean'], label='Mean')
plt.plot(df.index, df['Standard_deviation'], label='Standard_deviation')
# plt.plot(df.index, df['sd_range'], label='Standard deviation range')
# plt.plot(df.index, df['max_sd_from_me'], label='Max sd from mean')

plt.xlabel('Index')
plt.ylabel('Values')
plt.title('Real Value, Prediction, Difference, Validation, Mean, Standard_deviation')
# plt.title('Standard deviation range and Maximal number of SD from ME')
# plt.title('Actual, Prediction, and Difference')
# plt.title('Temperature, Validation')
# plt.title('Temperature, Validation, Mean, St_deviation')
plt.legend()
plt.grid(True)

# Show the plot
plt.savefig(source_file + '.png')
