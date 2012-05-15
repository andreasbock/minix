/* The kernel call implemented in this file:
 *   m_type:	SYS_EBPROFILE
 *
 * The parameters for this kernel call are:
 *    m1_p1:	EBP_BUFFER1_KEY 	(shmem key for first buffer for collected data)	
 *    m1_p2:	EBP_BUFFER2_KEY 	(shmem key for second buffer for collected data)	
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
  (void)fprintf(stdout,"do_ebprofile in kernel/sysem/do_ebprofile.c\n");
  void *buffer1 = m_ptr->EBP_BUFFER1_KEY;
  void *buffer2 = m_ptr->EBP_BUFFER2_KEY;
  int bitmap = m_ptr->EBP_BITMAP;
  unsigned int *relevant = (unsigned int *) m_ptr->EBP_RELBUF;
 
  /* check if we are trying to disable profiling */ 
  if (m_ptr->EBP_BUFFER1_KEY == NULL &&
      m_ptr->EBP_BUFFER2_KEY == NULL && m_ptr->EBP_BITMAP == 0)
  {
    set_internals(m_ptr);
    return(OK);
  }
  
  /* if not, enable and set buffer & flags appropriately */

  set_internals(m_ptr);

  return(OK);
}

#endif /* EBPROFILE */
