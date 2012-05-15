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
  message m;
  buffers = malloc(sizeof(ebp_buffers));
  buffers->first  = alloc_buffers();
  buffers->second = alloc_buffers();
  relevant_buffer = malloc(sizeof(int));

  /* Set profiling flag */
  bitmap &= 0x1;
 
  (void)fprintf(stdout,"LIB start3\n");
  /* do syscall */ 
  m.EBP_BUFFER1_KEY	= buffers->first->key;
  m.EBP_BUFFER2_KEY	= buffers->second->key;
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
ebp_sample_buffer *
alloc_buffers (void)
{
  int shmid;
  ebp_sample_buffer *shm, *s;
  key_t *key;
  fprintf(stdout,"allocB start\n");

  *key = 1234;
  /* Create segment */
  if ((shmid = shmget(key, sizeof(ebp_sample_buffer), IPC_CREAT | 0666)) < 0) {
          perror("Could not get shared memory segment");
          exit(1);
  }

  /* Attach the segment to our address space. */
  if ((shm = shmat(shmid, NULL, 0)) == -1) {
          perror("Could not attach shared memory segment.");
          exit(1);
  }
  else
  {
          shm->key = key;
          shm->lock = 0;
          shm->reached = 0;
          memset (shm+3, '\0', sizeof (ebp_sample_buffer));
  }
  fprintf(stdout,"allocB start3\n");
  return shm;
}

#endif /* EBPROFILE */
