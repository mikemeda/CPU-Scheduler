#include <iostream>
using namespace std;

struct process{          //struct to keep status of each process
int nice;    //nice value
int arrt;    //process arrival time
int cpu;     //number of cpu burst
int* cpub=new int[cpu];  //array to keep the cpu bursts
int* iobur= new int[cpu];  //array to keep I/O bursts
bool stat=false; //has it joined the ready que
int priority; // the processes priority
int timeSlice;  //the process' time slice
int pid;   //the process id
int leftover;  //leftover time if preempted or enounters IO
int iotot;
int cputot;
int completingtime;
};

void InPrior(process* , int );
void timeS(process*, int);
void prior(process*, int );
int sleect(process*, int);
int runproc(process*, int);
void deleteelement(process*,int,int);
int checkio(process*);
int iotot;
int cputot;

int main(){
process* arr=new process[5];  //arrau of process 
int pcnt=0; 
int enter=0;
int clock=0;
bool preempt=false;
	int x;
	while(cin>>x){
	arr[pcnt].nice=x;   //input the process nice value
	cin>>x;
	arr[pcnt].arrt=x;  //input arrival time
	cin>>x;
	arr[pcnt].cpu=x;  //input #of cpu burst
	int i=0;
    	int j=0;
    	int y;
    if(x==1){
        cin>>y; 
        arr[pcnt].cpub[0]=y; 
        arr[pcnt].cputot=y; 
        arr[pcnt].iotot=0;}   //input the cpu and I/O bursts
    else if(x==2){
        cin>>y; 
        arr[pcnt].cpub[0]=y; 
        arr[pcnt].cputot=y;
        cin>>y; 
        arr[pcnt].iobur[0]=y;  
        arr[pcnt].iotot=y;
        cin>>y; 
        arr[pcnt].cpub[1]=y; 
        arr[pcnt].cputot+=y;
			}
    else if(x>2){
    	int z=x+(x-1);
    while(i+j<z){
    	cin>>y;
    	if(i==j){
            arr[pcnt].cpub[i]=y;
             i++;}
    	else if(i>j){
            arr[pcnt].iobur[j]=y;
             j++;}
    	}}
    	arr[pcnt].pid=pcnt;
    	enter++;
    pcnt++;
}
bool run=true;  //flag for while loop
int isrunning=0; //which elemnt of array us using cpu
int n=pcnt;  //total number if process entered
int expir=-1;  //# of elemnents in expired array
int act=-1;   //active array elements
int total=0;  //# of elements that joined the ready que
int iocntr=-1;  //io que counter
bool cpufree=true;  //flag to indecate if cpu is free
process* active=new process[n];
process* expired=new process[n];
process* IOque=new process[n];
process* finshed=new process[n];
int fcntr=0;  //finshed process counter
		int running=-1;  //the number of ticks current process is running for

		while(run==true){
			int l=-1;
		 	int c=-1;
		 if(total<=pcnt){  
	for(int i=0; i<pcnt; i++){
	if(arr[i].arrt==clock && arr[i].stat==false){
		act++;
	arr[i].stat=true;
	active[act]=arr[i]; //move a proc to active array
	InPrior(active,act); //calculate it's initial priority and time slice
	timeS(active,act);
	cout<<"["<<clock<<"]"<<"<"<<active[act].pid<<"> joins the ready que (priority "<<active[act].priority<<" timeslice "<<active[act].timeSlice<<")"<<endl;

	preempt=true;  //when new proc enters active array, this flag will indecate to recheck the priority
		total++;
		}}}

		if(cpufree==true && act>-1){  //if cpu is free and there are elemnts in active que

			isrunning=sleect(active,act); //select a process to run
			if(active[isrunning].leftover>0){cout<<"leftover is coming"<<endl; running=active[isrunning].leftover;}
			else {
				running=active[isrunning].timeSlice;
			} 
			cpufree=false;  //set cpu to taken
			cout<<"["<<clock<<"]<"<<active[isrunning].pid<<"> gets the cpu "<<endl;
		}
		else if(preempt==true && running>0 &&cpufree==false){ //if cpu is taken and permpt flag is on
			int temp=isrunning;  //save the currently running process  
			isrunning=sleect(active,act);  //check if the new proc has lower priorite
			if(isrunning==c){preempt=false;} //if not continue
			else{ cout<<"<"<<clock<<">"<<"<"<<active[temp].pid<<"> has been preempted by <"<<active[isrunning].pid<<">"<<endl;
				active[c].leftover=running;  //is prempted save the remaining timeslice 
				running=active[isrunning].timeSlice; //get the new proc's timeslice
				preempt=false; //set preempt flag false
				cpufree=false; //indecate that cpu is taken
				cout<<"["<<clock<<"]<"<<active[isrunning].pid<<"> gets the cpu "<<endl;
				
			}		
		}
		

		if(cpufree==false && running>0){  //if cpu is tken and running is set

			int z=runproc(active,isrunning); //run the isrunning's cpu burst
						running--;  //decrement running
			if(active[isrunning].cpu==1 && active[isrunning].cpub[0]==0){ cout<<"@@@@@@@@@@@@ths process is complete"<<endl;
			finshed[fcntr]=active[running]; //if the function return o the proc has finshed all cpu bursts
			fcntr++; //move the completed proc to finshed que
			deleteelement(active,isrunning,act); //and remove it forn active
			cpufree=true;
			act--;

		}
			if(z==0){cout<<"["<<clock<<"]<"<<active[isrunning].pid<<"> has completed and moved to finshed queue"<<endl;
			finshed[fcntr]=active[running]; //if the function return o the proc has finshed all cpu bursts
			fcntr++; //move the completed proc to finshed que
			deleteelement(active,isrunning,act); //and remove it forn active
			cpufree=true;
			act--;
			//cout<<"act is now "<<act<<endl;

			}


			if(z==1){  //if function return 1, it has completed one element of the cpu burst array;
				cout<<"["<<clock<<"]<"<<active[isrunning].pid<<"> has completed current cpu burst and moves to I/O queue"<<endl;
				iocntr++;
				prior(active,isrunning);//calculate priority and timeslice and move it to the io que
				active[isrunning].leftover=running;
				IOque[iocntr]=active[isrunning];
				deleteelement(active,isrunning,act); //remove the element from active
				act--;
				cpufree=true;
			}
			if(running==0){ //if timeslice runs out
				expir++;  //calculate timeslice and priority and move it to ecpired array
				prior(active,isrunning);
				timeS(active,isrunning);
		cout<<"["<<clock<<"]"<<"<"<<active[isrunning].pid<<"> joins the expired queue (priority "<<active[isrunning].priority<<" timeslice "<<active[isrunning].timeSlice<<")"<<endl;
				expired[expir]=active[isrunning];
				//cout<<"the "<<expir<<" elemenet of Expired array is "<<expired[expir].pid<<endl;
				deleteelement(active,isrunning,act);
				act--;
				//cout<<"act after deletion"<<endl;
				//for(int i=0; i<=act; i++){
				//cout<<"process "<<active[i].pid<<endl;
				//		}
				cpufree=true;
		}}
		if(act==-1 && expir>-1){ //if active is empity but expired is not swithc their elemnts
			//cout<<"expired que has "<<expir+1<<" elements"<<endl;
			//for(int i=0; i<=expir; i++){
			//	cout<<"process "<<expired[i].pid<<endl;
			//}
			for(int i=0; i<=expir; i++){
			active[i]=expired[i];}
			act=expir;
			expir=-1;
			cout<<"****  queue swap"<<endl;
			//for(int i=0; i<=act; i++){
			//	cout<<"process "<<active[i].pid<<endl;
			//			}
		}
		if(iocntr>-1){ //if there are elemnts in io que
			int k=checkio(IOque);  //decrement 1 form io of the first elemnt ot join the que
			if(k==0){
			cout<<"["<<clock<<"]<"<<IOque[iocntr].pid<<"> has completed current I/O burst and moves to active queue"<<endl;
			act++;
			prior(IOque,0); //if the function return 0; it menas it has completed all io bursts
			timeS(IOque,0);  //calculate priority and timeslice and move the proc to active que
			active[act]=IOque[0];
			for(int i=0; i<=act; i++){
				cout<<"process "<<active[i].pid<<" priority "<<active[i].priority<<" timeslice "<<active[i].timeSlice<<")"<<endl;
						}
			deleteelement(IOque,0,iocntr); //remov the element form the io que
			iocntr--;
			}
		}
		
		clock++;   //inclrement the clock
			if(fcntr==pcnt ||clock==6000){run=false;}  //if all proc join the finshed que stop the while loop
		}
		for(int i=0; i<fcntr; i++){   //display the final ouput
			cout<<"************************************* overall performance cotput"<<endl;
			cout<<"1: Turnaround time "<<endl;
			cout<<"2: Total CPU time "<<endl;
			cout<<"3: Waiting Time "<<endl;
			cout<<"4: Percentage of cpu uitalization "<<endl;
		}
	return 0;

}

