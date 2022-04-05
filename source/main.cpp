#include <pthread.h>
#include <vector>
#include <algorithm>
#include <string> 

#include "../headers/cpu.h"

// using namespace std;

using std::vector;
using std::fstream;

#include <fstream>
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
    int arrivalTime; // process's arrival time
    int ioTime; // process's i/o time
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

    vector<pcb *> getProcessPcbList() { return processPCB; } // Return the entire vector of pointer to pcb structs in the process table

    pcb* getProcessPcb(int index) { return processPCB.at(index); } // Return the pointer to that pcb at the index in the vector of process PCB's

    vector<int> getPidList() { return pid_list; } // Return the entire vector of pid's in the process table

    int getPid(int index) { return pid_list.at(index); } // Return the pointer to the pid at the index given in the vector of pid's

    /* -- printf / cout function -- */
};


class cpu {
    public:
    /* -- attributes -- */
    processTable table; // Make the process table for the CPU [[Ignore this error, it is not true]]

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
        /* - Create a Fstream and create a new file by name of CPU#_PROC# such as "cpu1proc1" or something */
        fstream cpu_data;
        cpu_data.open("data_file.csv", std::ios::in);
        //Does the file exist?
        if (cpu_data.fail()) { std::cout << "The file you are trying to open DNE.\n"; exit(1); } //Verify file actually opened

        cpu_data << "PID # | ARRIVAL TIME | CPU TIME | IO TIME | (Repeating on until process ends)\n"; // Write the code for the values being placed into the table
        // Possibly comment out the line above to make code easier, and have this here instead as a key to refer too.
        
        for(int i=0; i<processCount; i++) {
            processes[i].pid = PID_ASSIGNED_NUM; // Set the PID of the processes, starting at 1
            PID_ASSIGNED_NUM++;
            pcbs[i].p_pointer = &processes[i]; // Set the p_pointer as a pointer to the [i] process
            pcbs[i].pid = processes[i].pid; // Set the pcb's PID to the process [i]'s PID
            pcbs[i].arrivalTime = 0;    // Set arrivalTime to 0 initially
            pcbs[i].ioTime = 0; // Set ioTime to 0 initially
            pcbs[i].priority = priority; // Set the priority given, 0 be default | low, medium, high, critical
            pcbs[i].state = true; // Set the state to ready
            pcbs[i].cpuBurstTime = 0; // Set the cpu burst time to 0
            pcbs[i].accumulated_T = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_W = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_R = 0; // set the accumulated_T to 0 due to being just created
            // Write to the cpu's file for this process on line [i]
            //- Form a string with the data just made above, then place that string in the CSV file on the line [i]+1 (or 2?)
            // PID #, Arrival t, CPU t, IO t, CPU t, IO t, CPU t 
            cpu_data <<pcbs[i].pid <<","<< pcbs[i].arrivalTime <<","<< pcbs[i].cpuBurstTime <<","<< pcbs[i].ioTime;
        }
        // Close the fstream
        cpu_data.close(); 

        // populate process table with process and their pcbs 
        for(int i=0; i<processCount; i++) { table.addPCB(&pcbs[i]); }
    }

    ~cpu() {
        delete processes;
        delete pcbs;
    }


    /* -- base class functions -- */
    int runproc(pcb* _selectedProcess, int runtime) // runs a process at specified 
    {   
        int temp_ioTime = rand() % 50;  // simulate random i/o time
        int temp_cpu_burst = rand() % 20;   // simulate random cpu time
        _selectedProcess->ioTime = temp_ioTime; // assign simulated i/o time to selected process
        _selectedProcess->cpuBurstTime = temp_cpu_burst;    // assign simulated cpu time to selected process
        _selectedProcess->accumulated_T = (_selectedProcess->accumulated_W + runtime);  // turnaround time = wait time + execution time
        
        /*
        process* p_pointer; // Points to the process it is containing the information about
        int pid; // pid = process.get_id(); Process ID (ID of the process)
        int arrivalTime; // process's arrival time
        int ioTime; // process's i/o time
        int priority; // The process's priority (its favor in the queues)
        bool state; // True = Ready ; False = Waiting
        int cpuBurstTime; // The CPU Burst time for the process
        int accumulated_T; // turnaround time
        int accumulated_W; // wait time
        int accumulated_R; // response time
    */
        int success = 0;    // maybe should be bool?
        // pick a random process from process table
        // update burst time, iotime, etc
        

        // Update Information

        return success;
    }

    /* -- Getters & Setters -- */
    processTable getProcessTable(){ return table; }
    
    /* -- printf / cout function -- */
    void print_data() {

        //return success;
    }
};

/*
    present.table.getProcessPcbList() <- Return the entire vector of pointer to pcb structs in the process table
    present.table.getProcessPcb(int index) <- Return the pointer to that pcb at the index in the vector of process PCB's
    present.table.getPidList() <- Return the entire vector of pid's in the process table
    present.table.getPid(int index) <- Return the pointer to the pid at the index given in the vector of pid's
*/
vector<int> FCFS(cpu present){

    vector<int> holder;
    //grab processes within the present CPU
    
    //set fixed cpu 
    //until all processes are done
     
    for (int i = 0; i < present.table.getPidList().size(); i++){
        //run the next process
        present.runproc(present.table.getProcessPcb(i));
        //update process state
    }
 
    //pop a process from the top of the queue
    //update the state to ready and run the process through cpus
    //set all necessary times needed for total process
    //take process out of queue
    //get all the analytic information from cpus process table and store in holder and store in holder
    

return holder;

}

vector<int> SPT(cpu present){

     vector<int> holder;
    
      for (int i = 0; i < present.table.getPidList().size(); i++){
          int x =0;
        //find the minimum process time
        //clear holder
        holder.clear();
        
        for (int j =0; i <present.table.getPidList().size(); i++){
        //fill holder with the process times
        holder[i]= present.table.getProcessPcb(i).ioTime;
        }
        //get minimum element from holder
        x = min_element(holder.begin(), holder.end());
        //set that index to a large time than possible 
        holder[x] = 1000;

        present.runproc(present.table.getProcessPcb(i));
        //update process state
        
        
        
    }
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

    // from ready queue to execution
    // void select_process()
    // {    // for each process in pcb in ready queue, determine if it should be executed
    //      while there's a process in ready queue/while ready queue not empty
    //      {   // use accumulated wait time for FCFS
    //          if(pcb[i].accummulated_W > 50)
    //          {
    //              execute(pcb[i]);
    //          }
                //service time required by process to execute
    //          else if(s < 5)
    //          {
    //              execute(pcb[i]);
    //          }
    //          else
    //          {just pick random process in ready queue}
    //
    //      }
    // }


    return 0;
}