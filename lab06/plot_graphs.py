import matplotlib.pyplot as plt

# --- YOUR ACTUAL LAB DATA ---
processors = [1, 2, 4]

# Execution times recorded from your terminal output
time_fib = [0.249159, 1.051421, 1.180062]
time_sum = [0.025427, 0.014316, 0.027956] 
time_pi  = [0.361877, 0.270110, 0.154999]

# --- CALCULATING SPEEDUP (S = T1 / Tn) ---
def calc_speedup(times):
    if times[0] == 0.0: return [0.0, 0.0, 0.0]
    return [times[0] / t for t in times]

speedup_fib = calc_speedup(time_fib)
speedup_sum = calc_speedup(time_sum)
speedup_pi  = calc_speedup(time_pi)

# --- PLOT 1: TIME VS PROCESSORS ---
plt.figure(figsize=(8, 5))
plt.plot(processors, time_fib, marker='o', label='OpenMP Fibonacci')
plt.plot(processors, time_sum, marker='s', label='MPI Array Sum')
plt.plot(processors, time_pi, marker='^', label='MPI Monte Carlo Pi')
plt.title('Execution Time vs Number of Processors')
plt.xlabel('Number of Processors')
plt.ylabel('Time (seconds)')
plt.xticks(processors)
plt.legend()
plt.grid(True)
plt.savefig('time_vs_processors.png')
plt.clf() # Clear the canvas for the next plot

# --- PLOT 2: SPEEDUP VS PROCESSORS ---
plt.figure(figsize=(8, 5))
plt.plot(processors, speedup_fib, marker='o', label='OpenMP Fibonacci')
plt.plot(processors, speedup_sum, marker='s', label='MPI Array Sum')
plt.plot(processors, speedup_pi, marker='^', label='MPI Monte Carlo Pi')
plt.plot(processors, processors, 'k--', label='Ideal Speedup') # The perfect 1:1 scaling line
plt.title('Speedup vs Number of Processors')
plt.xlabel('Number of Processors')
plt.ylabel('Speedup')
plt.xticks(processors)
plt.legend()
plt.grid(True)
plt.savefig('speedup.png')

print("Graphs successfully generated and saved as PNG files!")
