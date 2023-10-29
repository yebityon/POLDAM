#!/bin/bash

set -e

compile_and_run() {
    local directory=$1
    local class_name=$2
    local class_name2=$3

    echo "compile $class_name"
    cd "$directory"
    rm -f *.class
    javac -g "$class_name.java"
    cd ../

    if [ $# -eq 3 ]; then
        echo "compile $class_name2"
        cd "$directory"
        rm -f *.class
        javac -g "$class_name2.java"
        cd ../
    fi

    echo "compile Main"
    rm -f *.class
    javac -g Main.java

    echo "run selogger with omni"
    java -javaagent:../../../selogger/selogger/target/selogger-0.5.0.jar=output=selogger_out,format=omni,waverlog=hogehoeg_log.txt Main
}

# create log dir for origin
cd origin/
compile_and_run customClass myMath

#======================== target ================================
cd ../target
if [ $# -eq 2 ]; then
    compile_and_run customClass myMath
else
    compile_and_run customClass myMath myString
fi