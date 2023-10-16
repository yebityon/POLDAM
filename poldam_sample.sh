
set -e

# imoplement this at /PODLDAM/#!/bin/bash

TARGET_DIR="./build"

if [ -d "$TARGET_DIR" ]; then
  echo "OK, $TARGET_DIR will be used."
else
  read -p "Target directory does not exist. Do you want to create  it? (y/n) " answer
  if [ "$answer" == "y" ]; then
    mkdir "$TARGET_DIR"
  else
    echo "use existing directory."
  fi
fi

cd "$TARGET_DIR"

echo "Current directory: $PWD"
echo "building..."
cmake .. && make 

echo "Done."

echo "Run the program? (y/n)"
read answer
if [ "$answer" == "y" ]; then
  ./POLDAM -o "../Data/java8/src/motivationExample/origin/selogger_out" -t "../Data/java8/src/motivationExample/target/selogger_out"
else 
    echo "Bye."
    exit 1
fi
