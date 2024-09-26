```bash
supo apt install cscope
sudo apt install universal-ctags
ctags -R
```

building kernel
```bash
- download from https://kernel.org/
- extract
- export LKP_KSRC=~/linux-6.1.93
```

default configuration:
```bash
- make mrproper <- clean evrything
- make defconfig <- generate new default config
use existing config
- lsmod > /tmp/lsmod.now
- cd ${LSK_KSRC}
- make LSMOD=/tmp/lsmod.now localmodconfig
```

```bash
make menuconfig <- build UI program to tune config
scripts/diffconfig .config.old .config <- get config last change

scripts/config <- change config in scriptable way
```

build:
```bash
- make -j4 (from root dir)
- make kernelrelease kernelversion image_name (from root dir) 
```

install modules:
```bash
- sudo make modules_install
```

generate initramfs:
```bash
- sudo make install
```

GRUB customization
to alweys show GRUP prompt
```bash
- /etc/default/grub
- GRUB_HIDDEN_TIMEOUT_QUIET=false 
- GRUB_TIMEOUT_STYLE=menu
- sudo update-grub
```

load helloworld module
```bash
cd helloworld_lkm
make
sudo insmod ./helloworld_lkm.ko
```
```bash
sudo dmesg <- print kernel log buffer   
lsmod <- list loaded kernel modules
sudo rmmod <name> <- unload kernel module

journalctl -b -k --no-pager | tail -n2 <- example of using journalctl to get kernel logs

sysctl -w kernel.printk="8 4 1 7" <- redirect all printk levels to console (exclude debug level)
sudo bash -c "echo Y > /sys/module/printk/parameters/ignore_loglevel" <- include debug level
```

```bash
/proc/dynamic_debug/control
/sys/kernel/debug/dynamic_debug/control
sudo bash -c "echo -n 'file *usb +p' > /sys/kernel/debug/dynamic_debug/control" <- nable all debug messages in all files where pathname includes string "usb" (turn it ff with -p) 
sudo bash -c "echo -n 'file *ip* +p' > /sys/kernel/debug/dynamic_debug/control"
 ```

```bash
modinfo -p <path to .ko file> <- display kernel module parameters
ls /sys/module/<module name>/parameters <- each file corresponds to a parameter
 ```

get process kernel stack
```bash
sudo cat /proc/<pid>/stack
```
 
get process user stack
```bash
sudo gdb \
 -ex "set pagination 0" \
 -ex "thread apply all bt" \
 --batch -p <pid>
```
 
