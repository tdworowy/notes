```bash
bpftrace -e 't:syscalls:sys_enter_kill{time("%H:%M:%S "); printf("%s (PID %d) send a SIG %d to PID %d\n", comm, pid, args->sig, args->pid);}'
```

```bash
bpftrace -e 't:syscalls:sys_enter_recvfrom { @ts[tid] = nsecs; } t:syscalls:sys_exit_recvfrom /@ts[tid]/{ @usces = hist((nsecs - @ts[tid]) / 1000); delete(@ts[tid]);}'
```

trace new processes with arguments
```bash
bpftrace -e 'tracepoint:syscalls:sys_enter_execve { join(args->argv); }'
```

count syscalls by process
```bash
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[pid, comm] = count(); }'
```

sample user-level stacks at 49 Herts, for PID 189
```bash
bpftrace -e 'profile:hz:49 /pid == 189/ { @[ustack] = count(); }'
```

count process heap expansion (brk()) by code path
```bash
bpftrace -e 'tracepoint:syscalls:sys_enter_brk { @[ustack, comm] = count(); }'
```

count user page faults by user-level stack trace 
```bash
bpftrace -e 'tracepoint:exceptions:page_fault_user { @[ustack, comm] = count(): }'
```

count vmscan operations by tracepoint
```bash
bpftrace -e 'tracepoint:vmscan:* { @[probe]++; }'
```

trace files opened via openat(2) with process name
```bash
bpftrace -e 't:syscalls:sys_enter_openat {printf("%s %S\n", comm, str(args->filename)); }'
```

show the distributions of read() syscall read bytes (and errors)
```bash
bpftrace -e 'tracepoint:syscalls:sys_exit_read { @ = hist(args->ret); }' 
```

count VFS calls
```bash
bpftrace -e 'kprobe:vfs_* { @[probe] = count(); }' 
```

count ext4 tracepoint calls
```bash
bpftrace -e 'tracepoint:ext4:* { @[probe] = count(); }' 
```

summarize block I/O size as a histogram
```bash
bpftrace -e 't:block:block_rq_issue { @bytes = hist(args->bytes); }'
```

count block I/O requests user stack traces 
```bash
bpftrace -e 't:block:block_rq_issue { @[ustack] = count(); }'
```

count block I/O type flags
```bash
bpftrace -e 't:block:block_rq_issue { @[args->rwbs] = count(); }'
```

count socket accept(2) by PID and process name
```bash
bpftrace -e 't:syscalls:sys_enter_accept* { @[pid, comm] = count(); }'
```

count socket send/recive bytes by on-CPU PID and process name
```bash
bpftrace -e 'kr:socket_sendmsg,kr:sock_recmsg /retval > 0/ { @[pid, comm] = sum(retval); }'
```

TCP send bytes as a histogram 
```bash
bpftrace -e 'k:tcp_sendmsg { @send_bytes = hist(arg2); }'
```

TCP receive bytes as histogram 
```bash
bpftrace -e 'k:tcp_recvmsg /retval >= 0/ { @recv_bytes = hist(retval); }'
```

UDP send bytes as a histogram 
```bash
bpftrace -e 'k:udp_sendmsg { @send_bytes = hist(arg2); }'
```

sum malloc() requests bytes by user stack trace (high overhead)
```bash
bpftrace -e 'u:/lib/x86_64-linux-gnu/libc-2.27.so.malloc { @[ustack(5)] = sum(arg0); }'
``` 

trace kill() signals showing sender process name, target PID and
singnal number
```bash
bpftrace -e 't:syscalls:sys_enter_kill { printf("%s -> PID %d SIG %d\n", comm, args->pid, args->sig); }'
``` 

count system calls by syscall function
```bash
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[ksym(*(kaddr("sys_call_table") + args->id * 8))]  = count(); }'
``` 

count kernel function calls starting with "attach"
```bash
bpftrace -e 'kprobe:attach* { @[probe] = count(); }'
``` 

frequency count the third argument of vfs_write() (size)
```bash
bpftrace -e 'kprobe:vfs_write { @[arg2] = count(); }'
``` 

time the kernel function vfs_read() and summarize as a histogram
```bash
bpftrace -e 'k:vfs_read { @ts[tid] = nsecs; } kr:vfs_read /@ts[tid]/ { @ = hist(nsecs - @ts[tid]); delete(@ts[tid]); }'
``` 

count context switch stack traces
```bash
bpftrace -e 't:sched:sched_switch { @[kstack, ustack, comm] = count(); }'
``` 

sample kernel-level stack at 99 Hertz, excluding idle
```bash
bpftrace -e profile:hz:99 /pid/ { @[kstack] = count(); }'
``` 