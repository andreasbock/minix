#ifndef _LIB_EBPROF_H
#define _LIB_EBPROF_H

/* This header file defines types and structures
 *  for the ebprofile tool.
 */

#include <minix/ipc.h>
#include <minix/com.h>
#include <minix/priv.h>
#include <minix/const.h>

#if EBPROFILE

#define BUFFER_SIZE 1024 // has to match with kernel/ebprofile.h

typedef struct
{
  int time;
  int kcall;
  unsigned int p_nr;
  endpoint_t p_endpoint;
  int *params[12];
  /* from kernel/proc.c */
  unsigned cpu;
  char p_priority;
  struct priv *p_priv;
} kcall_sample;


typedef struct
{
  kcall_sample sample[BUFFER_SIZE];
  unsigned int reached;
  int lock;
} ebp_sample_buffer;

typedef struct
{
  ebp_sample_buffer *first;
  key_t first_key;
  ebp_sample_buffer *second;
  key_t second_key;
} ebp_buffers;

ebp_buffers *ebp_start (int bitmap);
void ebp_stop (void);
int ebp_get (ebp_sample_buffer *buffer);
key_t alloc_buffers (void);

#endif /* EBPROFILE */
#endif /* _LIB_EBPROF_H */
