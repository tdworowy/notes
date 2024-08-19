/*
 * klib.c
 ***********************************************************************
 * This program is part of the source code released for the book
 *  "Linux Kernel Programming" 2E
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *  GitHub repository:
 *  https://github.com/PacktPublishing/Linux-Kernel-Programming_2E
 *
 ************************************************************************
 * Brief Description:
 * This kernel (module) code is meant to serve as a 'library' of sorts. Other
 * kernel modules in our codebase might wish to link into it and use it's code.
 *
 * SECURITY NOTE
 * Here we do use the %px printk format specifier in order to see the actual
 * address; this is a security info-leak! Do NOT do this in production.
 *
 * For details, please refer the book.
 */
#include "klib.h"

/*
 * Simple wrapper over the snprintf() - a bit more security-aware.
 * Checks for and warns on overflow
 */
int snprintf_lkp(char *buf, size_t maxsize, const char *fmt, ...)
{
	va_list args;
	int n;

#ifndef __KERNEL__
#include <stdarg.h>
#endif
	va_start(args, fmt);
	n = vsnprintf(buf, maxsize, fmt, args);
	va_end(args);
	if (n >= maxsize) {
#ifdef __KERNEL__
		pr_warn("snprintf(): possible overflow! (maxsize=%lu, ret=%d)\n",
			maxsize, n);
		dump_stack();
#else
		fprintf(stderr, "snprintf(): possible overflow! (maxsize=%lu, ret=%d)\n",
			maxsize, n);
#endif
	}
	return n;
}

/* minsysinfo:
 * Similar to our ch5/min_sysinfo code; it's just simpler (avoiding deps) to
 * package this code into this small 'library' of sorts rather than to use it
 * via the module stacking approach.
 *
 * A more security-aware version of the klib_sysinfo routine. We used
 * David Wheeler's flawfinder(1) tool to detect possible vulnerabilities;
 * so, we change the strlen, and replace the strncat with strlcat.
 */
void minsysinfo(void)
{
#define MSGLEN   128
        char msg[MSGLEN];

        memset(msg, 0, MSGLEN);
        snprintf_lkp(msg, 48, "%s(): minimal platform info:\nCPU: ", __func__);

        /* Strictly speaking, all this #if... is considered ugly and should be
           isolated as far as is possible */
#ifdef CONFIG_X86
#if(BITS_PER_LONG == 32)
        strlcat(msg, "x86_32, ", MSGLEN);
#else
        strlcat(msg, "x86_64, ", MSGLEN);
#endif
#endif
#ifdef CONFIG_ARM
        strlcat(msg, "ARM-32, ", MSGLEN);
#endif
#ifdef CONFIG_ARM64
        strlcat(msg, "Aarch64, ", MSGLEN);
#endif
#ifdef CONFIG_MIPS
        strlcat(msg, "MIPS, ", MSGLEN);
#endif
#ifdef CONFIG_PPC
    strlcat(msg, "PowerPC, ", MSGLEN);
#endif
#ifdef CONFIG_S390
    strlcat(msg, "IBM S390, ", MSGLEN);
#endif

#ifdef __BIG_ENDIAN
    strlcat(msg, "big-endian; ", MSGLEN);
#else
    strlcat(msg, "little-endian; ", MSGLEN);
#endif

#if(BITS_PER_LONG == 32)
    strlcat(msg, "32-bit OS.\n", MSGLEN);
#elif(BITS_PER_LONG == 64)
    strlcat(msg, "64-bit OS.\n", MSGLEN);
#endif
    pr_info("%s", msg);
}

/* 
 * show_phy_pages() - show the virtual, physical addresses and PFNs of the memory
 *            range provided on a per-page basis.
 *
 * ! NOTE   NOTE   NOTE !
 * The starting kernel address MUST be a 'linear' address, i.e., an adrress
 * within the 'lowmem' direct-mapped region of the kernel segment, else this
 * will NOT work and can possibly crash the system.
 *
 * @kaddr: the starting kernel virtual address; MUST be a 'lowmem' region addr
 * @len: length of the memory piece (bytes)
 * @contiguity_check: if True, check for physical contiguity of pages
 *
 * 'Walk' the virtually contiguous 'array' of pages one by one (i.e. page by
 * page), printing the virtual and physical address as well as the PFN- page
 * frame number. This way, we can see if the memory really is *physically*
 * contiguous or not.
 */
