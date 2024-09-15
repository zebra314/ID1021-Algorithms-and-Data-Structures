import numpy as np
import matplotlib.pyplot as plt

# Configure Matplotlib to use PGF
plt.rcParams.update({
  "pgf.texsystem": "pdflatex",
  "font.family": "serif",
  "text.usetex": True,
  "pgf.rcfonts": False,
})

data = np.loadtxt('./data/binary_search.dat')

array_size = data[:, 0]
log_array_size = np.log2(array_size)
time_ms = data[:, 1]  

a, b = np.polyfit(log_array_size, time_ms, 1)
print(f"Linear regression equation: Time = {a} * log2(ArraySize) + {b}")

# Generate regression line values for the original array sizes
predicted_time = a * log_array_size + b

# Generate the full range for regression line including prediction point
extended_log_array_size = np.linspace(np.min(log_array_size), np.log2(64000000), 100)
extended_array_size = 2**extended_log_array_size
extended_predicted_time = a * extended_log_array_size + b

# Plot original data points
plt.scatter(array_size, time_ms, label='Data', color='blue')

# Plot regression line with extended range
plt.plot(extended_array_size, extended_predicted_time, label='Regression Line', color='red')

# Predict the time for a larger array size, e.g., 10 million
predicted_size = 64000000
predicted_log_size = np.log2(predicted_size)
predicted_time_for_large_size = a * predicted_log_size + b

# Plot the predicted point for array size = 10 million
plt.scatter(predicted_size, predicted_time_for_large_size, color='green', zorder=5)
plt.text(predicted_size, predicted_time_for_large_size, f'  ({predicted_size}, {predicted_time_for_large_size:.2f})', 
         verticalalignment='bottom', color='green')

# Set the plot to logarithmic scale for x-axis
plt.xscale('log', base=2)  # Use log base 2 for better visualization of log2 relationship
plt.xlabel('Array Size (log scale)')
plt.ylabel('Time (ms)')
plt.legend()

plt.tight_layout()
# plt.savefig('./data/regression.pgf')

# Show the plot
# plt.show()