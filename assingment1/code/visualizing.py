import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob

# Function for analyzing results and creating graphs
def analyze_and_visualize_results():
    # List of algorithms
    algorithms = ['BubbleSort', 'SelectionSort', 'InsertionSort', 'MergeSort', 'QuickSort', 'HeapSort', "LibrarySort", "TimSort", "CocktailSort", "IntroSort", "TournamentSort", "CombSort"]
    
    # List of sort types
    sort_types = ['ascending', 'descending', 'random', 'partially_sorted']
    
    # List of data sizes
    sizes = [1000, 10000, 100000, 150000]
    
    # Formatted size labels for x-axis
    size_labels = ['1k', '10k', '100k', '150k']
    
    # Dictionary to store result data
    all_data = {}
    
    # Read result files for all algorithms
    for algorithm in algorithms:
        results_file = f"output/{algorithm}/results.csv"
        if not os.path.exists(results_file):
            print(f"File does not exist: {results_file}")
            continue
        
        try:
            # Load CSV file
            df = pd.read_csv(results_file)
            print(f"{algorithm} data loaded: {len(df)} entries")
            
            # Select necessary columns
            df = df[['Size', 'Type', 'FileNumber', 'RunningTime(ms)']]
            
            # Store algorithm results
            all_data[algorithm] = df
            
        except Exception as e:
            print(f"Error loading {algorithm} data: {e}")
    
    # Create results directories if they don't exist
    os.makedirs("visualizations/by_algorithm", exist_ok=True)
    os.makedirs("visualizations/by_input_type", exist_ok=True)
    
    # 1. Create graphs for each algorithm showing performance across different input types
    for algorithm in algorithms:
        if algorithm not in all_data:
            continue
            
        plt.figure(figsize=(12, 8))
        
        for sort_type in sort_types:
            # Filter data for current algorithm
            df = all_data[algorithm]
            
            # Calculate average running times
            avg_times = []
            for size in sizes:
                filtered_data = df[(df['Type'] == sort_type) & (df['Size'] == size)]
                if not filtered_data.empty:
                    avg_time = filtered_data['RunningTime(ms)'].mean()
                    avg_times.append(avg_time)
                else:
                    avg_times.append(None)
            
            # Use only valid values for graph display
            valid_indices = [i for i in range(len(sizes)) if avg_times[i] is not None]
            valid_sizes = [sizes[i] for i in valid_indices]
            valid_times = [avg_times[i] for i in valid_indices]
            
            if valid_sizes and valid_times:
                plt.plot(valid_sizes, valid_times, marker='o', label=sort_type.capitalize())
        
        # Graph settings
        plt.title(f'{algorithm} - Running Time Comparison by Input Type')
        plt.xlabel('Input Size')
        plt.ylabel('Running Time (ms)')
        plt.xscale('linear')  # Use linear scale for x-axis to show actual proportions
        plt.xticks(sizes, size_labels)  # Set ticks at actual size values
        plt.grid(True, which="both", ls="--")
        plt.legend()
        
        # Save graph in by_algorithm directory
        plt.savefig(f"visualizations/by_algorithm/{algorithm}.png", dpi=300, bbox_inches='tight')
        plt.close()
    
    # 2. Create graphs for each input type showing performance of all algorithms
    for sort_type in sort_types:
        plt.figure(figsize=(12, 8))
        
        for algorithm in algorithms:
            if algorithm not in all_data:
                continue
            
            # Filter data for current algorithm and sort type
            df = all_data[algorithm]
            
            # Calculate average running times
            avg_times = []
            for size in sizes:
                filtered_data = df[(df['Type'] == sort_type) & (df['Size'] == size)]
                if not filtered_data.empty:
                    avg_time = filtered_data['RunningTime(ms)'].mean()
                    avg_times.append(avg_time)
                else:
                    avg_times.append(None)
            
            # Use only valid values for graph display
            valid_indices = [i for i in range(len(sizes)) if avg_times[i] is not None]
            valid_sizes = [sizes[i] for i in valid_indices]
            valid_times = [avg_times[i] for i in valid_indices]
            
            if valid_sizes and valid_times:
                plt.plot(valid_sizes, valid_times, marker='o', label=algorithm)
        
        # Graph settings
        plt.title(f'{sort_type.capitalize()} Input - Running Time Comparison by Algorithm')
        plt.xlabel('Input Size')
        plt.ylabel('Running Time (ms)')
        plt.xscale('linear')  # Use linear scale for x-axis to show actual proportions
        plt.xticks(sizes, size_labels)  # Set ticks at actual size values
        plt.grid(True, which="both", ls="--")
        plt.legend()
        
        # Save graph in by_input_type directory
        plt.savefig(f"visualizations/by_input_type/{sort_type}.png", dpi=300, bbox_inches='tight')
        plt.close()
    
    print("All visualizations completed. Results saved in:")
    print("- 'visualizations/by_algorithm': Graphs showing each algorithm's performance across input types")
    print("- 'visualizations/by_input_type': Graphs showing all algorithms' performance for each input type")

# Run analysis and visualization
if __name__ == "__main__":
    analyze_and_visualize_results()