void show_phy_pages(void *kaddr, size_t len, bool contiguity_check)
{
	void *vaddr = kaddr;
#if(BITS_PER_LONG == 64)
	const char *hdr = "-pg#-  --------va--------   --------pa--------   ---PFN---\n";
#else             // 32-bit
	const char *hdr = "-pg#-  ----va----   --------pa--------   --PFN--\n";
#endif
	phys_addr_t pa;
	int loops = len/PAGE_SIZE, i;
	long pfn, prev_pfn = 1;

#ifdef CONFIG_X86
	if (!virt_addr_valid(vaddr)) {
		pr_info("%s(): invalid virtual address (0x%px)\n", __func__, vaddr);
		return;
	}
	/* Worry not, the ARM implementation of virt_to_phys() performs an internal
	 * validity check
	 */
#endif

	pr_info("%s():start kaddr %px, len %zu, contiguity_check is %s\n",
		       __func__, vaddr, len, contiguity_check?"on":"off");
	pr_info("%s", hdr);
	if (len % PAGE_SIZE)
		loops++;
	for (i = 0; i < loops; i++) {
		pa = virt_to_phys(vaddr+(i*PAGE_SIZE));
		pfn = PHYS_PFN(pa);

		if (!!contiguity_check) {
		/* What's with the 'if !!(<cond>) ...' ??
		 * It's a 'C' trick ensuring that the if condition always evaluates
		 * to a boolean - either 0 or 1. Cool, huh.
		 */
			if (i && (pfn != prev_pfn + 1)) {
				pr_notice(" *** physical NON-contiguity detected (i=%d) ***\n", i);
				break;
			}
		}

		/* Below we show the actual virt addr and not a hashed value by
		 * using the 0x%px format specifier instead of the %pK as we
		 * should for security */
		/* if(!(i%100)) */
#if(BITS_PER_LONG == 64)
		pr_info("%05d  0x%px   %pa   %9ld\n",
#else
		pr_info("%05d  0x%px   %pa   %7ld\n",
#endif
			i, vaddr+(i*PAGE_SIZE), &pa, pfn);
		if (!!contiguity_check)
			prev_pfn = pfn;
	}
}

/*
 * powerof - a simple 'library' function to calculate and return
 *  @base to-the-power-of @exponent
 * f.e. powerof(2, 5) returns 2^5 = 32.
 * Returns -1UL on failure.
 */
u64 powerof(int base, int exponent)
{
	u64 res = 1;

	if (base == 0)		// 0^e = 0
		return 0;
	if (base <= 0 || exponent < 0)
		return -1UL;
	if (exponent == 0)	// b^0 = 1
		return 1;
	while (exponent--)
		res *= base;
	return res;
}

/*
 * show_sizeof()
 * Simply displays the sizeof data types on the platform.
 */
void show_sizeof(void)
{
#ifndef __KERNEL__
	printf(
#else
	pr_info(
#endif
		"sizeof: (bytes)\n"
		"  char = %2zu   short int = %2zu           int = %2zu\n"
		"  long = %2zu   long long = %2zu        void * = %2zu\n"
		" float = %2zu      double = %2zu   long double = %2zu\n",
		sizeof(char), sizeof(short int), sizeof(int),
		sizeof(long), sizeof(long long), sizeof(void *),
		sizeof(float), sizeof(double), sizeof(long double));
}

/*------------ delay_sec --------------------------------------------------
 * Delays execution for @val seconds.
 * The fact is, it's unnecessary (just a demo): the kernel already has the
 * ssleep() inline function (a simple wrapper over the msleep()).
 *
 * Parameters:
 * @val : number of seconds to sleep for; if -1, sleep forever
 * MUST be called from process context.
 * (We deliberately do not inline this function; this way, we can see it's
 * entry within a kernel stack call trace).
 */
void delay_sec(long val)
{
	asm ("");    // force the compiler to not inline it!
	if (in_task()) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (val == -1)
			schedule_timeout(MAX_SCHEDULE_TIMEOUT);
		else
			schedule_timeout(val * HZ);
	}
}