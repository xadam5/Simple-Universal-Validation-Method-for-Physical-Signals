import pandas as pd
import matplotlib.pyplot as plt
import sys
import numpy as np

source_file = "simulations/" + sys.argv[1]
# Load the CSV file, assuming the first column is the index
df = pd.read_csv(source_file + '.csv', index_col=0)

# Plot the data
plt.figure(figsize=(12, 6))

plt.plot(df.index, df['value'], label='Real Value')
plt.plot(df.index, df['forecast'], label='Forecast')

# Define colors for specific flags and a default color
flag_colors = {
    "VALID": "white",
    "INITIALIZING": "blue",
}
default_color = "red"
plotted_red = False

# Add ribbons for flags
flags = df['flags'].str.split(',').apply(set)
all_flags = set.union(*flags)  # Collect all unique flags
for flag in all_flags:
    # Get the color for the flag or use the default
    color = flag_colors.get(flag, default_color)
    
    # Create a mask for indices where the flag is active
    ribbon_mask = [flag in f for f in flags]
    label = f"Flag {flag}" if color in flag_colors.values() else ("INVALID" if not plotted_red else None)

    # Use fill_between to create a ribbon
    plt.fill_between(
        df.index, 
        df['value'].min() - 0.2,  # Bottom of the ribbon
        df['value'].max() + 0.2,  # Top of the ribbon
        where=ribbon_mask,
        color=color,
        alpha=0.2,
        label = label
    )

    plotted_red = True if color == "red" else False

plt.xlabel('Index')
plt.ylabel('Values')
plt.title('Real Value, Forecast, Failure count')
plt.legend(loc=3)
plt.grid(True)

# Show the plot
plt.savefig(source_file + '.png')
