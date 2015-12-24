#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_TAG	1
#define DATA_SIZE_TAG 2

#define BUF_SIZE 1024

FILE *fp;
long long mFileCharPointer=0;
long long sum=0;

int buf_read(char * buf, const char * end)
{
	fp = fopen("10001.tst", "r");
    int count;
	fseek( fp, mFileCharPointer, SEEK_SET );
    count = fread(buf, 1, BUF_SIZE, fp);
	mFileCharPointer=mFileCharPointer+count;

    if (count > 0 && buf[count - 1] == '\n')
	{
        --count;
    }
	fclose(fp);
    return count;
}

int main (int argc , char **argv)
{
	int myid, numprocs;
	int read=0;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	if (numprocs == 1)
	{
		/* trivial single CPU case */

		int c;
		fp = fopen("10001.tst", "r");
		
		do 
		{
		  c = (int)fgetc(fp);
		  c=(int)c-48;
		  if(c>=0)
		  {
			sum=sum+c;
		  }
		} while(c != EOF && c!=-49);

		fclose(fp);

		long long total=0;
		int flag=0;

		while(flag==0)
		{
			total=0;
			while(sum/10!=0)
			{
				total=total+sum%10;
				sum=sum/10;
			}
			total=total+sum%10;

			if((total/10)==0) 
			{
				flag=1;
			}
			else 
			{
				sum=total;
			}
		}

		printf("Trivial single CPU case: result is = %lld",total);
	}
	else
	{
		if(myid==0)
		{
			//Master

			/* read a block of chars to buf */
			read = buf_read(buf, buf + BUF_SIZE);

			/* initial allocation */
			int allocatedJobs = 1;
			
			int workerID;
			
			while (read)
			{
				workerID=allocatedJobs%(numprocs-1)+1;

				/* first sending data size, then data */
				MPI_Send(&read, 1, MPI_INT, workerID, DATA_SIZE_TAG, MPI_COMM_WORLD);
				MPI_Send(&buf, read, MPI_INT, workerID, DATA_TAG, MPI_COMM_WORLD);
					
				allocatedJobs++;

				/* read a block of chars to buf */
				read = buf_read(buf, buf + BUF_SIZE);

			}

			read=0;
			for(int i=1;i<numprocs;i++)
			{
				MPI_Send(&read, 1, MPI_INT, i, DATA_SIZE_TAG, MPI_COMM_WORLD);
			}

			allocatedJobs--;

			/* while there is any job to be allocated or received */
			while(allocatedJobs)
			{
				 long long value=0;
				 int size=1;

				 int workerProcessID=allocatedJobs%(numprocs-1)+1;

				 /*after sending jobs, we open a blocking socket and wait for return*/
				 MPI_Recv(&value, 1, MPI_INT, workerProcessID, DATA_TAG, MPI_COMM_WORLD, &status);
				 sum=sum+value;
				 allocatedJobs--;		
			}
			//collapse by the master processor
			long long total=0;
			int flag=0;

			while(flag==0)
			{
				total=0;
				while(sum/10!=0)
				{
					total=total+sum%10;
					sum=sum/10;
				}
				total=total+sum%10;

				if((total/10)==0) 
				{
					flag=1;
				}
				else 
				{
					sum=total;
				}
			}
			printf("Master, result is = %lld",total);
		}
		else
		{
			while(true)
			{
				//workers
				int data_size =0;
				MPI_Recv(&data_size, 1, MPI_INT, 0, DATA_SIZE_TAG, MPI_COMM_WORLD, &status);

				if(data_size>0)
				{
					MPI_Recv(&buf, data_size, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD, &status);
					long long temp_sum=0;

					for(int j=0;j<data_size;j++)
					{
						temp_sum=temp_sum+buf[j]-48;
					}

					MPI_Send(&temp_sum, 1, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD);
				}
				else
				{
					break;
				}
			}		
		}
	}
	MPI_Finalize();
	//while(1);
	return 0;
}

