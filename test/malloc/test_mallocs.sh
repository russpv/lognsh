#!/bin/bash

# Set the path to your executable (adjust accordingly)
PROGRAM="./test_script"  # Replace with the path to your compiled program

# Example arguments to pass to your program (adjust as needed)
ARGS="\"echo haha>out\""

# Number of runs to perform with different call_count values (1, 2, 3,...)
NUM_RUNS=1000  # Adjust this depending on how many times you want to run

# Step 1: Compile the shared library with your custom my_malloc
# echo "Compiling the shared library with the custom my_malloc..."
gcc -shared -fPIC -o $MALLOC_LIB malloc_override.c -ldl

# Path for the shared library with overridden my_malloc
MALLOC_LIB="./malloc_override.so"  # Path to the shared library


# Loop over different call_count values
for i in $(seq 1 $NUM_RUNS); do
  echo "Running Valgrind with simulated my_malloc failure on call #$i..."

  # Set the environment variable for the call_count you want to test
  unset CALL_COUNT
  export CALL_COUNT=$i
  
   # Set the LD_PRELOAD environment variable to use your custom my_malloc
  export LD_PRELOAD=$MALLOC_LIB

  # Run the program under Valgrind, using the custom my_malloc via LD_PRELOAD
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes $PROGRAM $ARGS
  
  # Optionally, unset the CALL_COUNT variable after each test run
  # unset LD_PRELOAD

  # Add a separator for readability
  echo "----------------------------------------"
done

