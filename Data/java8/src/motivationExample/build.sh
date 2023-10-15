set -e

compile_and_run() {
    local directory=$1
    local class_name=$2

    echo compile "$class_name"
    cd "$directory"
    rm *.class || true
    javac -g "$class_name.java"
    cd ../

    echo compile Main
    rm *.class || true
    javac -g Main.java

    echo run selogger with omni
    java  -javaagent:../../../selogger/selogger/target/selogger-0.5.0.jar=output=selogger_out,format=omni,waverlog=hogehoeg_log.txt Main
}

# create log dir for origin
cd origin/

compile_and_run customClass myMath

#======================== target ================================
cd ../target

compile_and_run customClass myMath
