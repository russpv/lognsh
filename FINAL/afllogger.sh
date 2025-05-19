#!/bin/bash

CRASH_DIR="fuzz_output/default/crashes"
LOG_FILE="crash_log.txt"

echo "[*] Logging crashes from $CRASH_DIR to $LOG_FILE"
> "$LOG_FILE"

for crash in "$CRASH_DIR"/id:*; do
    echo "==== $crash ====" >> "$LOG_FILE"
    echo "-- Hexdump --" >> "$LOG_FILE"
    xxd "$crash" >> "$LOG_FILE"
    echo "-- Printable view --" >> "$LOG_FILE"
    cat "$crash" | sed -E 's/[^[:print:]\t\r\n]/./g' >> "$LOG_FILE"
    echo -e "\n" >> "$LOG_FILE"
done

echo "[+] Done. See $LOG_FILE"

