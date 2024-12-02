#!/bin/bash

# Verify that required files and directories exist
if [ ! -f /usr/bin/writer ]; then
    echo "Error: writer not found in /usr/bin"
    exit 1
fi

if [ ! -f /usr/bin/finder.sh ]; then
    echo "Error: finder.sh not found in /usr/bin"
    exit 1
fi

if [ ! -d /etc/finder-app/conf ]; then
    echo "Error: Configuration directory not found in /etc/finder-app/conf"
    exit 1
fi

# Run the finder script and write output to /tmp/assignment4-result.txt
/usr/bin/finder.sh /var/log /etc/finder-app/conf > /tmp/assignment4-result.txt

# Test completed successfully
echo "Test completed. Output written to /tmp/assignment4-result.txt."
