#!/bin/bash

set -e

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
cmake -D CMAKE_C_COMPILER=/opt/homebrew/bin/gcc-13 -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-13 .. && make

echo "Done."

if [ "$1" == "-y" ]; then
  exit 0
fi

echo "Run the program? (y/n)"
read answer
if [ "$answer" == "y" ]; then
# Run the program
# ./example -o "../Data/java8/src/motivationExample/origin/selogger_out" --debug

# Run the program: entryPoint is customClass/myMath.primeFactors
# ./example -o "../Data/java8/src/motivationExample/origin/selogger_out" -c "customClass/myMath" -m "primeFactors"

# Run the program with filtering regexp
./example -o "../Data/java8/src/motivationExample/origin/selogger_out" -g ".*myMath*" -c "Main" -m "main" --debug



else 
    echo "Bye."
    exit 1
fi
