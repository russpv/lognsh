#!/bin/bash

# Determine how long to sleep in each iteration
# and when to timeout (integral seconds).
sleepInterval=1 timeout=10 elapsed=0 timedOut=0

# Spinner function
spin() {
  local i=0
  local sp='/-\|'
  local n=${#sp}
  
  printf 'Processing: '
  tput civis # Hide cursor.
  # To be safe, ensure that the cursor is turned back on when
  # the script terminates, for whatever reason.
  trap 'tput cnorm' EXIT
  while sleep 0.1; do
    printf '\b%s' "${sp:i++%n:1}"
    [ $elapsed -ge $timeout ] && { timedOut=1; printf '\nTIMED OUT\n' >&2; break 2; }
    # sleep $sleepInterval
    elapsed=$(( elapsed + sleepInterval ))
  done
  tput cnorm # Show cursor again.
}

spin
