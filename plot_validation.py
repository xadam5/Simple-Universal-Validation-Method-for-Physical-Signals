import pandas as pd
import matplotlib.pyplot as plt
import sys

source_file = sys.argv[1]
# Load the CSV file, assuming the first column is the index
df = pd.read_csv(source_file + '.csv', index_col=0)

# Plot the data
plt.figure(figsize=(12, 6))

plt.plot(df.index, df['value'], label='Real Value')
plt.plot(df.index, df['forecast'], label='Forecast')

# Define a function to determine the status based on flags
def determine_status(flags):
    if 'LOST' in flags:
        return 'LOST'
    elif 'INVALID' in flags:
        return 'INVALID'
    elif flags == 'INITIALIZING':
        return 'INITIALIZING'
    else:
        return 'VALID'

# Create a new column 'status' based on the flags column
df['status'] = df['flags'].apply(determine_status)

# Define colors for specific flags and a default color
flag_colors = {
    "VALID": "green",
    "INITIALIZING": "blue",
    "INVALID": "red",
    "LOST": "orange",
}

# Add ribbons for flags
all_flags = {'VALID', 'INITIALIZING', 'INVALID', 'LOST'}
for flag in all_flags:
    color = flag_colors[flag]
    
    # Create a mask for indices where the flag is active
    ribbon_mask = [flag in f for f in df['status']]
    label = f"Flag {flag}"

    if (flag == 'VALID' or flag == 'INITIALIZING'):
        a = 0.15
    else:
        a = 0.5

    # Use fill_between to create a ribbon
    plt.fill_between(
        df.index, 
        df['value'].min() - 0.2,  # Bottom of the ribbon
        df['value'].max() + 0.2,  # Top of the ribbon
        where=ribbon_mask,
        color=color,
        alpha=a,
        label = label
    )

    plotted_red = True if color == "red" else False

plt.xlabel('Timestamp')
plt.ylabel('Values')
plt.title('Real Value, Forecast, Validation')
plt.legend(loc='lower left')
plt.grid(True)

# Show the plot
plt.savefig(source_file + '.png')
