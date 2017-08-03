//***********************************************
//Raphael Montalvo
//U46380534
//
//assignment1.c
//
// Create a progam that uses the fork function
// and manipulates a single global variable
//***********************************************

// declare all libraries being used in this program
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* change the key number */
#define SHMKEY ((key_t) 18548)

// creates te struct used that will have the shared memory
typedef struct
{
  int value;
} shared_mem;

shared_mem *total;




/*----------------------------------------------------------------------*
 * This function increases the value of shared variable "total"
 *  by one all the way to 100000
 *----------------------------------------------------------------------*/

void process1 ()
{
  int k = 0;
 
  while (k < 100000)
    {
      k++;
      total->value = total->value + 1;
    }
  printf ("From process1 total = %d\n", total->value);
}


/*----------------------------------------------------------------------*
 * This function increases the vlaue of shared memory variable "total"
 *  by one all the way to 200000
 *----------------------------------------------------------------------*/

void process2 ()
{
  int k = 0;

  while (k < 200000)
    {
      k++;
      
     total->value = total->value + 1;
    }
  
  printf ("From process2 total = %d\n", total->value);
  
}

/*----------------------------------------------------------------------*
 * This function increases the vlaue of shared memory variable "total"
 *  by one all the way to 300000
 *----------------------------------------------------------------------*/

void process3 ()
{
  int k = 0;

  while (k < 300000)
    {
      k++;
      
     total->value = total->value + 1;
    }
  
  printf ("From process3 total = %d\n", total->value);
  
}


/*----------------------------------------------------------------------*
 * MAIN()
 *----------------------------------------------------------------------*/

main()
{
// declaration of all variables used in the program
  int   shmid;
  int   pid1; 
  int   pid2;  
  int   pid3;
  int   ID;
  int	status;

  char *shmadd;
  shmadd = (char *) 0;

/* Create and connect to a shared memory segmentt*/

  if ((shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
    {
      perror ("shmget");
      exit (1);
    }

 
 if ((total = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) -1)
    {
      perror ("shmat");
      exit (0);
    }
  
  // initialize the shared variable value to 0
  total->value = 0;

  if ((pid1 = fork()) == 0) // create a child from parent and check to see if its the first child
    process1();// child executes process 1

  if ((pid1 != 0) && (pid2 = fork()) == 0)// create a second child from parent and check to see if its the second child
    process2();// second child executes process 2
	
	if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0)// create a third child from parent and check to see if its the third child
    process3();// third child executes process 3

		while ((ID = wait(&status))!= -1)// loops until a child is finished with its respective process then prints the pid
		{
			if (ID == pid1)// checks if the first process has finished
			{
				printf("Child 1 ID is: %d \n",ID);// prints PID of first child
			}
			if (ID == pid2)// checks if the second process has finished
			{
				printf("Child 2 ID is: %d \n",ID);// prints PID of the second child
			}
			if (ID == pid3)// checks if the third process has finished
			{
				printf("Child 3 ID is: %d \n",ID);// prints PID of third child
			}
		}
  if ((pid1 != 0) && (pid2 != 0) &&(pid3 != 0)) // check to see if parent
    {
      if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)// checks to if it can clear memory and does so if possible
	{
	  perror ("shmctl");// prints error message if memory could not be printed
	  exit (-1);// exits program due to error
	}
	

      printf ("\t\t  End of Program.\n");// print the final line of the program
    }
  
} 

/***** Note:  loop for parent to wait for child processes to finish and print ID of each child*****/

