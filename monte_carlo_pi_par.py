import random
import time
import argparse
from concurrent.futures import ProcessPoolExecutor

def monte_carlo_pi_partial(num_samples):
    """Function to calculate the number of points inside the circle for a subset of samples."""
    inside_circle = 0
    for _ in range(num_samples):
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        if x**2 + y**2 <= 1:
            inside_circle += 1
    return inside_circle

def monte_carlo_pi_parallel(total_samples, num_workers):
    """Parallel Monte Carlo Pi computation."""
    samples_per_worker = total_samples // num_workers
    remaining_samples = total_samples % num_workers  # Handle remainder

    # Distribute the work across workers
    sample_counts = [samples_per_worker] * num_workers
    if remaining_samples > 0:
        sample_counts[0] += remaining_samples  # Add extra samples to the first worker

    with ProcessPoolExecutor(max_workers=num_workers) as executor:
        results = executor.map(monte_carlo_pi_partial, sample_counts)

    # Aggregate results
    total_inside_circle = sum(results)
    pi_estimate = (total_inside_circle / total_samples) * 4
    return pi_estimate

if __name__ == "__main__":
    # Argument Parsing
    parser = argparse.ArgumentParser(description="Parallel Monte Carlo Pi Simulation")
    parser.add_argument("iterations", type=int, help="Number of iterations for the simulation")
    parser.add_argument("--workers", type=int, default=4, help="Number of parallel workers (default: 4)")
    args = parser.parse_args()

    # Run Parallel Monte Carlo Simulation
    start = time.time()
    pi_estimate = monte_carlo_pi_parallel(args.iterations, args.workers)
    end = time.time()

    print(f"Estimated Pi: {pi_estimate}")
    print(f"Execution Time: {end - start:.2f} seconds")
