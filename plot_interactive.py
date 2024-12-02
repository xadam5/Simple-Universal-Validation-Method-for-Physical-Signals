import pandas as pd
import plotly.graph_objects as go
import sys

# Load the dataset
file_path = sys.argv[1]  # Replace with your CSV file path
data = pd.read_csv(file_path  + ".csv")

# Parse flags column to extract individual flags
data["Flags"] = data["flags"].str.split('|')

# Add a column indicating color of forecast line
data["Color"] = data["Flags"].apply(lambda x: "black" if "LOST" in x else ("red" if "INVALID" in x else "green"))

# Create the base figure
fig = go.Figure()

# Add line for 'value'
fig.add_trace(go.Scatter(
    x=data["timestamp"],
    y=data["value"],
    mode="lines+markers",
    name="Value",
    line=dict(color="orange"),
    hovertemplate="Timestamp: %{x}<br>Value: %{y}<br>",
    # customdata=list(zip(data["flags"], data["failure_count"]))  # Additional hover data
))

# Add line for 'forecast'
fig.add_trace(go.Scatter(
    x=data["timestamp"],
    y=data["forecast"],
    mode="lines+markers",
    name="Forecast",
    line=dict(color="gray"),  # Base line color
    marker=dict(color=data["Color"]),  # Marker color based on "Lost" or "Invalid"
    hovertemplate="Timestamp: %{x}<br>Forecast: %{y}<br>Flags: %{customdata[0]}<br>Failure Count: %{customdata[1]}",
    customdata=list(zip(data["flags"], data["failure_count"]))  # Additional hover data
    ))

# Layout customization
fig.update_layout(
    title="Interactive Plot of Value and Forecast",
    xaxis_title="Timestamp",
    yaxis_title="Numerical Value",
    template="plotly_white",
    legend=dict(orientation="h", x=0.5, xanchor="center", y=-0.2),
    hovermode="x unified"  # Show hover info for all lines at the same timestamp
)

# Save as interactive HTML file
fig.write_html(file_path + ".html")

