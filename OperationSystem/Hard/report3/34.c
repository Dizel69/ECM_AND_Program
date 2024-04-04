#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define IO_TIME 30
int io_op (float ratio, int exect) {
	int io = ((float) rand ( )) / RAND_MAX < ratio;
if (io)
	io = (int) trunc( ((float) rand()) / RAND_MAX * (exect - 1)) + 1;
return io;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



typedef struct job {
	int id;
	int arrival;
	int unblock;
	int exectime;
	int respons;
	int turnaround;
	float ioratio;
	struct job *next;
} job;

typedef struct spec{
	int arrival;
	int exectime;
	float ioratio;
} spec;

job * readyq = NULL;
job * blockedq = NULL;
job * doneq = NULL;

	spec specs [] = {
		{ 0, 10, 0.0 },
		{ 0, 30, 0.7 },
		{ 0, 20, 0.0 },

		{ 40, 80, 0.4 },
		{ 60, 30, 0.3 },
		{ 120, 90, 0.3 },

		{ 120, 40, 0.5 },
		{ 140, 20, 0.2 },
		{ 160, 10, 0.3 },

		{ 180, 20, 0.3 },
		{ 0, 0, 0.0 }
	};

void block(job * this) {
	job *nxt = blockedq;
	job * prev = NULL;
	while (nxt != NULL) {
		if (this-> unblock < nxt->unblock)
			break;
		else{
			prev = nxt;
			nxt = nxt->next;
		}
	}
	this->next = nxt;
	if(prev != NULL)
		prev->next = this;
	else{
		blockedq = this;
	}
	return;
}

void init(){
	int i = 0;
	while(specs[i].exectime != 0) {
		job * new = (job*) malloc(sizeof(job));
		new->id=i+1;
		new->arrival = specs[i].arrival;
		new->unblock = specs[i].arrival;
		new->exectime = specs[i].exectime;
       		new->respons = 0;
		new->ioratio = specs[i].ioratio;
		block(new);
		i++;
	}
}

void ready (job * this){
	job * nxt = readyq;
	job * prev = NULL;

	while (nxt != NULL){
		if(nxt -> exectime < this -> exectime){
			prev = nxt;
			nxt = nxt -> next;
		}
		else
		break;
	}
	this -> next = nxt;
	if (prev == NULL)
		readyq = this;
	else
		prev -> next = this;
	return;
}

void unblock (int time){
	while (blockedq != NULL && blockedq -> unblock <= time){
		job * nxt = blockedq;
		blockedq = nxt -> next;
		printf("(%4d) unblock job %2d\n", time, nxt -> id);
		ready(nxt);
	}
}

void done (job * this){
	this->next = doneq;
	doneq = this;
	return;
}

int schedule (int time, int slot){
	if (readyq != NULL){
		job * nxt = readyq;
		readyq = readyq -> next;
        if (nxt->respons == 0)
            nxt->respons = time - nxt->arrival;

       
	int io = 0;
	if (execl > 1){
		io = io_op(nxt->ioratio, exect);
	}	
	nxt->exectime -= exect;
	printf("(%4d) run job %2d for %3d ms", time, nxt->id, exect);
	if (nxt->exectime == 0){
		nxt->turnaround = time + exect - nxt->arrival;
		printf(" - done\n");
		done(nxt);
	}else
		if (io){
			ready(nxt);
			printf(" - %3d left - I/O\n", nxt->exectime);
			exect+=IO_TIME;
		} else{
			ready(nxt);
			printf(" - %3d left\n", nxt->exectime);
		}
	return execl;
}

int main(int argc, char *argv []){
	init();
	int time = 0;

    int slot = 10;
    if (argc == 2){
        slot = atoi(argv[1]);
    }


	while (blockedq != NULL || readyq != NULL){
		unblock(time);
		int tick = schedule(time, slot);
		time += tick;
	}
	printf("\n total execution time is %d\n", time);
	return 0;
}


