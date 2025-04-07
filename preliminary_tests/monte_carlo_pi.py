import random
import time
import argparse

def monte_carlo_pi(num_samples):
    inside_circle = 0
    for _ in range(num_samples):
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        if x**2 + y**2 <= 1:
            inside_circle += 1
    return (inside_circle / num_samples) * 4

if __name__ == "__main__":
    # Argument Parsing
    parser = argparse.ArgumentParser(description="Monte Carlo Pi Simulation")
    parser.add_argument("iterations", type=int, help="Number of iterations for the simulation")
    args = parser.parse_args()

    # Run Monte Carlo Simulation
    start = time.time()
    pi_estimate = monte_carlo_pi(args.iterations)
    end = time.time()

    print(f"Estimated Pi: {pi_estimate}")
    print(f"Execution Time: {end - start:.2f} seconds")
