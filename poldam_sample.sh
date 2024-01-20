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
cmake -D CMAKE_C_COMPILER=/usr/bin/gcc -D CMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-11 .. && make

echo "Done."

if [ "$1" == "-y" ]; then
  exit 0
fi

echo "Run the program? (y/n)"
read answer
if [ "$answer" == "y" ]; th
else 
    echo "Bye."
    exit 1
fi
