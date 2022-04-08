// OS_dispatcher_simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>

#define CPU_COUNT 5                 // implement five cpus
#define RR_Quantum 15               // time taken before process is interrupted by Round Robin scheduler
#define TOTAL_RUNTIME 2000000000    // time available to processes to run, decremented as processes run on cpus

int PID_ASSIGNED_NUM = 0;
int arrival_time = 0;       // instead of clock, set and incremented by 2 before a process is executed
int CPU_runtime = TOTAL_RUNTIME;

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

class processTable {  
public:
    /* -- attributes -- */
    std::vector<pcb*> processPCB; // List of Pcb pointer structs
    std::vector<int> pid_list; //list for PID's of the same PCB in the process PCB list
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
        for (int i = 0; i < pid_list.size(); i++) {
            if (pid_list.at(i) == _pid)
                return processPCB[i]->accumulated_T;
        }
    }

    int getProcessW(int _pid) { // Get the accumulated_W from the specified process which is called by the process ID 
        for (int i = 0; i < pid_list.size(); i++) {
            if (pid_list.at(i) == _pid)
                return processPCB[i]->accumulated_W;
        }
    }

    int getProcessR(int _pid) { // Get the accumulated_R from the specified process which is called by the process ID 
        for (int i = 0; i < pid_list.size(); i++) {
            if (pid_list.at(i) == _pid)
                return processPCB[i]->accumulated_R;
        }
    }

    std::vector<pcb*> getProcessPcbList() { return processPCB; } // Return the entire vector of pointer to pcb structs in the process table

    pcb* getProcessPcb(int index) { return processPCB.at(index); } // Return the pointer to that pcb at the index in the vector of process PCB's

    std::vector<int> getPidList() { return pid_list; } // Return the entire vector of pid's in the process table

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
        //std::ofstream cpu_data;
        //cpu_data.open("data_file.csv", std::ios::in);
        ////Does the file exist?
        //if (cpu_data.fail()) { std::cout << "The file you are trying to open DNE.\n"; exit(1); } //Verify file actually opened

        //cpu_data << "PID # | ARRIVAL TIME | CPU TIME | IO TIME | (Repeating on until process ends)\n"; // Write the code for the values being placed into the table
        // Possibly comment out the line above to make code easier, and have this here instead as a key to refer too.

        for (int i = 0; i < processCount; i++) {
            processes[i].pid = PID_ASSIGNED_NUM; // Set the PID of the processes, starting at 1
            PID_ASSIGNED_NUM++;
            pcbs[i].p_pointer = &processes[i]; // Set the p_pointer as a pointer to the [i] process
            pcbs[i].pid = processes[i].pid; // Set the pcb's PID to the process [i]'s PID
            pcbs[i].arrivalTime = 0;    // Set arrivalTime to 0 initially
            pcbs[i].ioTime = 0; // Set ioTime to 0 initially
            pcbs[i].priority = priority; // Set the priority given, 0 be default | low, medium, high, critical
            pcbs[i].state = false; // Set the state to waiting
            int proc_runtime = rand() % 20;
            pcbs[i].cpuBurstTime = proc_runtime; // Set the cpu burst time to a random number between 0 and 
            pcbs[i].accumulated_T = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_W = 0; // set the accumulated_T to 0 due to being just created
            pcbs[i].accumulated_R = 0; // set the accumulated_T to 0 due to being just created
            // Write to the cpu's file for this process on line [i]
            //- Form a string with the data just made above, then place that string in the CSV file on the line [i]+1 (or 2?)
            // PID #, Arrival t, CPU t, IO t, CPU t, IO t, CPU t 
            // cpu_data <<pcbs[i].pid <<", "<< pcbs[i].arrivalTime <<", "<< pcbs[i].cpuBurstTime <<", "<< pcbs[i].ioTime <<", "<< pcbs[i].cpuBurstTime <<", "<< pcbs[i].ioTime;
        }
        // Close the fstream
        //cpu_data.close();

        // populate process table with process and their pcbs 
        for (int i = 0; i < processCount; i++) { table.addPCB(&pcbs[i]); }
    }

    ~cpu() {
        delete processes;
        delete pcbs;
    }


    /* -- base class functions -- */
    int runproc(pcb* _selectedProcess, int runtime) // runs a process at specified 
    {
        CPU_runtime -= _selectedProcess->cpuBurstTime;      //once a process is run remove time taken to run it from CPU
        // update burst time, iotime, etc
        int temp_ioTime = rand() % 50;  // simulate random i/o time
        int temp_cpu_burst = rand() % 20;   // simulate random cpu time
        //_selectedProcess->arrivalTime = clock or time of system?
        //_selectedProcess->accumulated_W = time spent in wait queue
        //_selectedProcess->accumulated_R = response time, not sure how to calculate
        _selectedProcess->ioTime = temp_ioTime; // assign simulated i/o time to selected process
        _selectedProcess->cpuBurstTime = temp_cpu_burst;    // assign simulated cpu time to selected process
        _selectedProcess->accumulated_T = (_selectedProcess->accumulated_W + runtime);  // turnaround time = wait time + execution time

        // print process information to data file once process is run
        std::ofstream proc_data;
        proc_data.open("data_file.csv");
        //Does the file exist?
        if (proc_data.fail()) { std::cout << "The file you are trying to open DNE.\n"; exit(1); } //Verify file actually opened
        proc_data << _selectedProcess->pid << "," << _selectedProcess->cpuBurstTime << "," << _selectedProcess->ioTime << "," << _selectedProcess->cpuBurstTime << "," << _selectedProcess->ioTime << "," << _selectedProcess->cpuBurstTime;
        proc_data.close();
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

        return success;
    }

    /* -- Getters & Setters -- */
    processTable getProcessTable() { return table; }

    /* -- printf / cout function -- */
    void print_data() {

        //return success;
    }
};

