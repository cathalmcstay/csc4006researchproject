import subprocess
import time
import socket
import argparse

# Configuration
node4_ip = "192.168.0.122"
energy_server_port = 5000
log_file = "job_log.csv"

# Function to Get Power (Watts) from Node 4
def get_power():
    try:
        with socket.create_connection((node4_ip, energy_server_port), timeout=5) as sock:
            power = sock.recv(1024).decode().strip().replace(" W", "")
            print(f"Power from Node 4: {power} W")
            return float(power)
    except socket.error as e:
        print(f"Error connecting to Node 4: {e}")
        return -1

# Function to Run a Job and Measure Energy Use
def run_job(cmd):
    print(f"Starting job: {cmd}")

    # Record Initial Power
    initial_power = get_power()
    if initial_power == -1:
        print("Failed to get initial power reading.")
        return

    start_time = time.time()

    # Run the Specified Program
    process = subprocess.Popen(cmd, shell=True)
    process.wait()

    end_time = time.time()
    elapsed_time = end_time - start_time

    # Record Final Power
    final_power = get_power()
    if final_power == -1:
        print("Failed to get final power reading.")
        return

    # Calculate Energy in Joules (Watts per second)
    avg_power = (initial_power + final_power) / 2
    energy_used = avg_power * elapsed_time

    # Log Results
    with open(log_file, "a") as f:
        f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')},{cmd},{elapsed_time:.2f},{avg_power:.2f},{energy_used:.2f} J\n")

    print(f"Job completed in {elapsed_time:.2f} seconds")
    print(f"Average Power: {avg_power:.2f} W")
    print(f"Energy used: {energy_used:.2f} J")

# Function to Benchmark Energy Usage Over Time
def benchmark_energy(duration):
    print(f"Starting energy benchmark for {duration:.2f} seconds...")

    start_time = time.time()
    total_energy = 0.0
    samples = 0

    while time.time() - start_time < duration:
        power = get_power()
        if power == -1:
            print("Failed to get power reading. Skipping sample.")
        else:
            total_energy += power
            samples += 1
        time.sleep(1)  # Sample every second

    avg_power = total_energy / samples if samples > 0 else 0
    total_energy_used = avg_power * duration  # Joules (W·s)

    # Log Results
    with open(log_file, "a") as f:
        f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')},BENCHMARK,{duration:.2f},{avg_power:.2f},{total_energy_used:.2f} J\n")

    print(f"Benchmark completed over {duration:.2f} seconds")
    print(f"Average Power: {avg_power:.2f} W")
    print(f"Total Energy Used: {total_energy_used:.2f} J")

# Main Program Logic
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Submit a job or benchmark energy usage.")
    subparsers = parser.add_subparsers(dest="mode", required=True)

    # Run Job Parser
    run_parser = subparsers.add_parser("run", help="Run a job and measure energy consumption.")
    run_parser.add_argument("program", type=str, help="The command to run the program.")
    run_parser.add_argument("iterations", type=int, help="Number of iterations used in the program.")

    # Benchmark Parser
    benchmark_parser = subparsers.add_parser("benchmark", help="Benchmark energy usage over a fixed duration.")
    benchmark_parser.add_argument("duration", type=float, help="Duration for energy benchmarking in seconds.")

    args = parser.parse_args()

    # Ensure Log File Exists
    try:
        open(log_file, "x").write("Timestamp,Command,Time (s),Average Power (W),Energy (J)\n")
    except FileExistsError:
        pass

    # Run the Selected Mode
    if args.mode == "run":
        job_command = f"{args.program} {args.iterations}"
        run_job(job_command)
    elif args.mode == "benchmark":
        benchmark_energy(args.duration)
