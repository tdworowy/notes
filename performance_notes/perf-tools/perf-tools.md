count all kernel TCP functions
``` bash
funccount "tcp_*"
```

count all kernel VFS functions, printing the top 10 every 1 s
``` bash
funccount -t 10 -i 1 'vfs*'
```
trace the kernel function do_nanosleep() and show all child calls
``` bash
funcgraph do_nanosleep
```

trace the kernel function do_nanosleep() and show child calls
up to 3 level deep
``` bash
funcgraph -m 3 do_nanosleep
```

count all kerenl functions ending in "sleep" for PID 198
``` bash
functrace -p 198 '*sleep'
```

trace vfs_read() calls slower than 10 ms
``` bash
funcslower vfs_read 10000
```

trace do_sys_open() kernel function using a kprobe
``` bash
kprobe p:do_sys_open
```

trace the return of do_sys_open() using a kprobe
``` bash
kprobe 'r:do_sys_open $retval'
```

trace the file mode argument of do_sys_open()
``` bash
kprobe 'p:do_sys_open mode=$arg3:u16'
```

trace the file mode argument of do_sys_open()(x86_64 specific)
``` bash
kprobe 'p:do_sys_open mode=%dx:u16'
```

trace the filename argument of do_sys_open() as a string
``` bash
kprobe 'p:do_sys_open filename=+o($arg2):string'
```
trace the filename argument of do_sys_open() as a string(x86_64 specific)
``` bash
kprobe 'p:do_sys_open filename=+o(%si):string'
```
trace do_sys_open() when filename matches "stat"
``` bash
kprobe 'p:do_sys_open file=+o($arg2):string file ~"*stat"'
```

trace tcp_retransmit_skb() with kernel stact trace
``` bash
kprobe -s p:trp_retransmit_skb
```

list tracepoints
``` bash
tpoint -l
```

trace disk I/O with karnel stack traces
``` bash
tpoiny -s block:block_rq_issue
```

trace user-level readline() calls in all "bash" executables
``` bash
uprobe p:bash:readline
```

trace the return of readline() from "bash" and print its return value as a string
``` bash
uprobe 'r:bash:readline +O($retval):str'
```

trace readline() entry from /bin/bash with its entry argument(x86_64)
as as string
``` bash
uprobe 'p:bin/bash:readline prompt=+O(%di):string'
```

trace the libc gettimeofday() call for PID 1234 only
``` bash
uprobe -p 1234 p:libc:gettimeofday
```

trace the return of fopen() only when it returns NULL(and using "file" alias)
``` bash
uprobe 'r:libc:fopen file=$retval' 'file == 0'
```