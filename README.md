
# Analysis of Energy Efficiency in Cluster Computing for Green HPC

## Description
This software suite enables analysis of energy efficiency on a number of different workloads designed to work on different cluster systems (note in this case, the system was implemented on a Pi Bramble with 3 worker nodes and 1 co-ordinator node, however, in theory could be expanded with slight changes to work on a larger cluster)

Algorithms Include:

- Monte Carlo Pi approximation
- Recursive Fibonacci
- Sorting: Bubble, Merge, Quick
- Graph algorithms: BFS, DFS, Dijkstra


Execution and energy measurement are handled via `job_runner.py` using Arduino-based current sensing (for which the code can be found in the acknowledgements section)

## Authors

- [@cathalmcstay](https://github.com/cathalmcstay)


## Acknowledgements

 - [Code for outputting amperage from Arduino](https://wiki.dfrobot.com/Gravity_Analog_AC_Current_Sensor__SKU_SEN0211_)


# Requirements

This system was developed with **Debian-based Linux distributions** in mind (e.g. Raspberry Pi OS, Ubuntu). If you wish to compile the C binaries and run the Python scripts yourself, ensure the following software is installed.

### C Environment
The C programs use **OpenMPI** for parallel and distributed execution. To install the compiler and required MPI libraries:
```bash
sudo apt update
sudo apt install build-essential openmpi-bin libopenmpi-dev mpich
```

### Python Environment
The orchestration and energy monitoring is written in **Python 3.7+**. You will need the following packages:

- `pyserial` (for Arduino communication)

You can install Python dependencies using the provided `requirements.txt` file:
```bash
pip install -r requirements.txt
```

### Additional Libraries
- **GMP Library** is required for large-number Fibonacci calculations:
```bash
sudo apt install libgmp-dev
```

### Hosts.txt

In addition to the above, OpenMPI requires a hosts.txt file which holds the ip address/name of the nodes in your system. ## Installation

## Installation

1. **Clone the repository**  
   ```
   git clone https://gitlab.eeecs.qub.ac.uk/40293149/csc4006researchproject.git
   cd csc4006researchproject
   ```

2. **Install dependencies**  
   ```
   sudo apt update
   sudo apt install libgmp-dev libopenmpi-dev mpich python3-serial
   pip install -r requirements.txt
   ```

3. **Compile programs**  
   ```
   mpicc bubble_sort.c -o bubble_sort
   mpicc quick_sort.c -o quick_sort
   mpicc merge_sort.c -o merge_sort
   mpicc montecarlo.c -o montecarlo
   mpicc fibonacci.c -lgmp -o fibonacci
   mpicc bfs.c -o bfs
   mpicc dfs.c -o dfs
   mpicc dijkstra.c -o dijkstra
   ```

4. **Use shared storage or scp to distribute binaries across nodes**

    ```
    scp bubble_sort pi@192.168.1.100:~/cluster/bin/
    ```
## Execution Examples

### Sort Example (Bubble, Merge, Quick)
```bash
python3 job_runner.py sort 1000 3 --verbose
```

### Monte Carlo Pi
```bash
python3 job_runner.py montecarlo 10000000 3
```

### Fibonacci (n = 40)
```bash
python3 job_runner.py fibonacci 40 2
```

### Graph Algorithms (n = 25 nodes)
```bash
python3 job_runner.py graph 25 2
```

### Dijkstra
```bash
python3 job_runner.py dijkstra 30 2
```

Each program outputs:
- The MPI rank output
- Execution time
- Energy consumption (in watt-hours)

## Replication Guide

1. **Connect the Arduino** to the head node (`/dev/ttyACM0`) and confirm it's reporting current readings.

2. **Ensure your `hosts.txt`** contains the IP or hostname of each participating Raspberry Pi (excluding the Arduino coordinator node).

3. **Run tests** using `job_runner.py`.

4. **Sample output includes:**
   ```
   [INFO] Execution Time: 2.1334 seconds
   [INFO] Energy Consumed: 0.001347 Wh (27 readings)
   ```
## License

This project is licensed under the [MIT License](LICENSE).


