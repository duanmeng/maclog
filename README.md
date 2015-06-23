# maclog
A multithreading splittable lightweight log class

#Features
1. Thread safe, can be used in multithreading programmes.
2. Splittable, the log file can be splitted by year/month/day/hour/miniute/seconds 
   or a single file or stdout according the configuration.
3. Auto add the standard log header to each item:
          log_level date time(accurate to milliseconds) file:line ThreadID "message input by user".
4. Can set log level to output (DEBUG/INFO/WARNING/ERROR/FATAL).
5. Easy to use just, just the the macro (DEBUG/INFO/WARNING/ERROR/FATAL) to output corresponding level log item.

#How to use
1. Include the log.h.
2. Set the log file path pattern (linux timestamp format to split log file) and 
   log level by using logging::Log::init_log in main.
3. Use the macro (DEBUG/INFO/WARNING/ERROR/FATAL) when you want to print log in any file including log.h.

For example,  call
    logging::Log::init_log("/home/duanmeng/tmp/log.%Y-%m-%d-%H-%M.%S", logging::Log::DEBUG);
"/home/duanmeng/tmp" is the dir where the log files saved, log.%Y-%m-%d-%H-%M.%S is the log 
file name pattern (linux timestamp format),
as you can see, the programme would create log file each seconds for the log file name pattern is accurated to seconds.
And of course you can split by hour using "log.%Y-%m-%d.%H", by day using "log.%Y-%m.%d" or not split just using "log".

If you only care about the log item whose level is above ERROR (including ERROR), you can use logging::Log::ERROR.
And you can print log to stdout by call logging::Log::init_log(NULL, logging::Log::DEBUG)

#A First Glance of the log item and log file name
This is part of log file output by using 
logging::Log::init_log("/home/duanmeng/tmp/log.%Y-%m-%d-%H-%M.%S", logging::Log::ERROR)
and output by using ERROR("sucker error %d %s %f", 1, "hello", 3.14), 
FATAL("sucker fatal %d %s %f", 1, "hello", 3.14).

loglevel   data      time         file:line  ThreadID     message input by user
ERROR   2015-06-22 06:42:05:510 log.cpp:136 3040992064  sucker error 1 hello 3.140000
FATAL   2015-06-22 06:42:05:510 log.cpp:136 3040992064  sucker fatal 1 hello 3.140000
ERROR   2015-06-22 06:42:05:511 log.cpp:136 3049384768  sucker error 1 hello 3.140000
FATAL   2015-06-22 06:42:05:511 log.cpp:136 3049384768  sucker fatal 1 hello 3.140000

duanmeng@ubuntu:~/work/common/cpp/log$ ll /home/duanmeng/tmp/log.2015-06-22-06-42.*    
-rw-rw-r-- 1 duanmeng duanmeng 2130 Jun 22 06:42 /home/duanmeng/tmp/log.2015-06-22-06-42.05
-rw-rw-r-- 1 duanmeng duanmeng 2130 Jun 22 06:42 /home/duanmeng/tmp/log.2015-06-22-06-42.06
-rw-rw-r-- 1 duanmeng duanmeng 2130 Jun 22 06:42 /home/duanmeng/tmp/log.2015-06-22-06-42.07
-rw-rw-r-- 1 duanmeng duanmeng 2130 Jun 22 06:42 /home/duanmeng/tmp/log.2015-06-22-06-42.08


