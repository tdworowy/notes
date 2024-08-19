count VFS kernel calls
```bash
funcgraph 'vfs_*'
```

count TCP kernel calls
```bash
funccount 'tcp_*'
```

count TCP send calls per second
```bash
funccount -i 1 'tcp_send*'
```

show the rate of block I/O evenst per second
```bash
funccount -i 1 't:block:*'
```

show the rate fo libc getaddrinfo() (name resolution) per second
```bash
funccount -i 1 'c:getaddrinfo'
```

count stack traces that created block I/O
```bash
stackcount t:block:block_rq_insert
```

count stact traces that led to sending IP packets
with responsible IPD
```bash
stackcount -P ip_output
```

count stack traces that led to the thread blocking and moving off-CPU
```bash
stackcount t:sched:sched_swtich
```

trace the kernel do_sys_open() function with the filename
```bash
trace 'do_sys_open "%sm arg2'
```

trace the return of kernel function do_sys_open()
```bash
trace 'r::do_sys_open "ret: %d", retval'
```

trace the kernel function do_nanosleep()
with mode nad user-level stacks
```bash
trace -U 'do_nanosleep "mode: %d", arg2'
```

trace authentication requests via the pam library
```bash
trace 'pam:pam_start "%s: %s", arg1, arg2'
```

summarize VFS reads by return value (size or error)
```bash
argdist -H 'r::vfs_read()'
```

summarize libc read() by return value (size on error) on PID 1005
```bash
argdist -p 1005 -H 'r::vfs_read()'
```

count syscalls by syscall ID
```bash
argdist -C 't:raw_syscalls:sys_enter():int:args->id'
```

summarize the kernel function tcp_sendmsg() size argument using counts
```bash
argdist -C 'p::tcp_sendmsg(struct sock *sk, struct msghdr *msg, size_t size):u32:size'
```
summarize the kernel function tcp_sendmsg() as power-of-two histogram
```bash
argdist -H 'p::tcp_sendmsg(struct sock *sk, struct msghdr *msg, size_t size):u32:size'
```

count the libc write() call for PID 181 by file descriptor

```bash
argdist -p 181 -C 'p:c:write(int fd):int:fd'
```

summarize reads by process where latency was >100 μs
```bash
argdist -C 'r::__vfs_read():u32:$PID:$latency > 100000'
```