std::vector<int> FCFS(cpu present) {
    int curno = present.table.getPidList().size();
    std::vector<int> holder;

    std::deque<pcb*> wait_queue;
    std::deque<pcb*> ready_queue;

    //grab processes within the present CPU
    //put some in a waiting queue
    for (int i = 0; i < present.table.getPidList().size(); i++)
    {
        wait_queue.push_back(present.table.getProcessPcb(i));
    }

    //while the wait queue still has processes
    while (!wait_queue.empty())
    {
        //select a random number of processes from wait queue and add to ready queue
        int randno = rand() % curno;
        curno = curno - randno;
        //move processes from wait queue to ready queue
        for (int i = 0; i <= randno; i++) {
            //get top process of wait queue because pop destroys element
            pcb* curr_process = wait_queue.front();
            //pop process from wait queue
            wait_queue.pop_front();
            //add to ready queue
            ready_queue.push_back(curr_process);
        }

        //run processes in ready queue 
        while (!ready_queue.empty()) {
            present.runproc(ready_queue.front(), ready_queue.front()->ioTime);
            ready_queue.pop_front();    // remove process already run
        }
    }
    return holder;
}

bool compare(std::pair<int, int> i, std::pair<int, int> j) {
    return i.second < j.second;
}

int checkproc(std::deque<pcb*> hold, int iotime) {
    int holding;
    for (int i = 0; i < hold.size(); i++) {
        if (hold[i]->ioTime == iotime) {
            holding = i;
            break;
        }
    }
    return holding;
}

std::vector<int> SPT(cpu present) {
    std::vector<int> holder;
    std::map<int, int> processholder;
    int curno = present.table.getPidList().size();

    //map holds iotimes and index
    for (int i = 0; i < present.table.getPidList().size(); i++) {
        processholder.insert(std::pair<int, int>(i, present.table.getProcessPcb(i)->ioTime));
    }

    //having waitqueue of processes
    std::deque<pcb*> wait_queue;
    std::deque<pcb*> ready_queue;

    //grab processes within the present CPU
    //put some in a waiting queue
    for (int i = 0; i < present.table.getPidList().size(); i++)
    {
        wait_queue.push_back(present.table.getProcessPcb(i));
    }

    while (!wait_queue.empty())
    {
        //select a random number of processes from wait queue and add to ready queue
        int randno = rand() % curno;
        curno = curno - randno;
        //move processes from wait queue to ready queue
        for (int i = 0; i <= randno; i++) {
            //get min ioTime from map 
            std::pair<int, int> min = *std::min_element(processholder.begin(), processholder.end(), compare);
            int ioTim = min.second;
            //run function to get process with that iotime and push process to ready queue
            int index = checkproc(wait_queue, ioTim);
            //add that process to ready queue
            ready_queue.push_back(wait_queue[index]);
            //delete that process
            wait_queue.erase(wait_queue.begin() + index);
            //add that process to ready queue
        }
        //run processes in ready queue 
        while (!ready_queue.empty()) {
            present.runproc(ready_queue.front(), ready_queue.front()->ioTime);
            ready_queue.pop_front();    // remove process already run
        }
    }
    return holder;
}

std::vector<int> Robin(cpu present) {
    std::vector<int> holder;
    int curno = present.table.getPidList().size();
    // holds processes just created
    std::deque<pcb*> wait_queue;
    // holds processes ready to be executed
    std::deque<pcb*> ready_queue;
    //create map mapping process to its time left (map holds pid and time left)
    std::map<int, int> processholder;
    //populate process holder with pid and iotime 
    for (int i = 0; i < present.table.getPidList().size(); i++) {
        processholder.insert(std::pair<int, int>(present.table.getProcessPcb(i)->pid, present.table.getProcessPcb(i)->ioTime));
    }
    //if process has no time left, delete it from waiting queue
    //if it still has time, move it back to waiting queue and remove it from ready queue

    //load wait queue 
    for (int i = 0; i < curno; i++)
    {
        wait_queue.push_back(present.table.getProcessPcb(i));
    }
    //while the wait queue still has processes
    while (!wait_queue.empty())
    {  //select a random number of processes from wait queue and add to ready queue
        int randno = rand() % curno;
        curno = curno - randno;
        //move processes from wait queue to ready queue
        for (int i = 0; i <= randno; i++) {
            //get top process of wait queue because pop destroys element
            pcb* curr_process = wait_queue.front();
            //pop process from wait queue
            wait_queue.pop_front();
            //add to ready queue
            ready_queue.push_back(curr_process);
        }
        //while ready queue isnt empty
        while (!ready_queue.empty()) {
            int runtime = rand() % processholder.at(ready_queue.front()->pid);
            //select time that current process wil run
             //run top process
            present.runproc(ready_queue.front(), runtime);
            //update in map
            processholder.at(ready_queue.front()->pid) = processholder.at(ready_queue.front()->pid) - runtime;
            if (processholder.at(ready_queue.front()->pid) > 0) {
                //put process into wait queue
                wait_queue.push_back(ready_queue.front());
                //remove from ready queue
                ready_queue.pop_front();
            }
            else {
                ready_queue.pop_front();
            }
        }
    }
    return holder;

}

int main()
{
    cpu virtualCPU[CPU_COUNT];

    // call scheduling function for each cpu
    //FCFS(virtualCPU[0]);

     

    SPT(virtualCPU[1]);

    //Robin(virtualCPU[2]);

    //SPT(virtualCPU[4]);

    //Robin(virtualCPU[5]);

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
