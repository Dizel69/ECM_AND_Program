#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



typedef struct job {
	int id;
	int arrival;
	int unblock;
	int exectime;
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



	//while (nxt != NULL){
	//	prev = nxt;
	//	nxt = nxt -> next;
	//}
	//this -> next = nxt;
	//if (prev == NULL)
	//	readyq = this;
	//else
	//	prev -> next = this;
	//return;
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

int schedule (int time){
	if (readyq != NULL){

        //job * shortest = readyq;
        //job * left = NULL;
        //job * temp = readyq;
        //while(temp->next != NULL){
        //    if(temp->next -> exectime < shortest -> exectime){
        //        shortest = temp->next;
        //        left = temp;
        //    }
        //    temp = temp -> next;
        //}

		//job * nxt = shortest;
        //if(left == NULL)
        //    {readyq = readyq->next;}
        //else
        //    {left -> next = shortest -> next;}
        
        //int exect = nxt -> exectime;
        //nxt -> exectime = 0;
        //printf("(%4d) run job %2d for %3d ms\n", time, nxt -> id, exect);
		//done(nxt);
		//return exect;


		job * nxt = readyq;
		readyq = readyq -> next;
		int exect = nxt -> exectime;
		nxt -> exectime = 0;
		printf("(%4d) run job %2d for %3d ms\n", time, nxt -> id, exect);
		done(nxt);
		return exect;
	}
	else{
		return 1;
	}
}

int main () {
	init();
	int time = 0;
	while (blockedq != NULL || readyq != NULL){
		unblock(time);
		int tick = schedule(time);
		time += tick;
	}
	printf("\n total execution time is %d\n", time);
	return 0;
}


