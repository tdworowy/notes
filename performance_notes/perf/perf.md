-1: Allow use of (almost) all events by all users
      Ignore mlock limit after perf_event_mlock_kb without CAP_IPC_LOCK  
\>= 0: Disallow raw and ftrace function tracepoint access  
\>= 1: Disallow CPU event access  
\>= 2: Disallow kernel profiling
``` bash
sudo sysctl -w kernel.perf_event_paranoid=-1
```

Sample any porogram runing on CPU at 99 Hertz for 30 s
``` bash
perf record -F 99 -a -- sleep 30 
perf report --stdio
```

trace
``` bash
perf trace -p $(pgrep mysqld)
```

list all known events
``` bash
perf list
```

list sched tracepoints
``` bash
perf list 'sched:*'
```

list events witch "block" in names
``` bash
perf list block
```

list available dynamic probes
``` bash
perf probe -l
```

show PMC statistic for command
``` bash
perf stat <command>
```

show PMC statistic for PID
``` bash
perf stat -p PID
```

show PMC statistic for entire system
``` bash
perf stat -a
```

show CPU last level cache (LLC) statistics for command
``` bash
perf stat -e LLC-loads,LLC-loed-misses,LLC-stores,LLC-prefatches <command>
```

count unhalted core cycles using a PMC raw specification (Intel)
``` bash
perf stat -e r003c -a sleep 5
```

count front-end stalls using a verbouse PMC raw specification (Intel)  
``` bash
perf stat -e  cpu/event=0x0e,umask0x01,inv,cmask=0x01 -a sleep 5
```

count syscalls per second system-wide
``` bash
perf stat -e  raw_syscalls:sys_enter -I 1000 -a
```

count syscalls by type for the specified PID
``` bash
perf stat -e  'syscalls:sys_enter_*' -p PID
```

count block device I/O events for the entire system
``` bash
perf stat -e  'block:*' -a sleep 10
```

sample CPU stack traces (via frame pointers)
``` bash
perf record -F 99 -a -g -- sleep 10 
```

sample CPU stack traces for the PID, using dwarf(debuginfo)
to unwind stacls
``` bash
perf record -F 99 -p PID --cell-graph dwarf sleep 10
```

sample CPU stack traces for a container by 
its /sys/fs/cgroup/pref_event cgroup
``` bash
perf record -F 99 -e cpu-clock --cgroup=docker/1d5..etc..
-a sleep 10 
```

sample CPU stack traces for entire system, using last
branch record (LBR;Intel)
``` bash
perf record -F 99 -a --cell-graph ibr sleep 10
```

sample CPU stack traces, onece every 100 last-level cache misses
``` bash
perf record -e LLC-load-misses -c 100 -ag sleep 5
```

sample on-CPU user instructions precisely (e.g, using Intel PEBS)
``` bash
perf record -e cycles:up -a sleep 5
```

sample CPUs at 49 Hertz, and show top process names 
and segments
``` bash
perf top -F 49 -ns comm,dso
``` 

trace processes
``` bash
perf record -e sched:sched_process_exec -s
```

sample a subset of context switches with stack traces
``` bash
perf record -e context-switches -a -g sleep 1
```

trace all context switches with stact traces
``` bash
perf record -e sched:sched_switch -a -g sleep 1
```

trace all context switches with 5-level deep stact traces
``` bash
perf record -e sched:sched_switch/max-stack=5/ -a sleep 1
```

trace connect(2) calls (outbound connections) with stack traces
``` bash
perf record -e syscalls:sys_enter_connect -a -g
```
sample at most 100 block devuce reqyests per second
``` bash
perf record -F 100 -e block:block_rq_issue -a
```

trace all block device issues and completions
``` bash
perf record -e block:block_rq_issue,block:block_rq_complete -a
```

trace all block requests, of size at least 64KB
``` bash
perf record -e block:block_rq_issue --filter 'bytes > =65536'
```

tarce all ext4 calls, and write to a non-ext4 locations
``` bash
perf record -e "ext4:*" -o /tmp/perf.data -a
```

trace the http__server__request USDT event (from Node.js)
``` bash
perf record -e sdt_node:http__server__request -a
```

trace block device requests with live output (no perf.data)
``` bash
perf record -e block:block_rq__issue
```

trace block device requests and completions whth live output
``` bash
perf trace -e block:block_rq__issue,block_rq__complete
```

add a probe for the kernel tcp_sendmsg() function entry 
``` bash
perf probe --add tcp_sendmsg
```

remvoe a probe for the kernel tcp_sendmsg() function entry 
``` bash
perf probe --del tcp_sendmsg
```

list available variables for tcp_sendmsg() plus externals 
(needs kernel debuginfo)
add a probe for the kernel tcp_sendmsg() function entry 
``` bash
perf probe --V tcp_sendmsg --externs
```

list available line probes for tcp_sendmsg()
``` bash
perf probe --L tcp_sendmsg
```

list available variables for tco_sendmsg() at line 81
(needs kernel debuginfo)
``` bash
perf probe --V tcp_sendmsg:81
```

add a probe for tcp_sendmsg() with entry argument registers
``` bash
perf probe 'tcp_sendmsg %ax %dx %cx'
```

add a probe for tcp_sendmsg() with an alias ("bytes")
for %cx register
``` bash
perf probe 'tcp_sendmsg bytes=%cx'
```

trace previously created probe when bytes(alias) is greater than 100
``` bash
perf recod --e probe:tcp_sendmsg --filter 'bytes > 100'
```

add a tarcepoint for tcp_sendmsg() return, and capture the return value
``` bash
perf probe 'tcp_sendmsg%return $retval'
```

add a tracepoint for tcp_sendmsq() with size and socket state
(needs kernel debuginfo)
``` bash
perf probe 'tcp_sendmsg size sk>__sk_common.sk_state'
```

add a tracepoint for do_sys_open() with filename as a string
(needs kernel debuginfo)
``` bash
perf probe 'do_sys_open filename:string'
```

add a tracepoint for the user-level fopen(3) fucntion from libc
``` bash
perf probe -x /lib/x86_64-linux-gnu/lobc.os.6 --add fopen
```

show perf.data in an ncurses browser (TUI) if possible
``` bash
perf report
```

show perf.data as text report
``` bash
perf report -n --stdio
```

list all perf.data events
``` bash
perf script --header
```

list all perf.data events, with specified fields
``` bash
perf script --header -F comm,pid,tid,cpu,time,event,ip,sym,dso
```

generate a flame graph visualization 
``` bash
perf script report flamegraph
```

disassemble and annotate instructions with percentages 
(needs kernel debuginfo)
``` bash
perf annotate --stdio
```