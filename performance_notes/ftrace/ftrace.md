list [Ftrace](https://www.kernel.org/doc/html/v5.0/trace/ftrace.html) tracers availabe on your kernel
``` bash
cat /sys/kernel/debug/tracing/available_tracers
```

[trace-cmd](https://man7.org/linux/man-pages/man1/trace-cmd.1.html)

record kernel function do_nanosleep() via the function tracer for
ten second (using a dummpy sleep(1) command)
``` bash
trace-cmd record -p function -1 do_nanosleep sleep 10
trace-cmd report
```

list all tracing event sources and options
``` bash
trace-cmd list
```

list Ftrace tracers
``` bash
trace-cmd list -t
```

list event sources
``` bash
trace-cmd list -e
```

list syscall tracepoints
``` bash
trace-cmd list -e syscalls:
```

show the format file for given tracepoint
``` bash
trace-cmd list -e syscalls:sys_enter_nanosleep -F
```

trace a kernel function system-wide
``` bash
trace-cmd record -p function -l <function_name>
```

trace all kernel functions beginning with "tcp_", system-wide 
``` bash
trace-cmd record -p function -l 'tcp_*'
```

trace all kernel functions beginning with "vfs_" for the ls(1) command
``` bash
trace-cmd record -p function -l 'vfs_' -F ls
```

trace all kernel functions beginning with "vfs_" for the bash(1)
and its children
``` bash
trace-cmd record -p function -l 'vfs_' -F -c bash
```

trace all kernel functions beginning with "vfs_" for PID 21124
``` bash
trace-cmd record -p function -l 'vfs_' -P 21124
```

trace a kernel function and ist child calls, system-wide
``` bash
trace-cmd record -p function_graph -g <function_name>
```

trace new process via the shed:shed_process_exec tracepoint
``` bash
trace-cmd record -e shed:sched_process_exec
trace-cmd record -e sched_process_exec
```

trace block I/O requests with kernel stack traces
``` bash
trace-cmd record -e block_rq_issue -T
```

trace all block tracepoints
``` bash
trace-cmd record -e block
```

trace a previously created kprobe named "test1" for 10 s
``` bash
trace-cmd record -e probe:test1 sleep 10
```

trace all yusscalls for the ls(1) command
``` bash
trace-cmd record -e syscalls -F ls
```

print a contents of the trace.dat output file
``` bash
trace-cmd report
```

print a contents of the trace.dat output file CPU 0 only
``` bash
trace-cmd report --cpu 0
```

trace events from the shed_switch plugin
``` bash
trace-cmd record -p sched_switch
```

listen for tracing request on TCP port 8081
``` bash
trace-cmd listen -p 8081
```

connect to remote host for runing a record subcommand
``` bash
trace-cmd record ... -N addr:port
```
