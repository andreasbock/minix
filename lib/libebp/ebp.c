/* Library implementing functions for 
*  event-based profiling in MINIX 3. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <minix/ebp.h>
#include <minix/syslib.h>
#include <minix/callnr.h>
#include <sys/ipc.h>

#if EBPROFILE

/* Until mutexes are implemented */
#define mutex_lock() (void)0
#define mutex_unlock() (void)0

int relevant_buffer;
ebp_buffers *buffers;

/* Initializes datastructures used for profiling. */
ebp_buffers *
ebp_start (int bitmap)
{
  (void)fprintf(stdout,"LIB start1\n");
  message m;
  (void)fprintf(stdout,"LIB start101\n");
  buffers = malloc(sizeof(ebp_buffers));
  (void)fprintf(stdout,"LIB start11\n");
  buffers->first_key = alloc_buffers();
  (void)fprintf(stdout,"LIB start12\n");
  buffers->second_key = alloc_buffers();
  (void)fprintf(stdout,"LIB start13\n");
  relevant_buffer = malloc(sizeof(int));
  (void)fprintf(stdout,"LIB start2\n");

  /* Set profiling flag */
  bitmap &= 0x1;
 
  (void)fprintf(stdout,"LIB start3\n");
  /* do syscall */ 
  m.EBP_BUFFER1_KEY	= buffers->first_key;
  m.EBP_BUFFER2_KEY	= buffers->second_key;
  m.EBP_RELBUF          = relevant_buffer;
  m.EBP_BITMAP	        = bitmap;

  (void)fprintf(stdout,"LIB start4\n");
  _syscall(PM_PROC_NR, EBPROF, &m);
  (void)fprintf(stdout,"LIB start5\n");
  return buffers;
}

/*  Stops profiling. */
void
ebp_stop (void)
{
  message m;
  m.EBP_BUFFER1_KEY	= NULL;
  m.EBP_BUFFER2_KEY	= NULL;
  m.EBP_BITMAP	        = 0x0;
  m.EBP_RELBUF	        = NULL;
  free(buffers->first);
  free(buffers->second);
  free(relevant_buffer);
  _syscall(PM_PROC_NR, EBPROF, &m);
  return;
}

/* Write current profiling information to buffer. */
int
ebp_get (ebp_sample_buffer *buffer)
{ 
        unsigned int tmp, reached;
        ebp_sample_buffer *buf_ptr;

        mutex_lock();
        /* Change buffer */
        if (relevant_buffer)
        {
                relevant_buffer = 0;
                buf_ptr = &buffers->first; 
        }
        else
        {
                relevant_buffer = 1;
                buf_ptr = &buffers->second; 
        }
        reached = buf_ptr->reached;
        tmp = reached;

        (reached <= BUFFER_SIZE) ?: (reached = BUFFER_SIZE);
	memcpy(buffer, (void *)buf_ptr, sizeof(kcall_sample[reached]));

        buf_ptr->reached = 0;
        mutex_unlock();
        return tmp;
}

/* Allocates memory for double buffering */
key_t
alloc_buffers (void)
{
  int shmid;
  ebp_sample_buffer *shm, *s;
  key_t key = 1234;
  fprintf(stdout,"allocB start\n");

  /* Create segment */
  if ((shmid = shmget(key, sizeof(ebp_sample_buffer), IPC_CREAT | 0666)) < 0) {
          perror("shmget");
          exit(1);
  }

  /* Now we attach the segment to our data space. */
  if ((shm = shmat(shmid, NULL, 0)) == -1) {
          perror("shmat");
          exit(1);
  }
  else
  {
          memset (shm, '\0', sizeof (ebp_sample_buffer));
          shm->lock = 0;
          shm->reached = 0;
  }

  fprintf(stdout,"allocB start3\n");
  return shm;
}

#endif /* EBPROFILE */
