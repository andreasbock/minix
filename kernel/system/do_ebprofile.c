/* The kernel call implemented in this file:
 *   m_type:	SYS_EBPROFILE
 *
 * The parameters for this kernel call are:
 *    m1_i1:	EBP_BUFFER1 	(first buffer for collected data)	
 *    m1_i2:	EBP_BUFFER2 	(second buffer for collected data)	
 *    m1_i3: 	EB_BITMAP	(bitmap specifying what to profile)
 */

#include "kernel/system.h"
#include <unistd.h>
#include "ebprofile.h"
#include <stdio.h>

#if EBPROFILE


/*===========================================================================*
 *				do_ebprofile				     *
 *===========================================================================*/
int do_ebprofile(struct proc * caller, message * m_ptr)
{
/* Handle sys_ebprofile.
*/
  printf("do_ebprofile in kernel/sysem/do_ebprofile.c\n");
  void *buffer1 = m_ptr->EBP_BUFFER1;
  void *buffer2 = m_ptr->EBP_BUFFER2;
  int bitmap = m_ptr->EBP_BITMAP;
  unsigned int *relevant = (unsigned int *) m_ptr->EBP_RELBUF;
 
  /* check if we are trying to disable profiling */ 
  if (m_ptr->EBP_BUFFER1 == NULL &&
      m_ptr->EBP_BUFFER1 == NULL && m_ptr->EBP_BITMAP == 0)
  {
    printf("turning off ebprofiling\n");
    set_internals(m_ptr);
    return(OK);
  }
  
  /* if not, enable and set buffer & flags appropriately */

  printf("setting internals for ebprofiling\n");
  set_internals(m_ptr);

  printf("returnings from ebprofiling in sysexm/do_ebprofilesd\n");
  return(OK);
}

#endif /* EBPROFILE */
