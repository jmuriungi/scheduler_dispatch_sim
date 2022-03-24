#include <pthread.h>
#include <vector>
#include <string>

#include "../headers/cpu.h"

using namespace std;

#include <iostream>

#define CPU_COUNT 5
// mutex <- for interaction somewhere

// things to keep track of to be printed out later:
// throughput time
// avg turnaround times
// avg wait times
// avg response times
// avg context switching times
// processor utilization - U [%]
// speedup over multiple CPUs
int PID_ASSIGNED_NUM = 0;

struct process {
    int pid;
    std::string data = ""; // Data generated for the process to hold 
};

struct pcb {
    process* p_pointer; // Points to the process it is containing the information about
    int pid; // pid = process.get_id(); Process ID (ID of the process)
    int priority; // The process's priority (its favor in the queues)
    bool state; // True = Ready ; False = Waiting
    int cpuBurstTime; // The CPU Burst time for the process
    int accumulated_T; // turnaround time
    int accumulated_W; // wait time
    int accumulated_R; // response time
};

class processTable : protected cpu {
    public:
    /* -- attributes -- */
    vector<pcb *> processPCB; // List of Pcb pointer structs
    vector<int> pid_list; //list for PID's of the same PCB in the process PCB list
    int totalT;
    int totalW;
    int totalR;
    /* -- constructors -- */
    processTable() { // Default Constructor
        // Do default stuff here
    }
    /* -- base class functions -- */
    void addPCB(pcb* _pcb) {
        processPCB.push_back(_pcb);
        pid_list.push_back(_pcb->pid);
    }    
    // Remove / clean a process
    /* -- Getters & Setters -- */
    int getTotalT() { return totalT; } // Return the total turn around time amongst all the processes
    int getTotalW() { return totalW; } // Return the total wait time amongst all the processes  
    int getTotalR() { return totalR; } // Return the total responce time amongst all the processes
    
    int getProcessT(int _pid) { // Get the accumulated_T from the specified process which is called by the process ID 
        for(int i=0; i<pid_list.size(); i++) {
            if (pid_list.at(i) == _pid)
                return processPCB[i]->accumulated_T;
        }
    }
    
    int getProcessW(int _pid) { // Get the accumulated_W from the specified process which is called by the process ID 
        for(int i=0; i<pid_list.size(); i++) {
            if (pid_list.at(i) == _pid) 
                return processPCB[i]->accumulated_W;
        }
    }

    int getProcessR(int _pid) { // Get the accumulated_R from the specified process which is called by the process ID 
        for(int i=0; i<pid_list.size(); i++) {
            if (pid_list.at(i) == _pid) 
                return processPCB[i]->accumulated_R;
        }
    }
    /* -- printf / cout function -- */
};


class cpu {
    public:
    /* -- attributes -- */
    processTable table; // Make the process table for the CPU

    protected:
    int processCount;
    process* processes; // The dynamically allocated processes for the cpu
    pcb* pcbs; // The dynamically allocated pcbs for the cpu
    
    public:
    /* -- constructors -- */ 
    cpu() : cpu(rand() % 50 + 26) {} // default constructor for cpu 
    /*We're gonna have x ammount of processes (1-50 +25 processes), automatically
      call argumented constructor with that process count to invoke it. */
    cpu(int _processCount) : cpu(_processCount, 0) {} // single argumented constructor for cpu 

    cpu(int _processCount, int priority) { // argumented constructor for cpu 
        processCount = _processCount;
        processes = new process[processCount]; // Dynamically allocate that many process structs
        pcbs = new pcb[processCount]; // Dynamically allocate that many pcb structs

        // For each process and pcb, do the following
        /* - Create a Fstream and create a new file by name of CPU such as "core1" or something */
        for(int i=0; i<processCount; i++) {
            processes[i].pid = PID_ASSIGNED_NUM; // Set the PID of the processes, starting at 1
            PID_ASSIGNED_NUM++;
            pcbs[i].p_pointer = &processes[i]; // Set the p_pointer as a pointer to the [i] process
            pcbs[i].pid = processes[i].pid; // Set the pcb's PID to the process [i]'s PID
            pcbs[i].priority = priority; // Set the priority given, 0 be default | low, medium, high, critical
            pcbs[i].state = true; // Set the state to ready
            pcbs[i].cpuBurstTime = 0; // Set the cpu burst time to 0
            pcbs[i].accumulated_T = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_W = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_R = 0; // set the accumulated_T to 0 due to being just created
            /* Write to the cpu's file for this process on line [i]
                - Form a string with the data just made above, then place that string in the CSV file on the line [i]+1 (or 2?)
                - fstream <<pidno,<<arrivaltime,<<cputime,<<iotime,<<cputime,<<iotime,cputime,<<iotime,<<cputime,<<iotime (on the line of the pid+1 (or 2?)) [and not placing a data string]
            */
        }
        // Close the fstream

        // populate process table with process and their pcbs 
        for(int i=0; i<processCount; i++) { table.addPCB(&pcbs[i]); }
    }

    ~cpu() {
        delete processes;
        delete pcbs;
    }

    /* -- base class functions -- */
    int runproc() // runs a process
    {
        int success = 0;

        return success;
    }

    /* -- Getters & Setters -- */
    processTable getProcessTable(){ return table; }
    
    /* -- printf / cout function -- */

};

vector<int> FCFS(){

    vector<int> holder;
    //load processes into queue
    //set fixed cpu 
    //until all processes are done
    //pop a process from the top of the queue
    //update the state to ready and run the process through cpus
    //set all necessary times needed for total process
    //take process out of queue
    //get all the analytic information from cpus process table and store in holder and store in holder
    

return holder;

}

vector<int> SPT(){
     vector<int> holder;
    //load processes into queue
    //set fixed cpu
    //until all processes are done
    //use math min to get process with shortest processing 
    //update the state to ready and run the process through cpus
    //update times 
    //take process out of queue
    //get all the analytic information and store in holder
    

return holder;

}

vector<int> Robin(){
    vector<int> holder;
    //load processes into map with slot time
    //until all processes are done
    //run top process
    //if it's not completed. upddate necessary cpu time to complete and add it back to the map
    //if complete, take out of map
    //get all the analytic information and store in holder
    

return holder;

}

int main () {
    cpu virtualCPU[CPU_COUNT]; 
    // For each CPU created, assign this

    // Do the threading stuff here and queues and things


    return 0;
}