void InPrior(process* arr, int x){  //claculate the initial priority
int y=(int)(((arr[x].nice+20)/39.0)*30+0.5)+105;
arr[x].priority=y;
}

void prior(process* arr, int x){  //calcualte the priority

	int bonus;
	int y=arr[x].cpu;
	int totalIO=0;
	int totalCPU=0;
	for(int i=0; i<y; i++){
		totalCPU=totalCPU+arr[i].cpub[i];
	}
	for(int i=0; i<y-1; i++){
		totalIO+=arr[i].iobur[i];
	}
	if(totalCPU<totalIO){
		bonus=(int)(((1-(totalCPU/(double)totalIO))*-5)-0.5);
	}
	else
		bonus=(int)(((1-(totalIO/(double)totalCPU))*5)+0.5);
	arr[x].priority+=bonus;
}

void timeS(process* arr, int x){  //calculate the time slice
int time=(int)((1-(arr[x].priority/150.0))*395+0.5)+5;
arr[x].timeSlice=time;
}

int checkio(process* arr) {  //this function will decrease the current io burst of all process in io que
int element=0;
	arr[0].iobur[element]-=1;  //decrement 1 form the element of the process' io burst
	if(arr[0].iobur[element]==0)return 0;
	else {return 1;}
	}

int sleect(process* arr, int x ){ //this function will find the proc with the lowest priorite and return where it found it
	int index=0;
	if(x==1){index=0;}
	else{
	int lowest=arr[0].priority;
	for(int i=1; i<x; i++){
		if(arr[i].priority<lowest){index=i; lowest=arr[index].priority;}
	}}
	return index;
}

void deleteelement(process* arr,int y,int x){ //this function delete que elemnts by moving themo over one to the left and decremwnting total que size
	for(int i=y; i<=x; i++){
		arr[i]=arr[i+1];
	}
}

int runproc(process*arr, int index){  //this function will decrement forn proc's cpu burst.
	bool found=false;
	int element=0;
	int x=arr[index].cpu;
	if(x==1){element=0;}
	else{ if(arr[index].cpub[element]<=0) element++; //if proc has multiple burst it will check each if they are 0, if so move to the next burst
	} //if not decrement it. when it reaches the last burst and finshes it, it return 0
	arr[index].cpub[element]--;

	if(element==(x-1) && arr[index].cpub[element]==0) return 0;
	else if(arr[index].cpub[element]==0) {return 1;}
	else {return 2;}
	}