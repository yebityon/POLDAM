echo compiling custom class
javac origin/customClass/*.java

# create log dir for origin
cd origin/
echo compile Main
javac Main.java
echo run selogger with omni
java  -javaagent:../../../selogger/target/selogger-0.5.0.jar=output=selogger_out,format=omni,waverlog=hogehoeg_log.txt Main

# echo run selogger with omnibinary
# java -javaagent:../../../selogger/target/selogger-0.5.0.jar=output=selogger_bin_out,format=omnibinary, Main

# echo run selogger.reader
# java -classpath ../../../selogger/target/selogger-0.5.0.jar selogger.reader.LogPrinter 

# create Log dir for target
cd ../target
javac Main.java
echo run selogger with omni
java  -javaagent:../../../selogger/target/selogger-0.5.0.jar=output=selogger_out,format=omni,waverlog=hogehoeg_log.txt Main

# rm Main.class