#!/bin/bash

# Path to your custom malloc override shared library
MALLOC_LIB="./malloc_override.so"  # Replace with the correct path if necessary

# Path to the compiled program (e.g., your shell executable)
PROGRAM="../../minish"  # Replace with the correct path to your shell program

# Example command to run with your shell (adjust as needed)
ARGS="echo haha>out"

# Set the environment variable for LD_PRELOAD to use your custom malloc
export LD_PRELOAD=$MALLOC_LIB

# Number of runs to perform (e.g., 1000 iterations for the test)
NUM_RUNS=1000

# Set up pipes
for i in $(seq 1 $NUM_RUNS); do
    echo "Running Valgrind with simulated malloc failure (iteration #$i)..."

    # Set CALL_COUNT for the current iteration (if you are simulating malloc failure)
    export CALL_COUNT=$i

	rm -f pipe_in pipe_out
    # Create pipes for communication with the child process
    mkfifo pipe_in pipe_out

    # Run the program under Valgrind with the necessary flags
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
        $PROGRAM < pipe_in > pipe_out &  # Redirect stdin from pipe_in and stdout to pipe_out

	echo "Ran program"
    # Send the input to the program via the pipe (simulate fgets input)
    echo -n $ARGS > pipe_in  # Send the input command via the pipe

    # Wait for the program to finish
#    wait $!

    # Read the output from pipe_out (the program's output)
    cat pipe_out

    # Clean up the pipes after use
    rm pipe_in pipe_out

    # Add a separator for readability
    echo "----------------------------------------"
done

