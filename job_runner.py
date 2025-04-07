import subprocess
import sys
import serial
import time
import threading
import re
import random

# Constants
arduino_port = "/dev/ttyACM0" # Arduino Port
baud_rate = 115200
VOLTAGE = 230.0 # UK Voltage
time_interval = 0.05 # Time interval for energy readings (in seconds)

# Globals
energy_consumed = 0.0 # Total energy consumed
running = True  # Flag to control the energy reading thread
num_readings = 0  # Number of energy readings taken

# Function to read data from the Arduino and calculate energy consumption
def read_arduino():
    global energy_consumed, running, num_readings
    try:
        with serial.Serial(arduino_port, baud_rate, timeout=1) as ser:
            while running:
                try:
                    # Read and decode a line from the Arduino
                    line = ser.readline().decode('utf-8').strip()
                    match = re.search(r"([\d.]+)", line)
                    if match:
                        # Parse current value and calculate power and energy
                        current = float(match.group(1))
                        power = VOLTAGE * current
                        energy_increment = power * (time_interval / 3600)
                        energy_consumed += energy_increment
                        num_readings += 1
                except ValueError:
                    # Ignore invalid readings
                    pass
                time.sleep(time_interval)
    except serial.SerialException:
        print("[ERROR] Unable to access Arduino.")

# Function to generate a random array of integers
def generate_array(length):
    return [random.randint(1, 10000000) for _ in range(length)]

# Function to generate an adjacency matrix for a graph
def generate_graph(n, weighted=False):
    adj_matrix = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if random.random() < 0.3: # 30% chance of creating an edge
                weight = random.randint(1, 20) if weighted else 1
                adj_matrix[i][j] = weight
                adj_matrix[j][i] = weight
    return adj_matrix

# Function to format a graph's adjacency matrix into a flat input string
def format_graph_input(matrix):
    n = len(matrix)
    flat = [str(n)]
    for row in matrix:
        flat.extend(map(str, row))
    return ' '.join(flat)

# Function to print a graph's adjacency matrix in a readable format
def print_adjacency_matrix(matrix):
    n = len(matrix)
    print("\n    " + "  ".join(f"{i:2}" for i in range(n)))
    print("   " + "-" * (3 * n))
    for i, row in enumerate(matrix):
        print(f"{i:2} | " + "  ".join(f"{val:2}" for val in row))

# Function to run a program using MPI and measure its execution time and energy consumption
def run_program(binary, input_data, cores, label):
    global running, energy_consumed, num_readings
    energy_consumed = 0.0
    num_readings = 0
    running = True

    # Construct the MPI command
    run_command = ["mpirun", "--hostfile", "hosts.txt", "-np", str(cores)] + binary.split()

    # Start the energy reading thread
    energy_thread = threading.Thread(target=read_arduino)
    energy_thread.start()
    time.sleep(0.1)

    print(f"\n[INFO] Running {label} on {cores} core(s)...")
    start_time = time.time()

    # Run the program and capture its output
    proc = subprocess.run(run_command, input=input_data,
                          capture_output=True, text=True)

    end_time = time.time()
    running = False
    energy_thread.join()

    exec_time = end_time - start_time

    # Display program output and results
    print("\n--- Program Output ---")
    print(proc.stdout.strip())
    print("----------------------")

    print(f"[RESULT] {label}")
    print(f"[INFO] Execution Time: {exec_time:.4f} seconds")
    print(f"[INFO] Energy Consumed: {energy_consumed:.6f} Wh ({num_readings} readings)\n")

    # Return results as a dictionary
    return {
        "name": label,
        "time": exec_time,
        "energy": energy_consumed,
        "readings": num_readings
    }

# Main function to parse arguments and execute the appropriate program
def main():
    if len(sys.argv) < 4:
        # Display usage instructions if insufficient arguments are provided
        print("Usage:")
        print("  python test.py <program> <input_value> <cores> [--verbose]")
        print("\nExamples:")
        print("  python test.py montecarlo 10000000 4")
        print("  python test.py fibonacci 40 3")
        print("  python test.py bubble 100 2 --verbose")
        print("  python test.py sort 100 2")
        print("  python test.py graph 50 4")
        sys.exit(1)

    # Parse command-line arguments
    program = sys.argv[1].lower()
    input_value = int(sys.argv[2])
    cores = int(sys.argv[3])
    verbose = "--verbose" in sys.argv

    results = []

    # Handle different program options
    if program == "sort":
        array = generate_array(input_value)
        for algo, binary in {
            "bubble": "./bubble_sort",
            "merge": "./merge_sort",
            "quick": "./quick_sort"
        }.items():
            args = [str(input_value)]
            if verbose:
                args.append("--verbose")
            input_data = ' '.join(map(str, array))
            result = run_program(f"{binary} {' '.join(args)}", input_data, cores, f"{algo.capitalize()} Sort")
            if result: results.append(result)

    elif program == "graph":
        graph = generate_graph(input_value)

        print("\n[INFO] Generated Adjacency Matrix:")
        print_adjacency_matrix(graph)

        graph_input = format_graph_input(graph)
        for algo, binary in {
            "BFS": "./bfs",
            "DFS": "./dfs"
        }.items():
            result = run_program(binary, graph_input, cores, f"{algo} Traversal")
            if result: results.append(result)
    
    elif program == "dijkstra":
        graph = generate_graph(input_value, weighted=True)

        print("\n[INFO] Generated Weighted Adjacency Matrix:")
        print_adjacency_matrix(graph)

        graph_input = format_graph_input(graph)
        result = run_program("./dijkstra", graph_input, cores, "Dijkstra Shortest Paths")
        if result:
            results.append(result)

        
    elif program in ["montecarlo", "fibonacci"]:
        binary = f"./{program}"
        result = run_program(f"{binary} {input_value}", "", cores, program.capitalize())
        if result: results.append(result)

    elif program in ["bubble", "merge", "quick"]:
        array = generate_array(input_value)
        args = [str(input_value)]
        if verbose:
            args.append("--verbose")
        input_data = ' '.join(map(str, array))
        binary = f"./{program}_sort"
        result = run_program(f"{binary} {' '.join(args)}", input_data, cores, f"{program.capitalize()} Sort")
        if result: results.append(result)

    else:
        print(f"[ERROR] Unknown program: {program}")
        return

    if len(results) > 1:
        print("\n====== SUMMARY ======")
        for r in results:
            print(f"{r['name']} → Time: {r['time']:.4f}s | Energy: {r['energy']:.6f} Wh ({r['readings']} readings)")
        print("=====================\n")

if __name__ == "__main__":
    main()
