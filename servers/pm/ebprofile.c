/* This file implements the entry point for event-based profiling.
 *
 * The entry points in this file are:
 *   do_ebprofile:   start/stop event-based profiling
 */

#include <minix/config.h>
#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include "mproc.h"
#include "param.h"
#include <sys/shm.h>
#include <minix/ebp.h>

/*===========================================================================*
 *				do_ebprofile				     *
 *===========================================================================*/
PUBLIC int do_ebprofile(void)
{
#if EBPROFILE
        int shmid1, shmid2;
        int *shm1, shm2;
        if ((shmid1 = shmget(m_in.EBP_BUFFER1_KEY, sizeof(ebp_sample_buffer), 0666)) < 0) {
                 perror("shmget fail buffer1");
                 exit(1);
        }
        if ((shmid2 = shmget(m_in.EBP_BUFFER2_KEY, sizeof(ebp_sample_buffer), 0666)) < 0) {
                 perror("shmget fail buffer2");
                 exit(1);
        }
        
        if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1) {
                 perror("shmat1 fail");
                 exit(1);
        }
        if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
                 perror("shmat2 fail");
                 exit(1);
        }

        m_in.EBP_BUFFER1_KEY = (char *)shm1;
        m_in.EBP_BUFFER2_KEY = (char *)shm2;

	return sys_ebprof(m_in.EBP_BUFFER1_KEY, m_in.EBP_BUFFER2_KEY, m_in.EBP_BITMAP, m_in.EBP_RELBUF);
#else
	return ENOSYS;
#endif 
}
