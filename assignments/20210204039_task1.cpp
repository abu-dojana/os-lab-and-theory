/* Question 1
  Write a code to implement the Round robin scheduling algorithm and calculate average turnaround time, waiting time and response time of the scheduled processes. Your code must generate a gantt chart as well. 
*/
#include <bits/stdc++.h>
using namespace std;


struct Process {
    int id;
    int arrival;
    int cpu_time;
    int remaining;
    int waiting;
    int turnaround;
    int response_time;
    int completion;
    bool in_queue;

    Process(int id, int arrival, int cpu_time)
        : id(id), arrival(arrival), cpu_time(cpu_time),
          remaining(cpu_time), waiting(0), turnaround(0),
          response_time(-1), completion(0), in_queue(false) {}
};

struct GanttEntry {
    int id;
    int start;
    int end;
};

void roundRobin(vector<Process>& processes, int time_quantum, vector<GanttEntry>& gantt,
                double& avg_waiting, double& avg_turnaround, double& avg_response) {
    queue<Process*> q;
    vector<Process*> remaining;
    int current_time = 0;
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0;
    int n = processes.size();

    for (auto& p : processes) remaining.push_back(&p);

    sort(processes.begin(), processes.end(), 
        [](const Process& a, const Process& b) { return a.arrival < b.arrival; });

    while (!remaining.empty()) {
        // Add arriving processes to queue
        for (auto& p : processes) {
            if (p.arrival <= current_time && p.remaining > 0 && !p.in_queue) {
                q.push(&p);
                p.in_queue = true;
            }
        }

        if (q.empty()) {
            current_time++;
            continue;
        }

        Process* current = q.front();
        q.pop();
        current->in_queue = false;

        // Set response time if first run
        if (current->response_time == -1) {
            current->response_time = current_time - current->arrival;
            total_response += current->response_time;
        }

        // Execute process
        int run_time = min(time_quantum, current->remaining);
        gantt.push_back({current->id, current_time, current_time + run_time});
        
        // Update waiting time for processes in queue
        queue<Process*> temp = q;
        while (!temp.empty()) {
            temp.front()->waiting += run_time;
            temp.pop();
        }

        current_time += run_time;
        current->remaining -= run_time;

        if (current->remaining > 0) {
            // Re-add to queue if not finished
            q.push(current);
            current->in_queue = true;
        } else {
            // Update completion metrics
            current->completion = current_time;
            current->turnaround = current->completion - current->arrival;
            total_waiting += current->waiting;
            total_turnaround += current->turnaround;
            
            // Remove from remaining list
            auto it = find(remaining.begin(), remaining.end(), current);
            if (it != remaining.end()) remaining.erase(it);
        }
    }

    avg_waiting = static_cast<double>(total_waiting) / n;
    avg_turnaround = static_cast<double>(total_turnaround) / n;
    avg_response = static_cast<double>(total_response) / n;
}

void printGantt(const vector<GanttEntry>& gantt) {
    cout << "Gantt Chart:\n";
    for (const auto& entry : gantt) {
        cout << "[P" << entry.id << " | " << entry.start << "-" << entry.end << "] ";
    }
    cout << "\n\n";
}

int main() {
    int n, time_quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> time_quantum;

    vector<Process> processes;
    for (int i = 0; i < n; i++) {
        int arrival, cpu_time;
        cout << "Enter arrival and CPU time for process " << (i+1) << ": ";
        cin >> arrival >> cpu_time;
        processes.emplace_back(i+1, arrival, cpu_time);
    }

    vector<GanttEntry> gantt;
    double avg_waiting, avg_turnaround, avg_response;
    
    roundRobin(processes, time_quantum, gantt, avg_waiting, avg_turnaround, avg_response);
    
    printGantt(gantt);
    
    cout << "Average Waiting Time: " << avg_waiting << "\n";
    cout << "Average Turnaround Time: " << avg_turnaround << "\n";
    cout << "Average Response Time: " << avg_response << "\n";

    return 0;
}


/*
    Example Problem:
    ====================================
    Process     Arrival Time    CPU Time
    ------------------------------------
    P1              0               6
    P2              2               6
    P3              5               7
    P4              6               6
    P5              8               7
    P6              18              3
    P7              22              3
    ====================================
    Time Quantum: 3
    
    Gannt Chart:
    [P1 | 0-3] [P2 | 3-6] [P1 |6-9] [P3 | 9-12] [P4 | 12-15] [P2 | 15-18] [P5 | 18-21] 
    [P3 | 21-24] [P4 | 24-27] [P6 | 27-30] [P5 | 30-33] [P3 | 33-34] [P7 | 34-37] 
    [P5 | 37-38]
    
*/