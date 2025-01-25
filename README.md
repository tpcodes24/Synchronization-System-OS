# Synchronization-System-OS
# Synchronizer System in Operating Systems  

This repository provides the implementation of a **synchronizer system** in an operating system environment using **mutex locks** and **condition variables**. The system ensures thread-safe execution and has been tested against the following classic concurrency problems:  
- **Monkey Crossing**  
- **Reader-Writer**  
- **Child Care**  

## Features  
- **Mutex Locks:** Ensures mutual exclusion for critical sections.  
- **Condition Variables:** Manages thread coordination using `wait()` and `signal()`/`broadcast()`.  
- **Modular Design:** A reusable synchronizer framework for solving different concurrency challenges.  
- **Problem Testing:** Verified for correctness using Monkey Crossing, Reader-Writer, and Child Care problems.  

## How It Works  
### Core Components  
- **Mutex Locks:** Block access to critical sections during thread execution.  
- **Condition Variables:**  
  - `wait()` suspends threads until specific conditions are met.  
  - `signal()` and `broadcast()` notify one or all waiting threads to proceed.  

### Implementation Highlights  
1. **Shared Variables:** Maintain counters and flags for thread synchronization.  
2. **Locking and Waiting:** Protect shared resources using mutex locks and coordinate threads with condition variables.  
3. **Condition Enforcement:** Implement constraints specific to each problem, ensuring logical correctness.  

## Problem Testing  
### 1. Monkey Crossing  
- **Objective:** Allow monkeys to cross a rope safely without head-on collisions.  
- **Solution:**  
  - Mutex locks ensure only monkeys traveling in the same direction access the rope.  
  - Condition variables manage the switching of crossing directions.  

### 2. Reader-Writer  
- **Objective:** Allow multiple readers to access shared data simultaneously, but ensure writers have exclusive access.  
- **Solution:**  
  - Readers acquire shared access while updating a read counter.  
  - Writers wait until no readers or writers are active.  

### 3. Child Care  
- **Objective:** Maintain a safe ratio of caregivers to children.  
- **Solution:**  
  - Mutex locks protect counters for children and caregivers.  
  - Condition variables enforce the required ratio before allowing additional children.  

## How to Run  
1. **Prerequisites:**  
   - A C/C++ compiler (e.g., GCC).  
   - pthread library installed for multithreading.  

2. **Compilation:**  
   ```bash  
   gcc -pthread synchronizer.c -o synchronizer  
