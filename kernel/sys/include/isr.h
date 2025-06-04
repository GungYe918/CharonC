#pragma once
#include <stdint.h>
#include <vt.h>
#include <subr_printk.h>

struct isr_frame;
typedef struct isr_frame isr_frame_t;


void isr_handler_c(isr_frame_t* frame);