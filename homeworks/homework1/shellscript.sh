#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: $0 file1 [file2 ...];"
  exit 1
fi

total_lines=0
file_count=1
output_file="line_count_results.txt"

echo "Line Count Results" > $output_file
echo "------------------" >> $output_file

for file in "$@"; do
  if [ -f "$file" ]; then
    lines=$(wc -l < "$file")
    total_lines=$((total_lines + lines))
    file_count=$((file_count + 1))
    echo "$file: $lines lines (Running total: $total_lines)" | tee -a $output_file
  else
    echo "$file not found:" | tee -a $output_file
  fi
done

echo "-----------------" | tee -a $output_file
echo "Total files checked: $file_count" | tee -a $output_file
echo "Final total lines: $total_lines" | tee -a $output_file
