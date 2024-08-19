/* 
 * klib.h
 * Our klib 'library' header.
 ***********************************************************************
 * This program is part of the source code released for the book
 *  "Linux Kernel Programming" 2E
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *  GitHub repository:
 *  https://github.com/PacktPublishing/Linux-Kernel-Programming_2E
 ***********************************************************************
 * This kernel (module) header code is meant to serve as a 'library' of sorts.
 * Other kernel modules in our codebase might wish to link into it and use
 * it's code.
 *
 * For details, please refer the book.
 */
#ifndef __KLIB_LKP_H__
#define __KLIB_LKP_H__

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>		/* virt_to_phys(), phys_to_virt(), ... */

int snprintf_lkp(char *buf, size_t maxsize, const char *fmt, ...);
void minsysinfo(void);
u64 powerof(int base, int exponent);
void show_phy_pages(void *kaddr, size_t len, bool contiguity_check);
void show_sizeof(void);
void delay_sec(long);

#endif