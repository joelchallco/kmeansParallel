import csv
import matplotlib.pyplot as plt
import random

# Read data points and cluster assignments from a CSV file
def read_data_from_csv(file_path):
    data = []
    assignments = []
    with open(file_path, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        next(csv_reader, None)
        for row in csv_reader:
            x = float(row[0])
            y = float(row[1])
            cluster = int(row[2])
            data.append([x, y])
            assignments.append(cluster)
    return data, assignments

# Plot data points with coloring based on cluster assignments
def plot_data_points(data, assignments, data1):
    colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']  # Define color options

    plt.figure(figsize=(8, 6))

    for i, point in enumerate(data):
        x, y = point[0], point[1]
        color = colors[assignments[i] % len(colors)]
        plt.scatter(x, y, color=color)

    for i, point in enumerate(data1):
        x, y = point[0], point[1]
        color = colors[6]
        plt.scatter(x, y, color=color)

    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('2D Data Points')
    # plt.grid(True)
    plt.show()

# Main program
if __name__ == '__main__':
    data_file = 'build/points_clustered.csv'
    cluster_file = 'build/centroids.csv'

    # Read data points and cluster assignments from CSV file
    data, assignments = read_data_from_csv(data_file)
    data1, assignments1 = read_data_from_csv(cluster_file)
    N = 40000
    idx= random.sample(range(len(data)), 1000)

    data = [data[i] for i in idx]
    assignments = [assignments[i] for i in idx]


    # Plot data points with coloring based on cluster assignments
    plot_data_points(data, assignments, data1)
