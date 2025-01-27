/*
    Question:
    Write a code to implement the Shortest Job First scheduling algorithm and calculate average turnaround time, waiting time and response time of the scheduled processes. Your code must generate a gantt chart as well.
*/
#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    int arrival;
    int cpu_time;
    int waiting;
    int turnaround;
    int response_time;
    int completion;
    bool executed;

    Process(int id, int arrival, int cpu_time)
        : id(id), arrival(arrival), cpu_time(cpu_time),
          waiting(0), turnaround(0), response_time(-1),
          completion(0), executed(false) {}
};

struct GanttEntry {
    int id;
    int start;
    int end;
};

void shortestJobFirst(vector<Process>& processes, vector<GanttEntry>& gantt,
                      double& avg_waiting, double& avg_turnaround, double& avg_response) {
    int current_time = 0;
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_response = 0;
    int n = processes.size();
    int completed = 0;

    while (completed < n) {
        // Find the process with the shortest CPU time that has arrived and not yet executed
        int shortest_index = -1;
        int shortest_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!processes[i].executed && processes[i].arrival <= current_time && processes[i].cpu_time < shortest_time) {
                shortest_time = processes[i].cpu_time;
                shortest_index = i;
            }
        }

        if (shortest_index == -1) {
            current_time++;
            continue;
        }

        Process& current = processes[shortest_index];

        // Set response time if first run
        if (current.response_time == -1) {
            current.response_time = current_time - current.arrival;
            total_response += current.response_time;
        }

        // Execute the process
        gantt.push_back({current.id, current_time, current_time + current.cpu_time});
        current.completion = current_time + current.cpu_time;
        current.turnaround = current.completion - current.arrival;
        current.waiting = current.turnaround - current.cpu_time;
        total_waiting += current.waiting;
        total_turnaround += current.turnaround;
        current.executed = true;
        current_time = current.completion;
        completed++;
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
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    for (int i = 0; i < n; i++) {
        int arrival, cpu_time;
        cout << "Enter arrival and CPU time for process " << (i + 1) << ": ";
        cin >> arrival >> cpu_time;
        processes.emplace_back(i + 1, arrival, cpu_time);
    }

    vector<GanttEntry> gantt;
    double avg_waiting, avg_turnaround, avg_response;

    shortestJobFirst(processes, gantt, avg_waiting, avg_turnaround, avg_response);

    printGantt(gantt);

    cout << "Average Waiting Time: " << avg_waiting << "\n";
    cout << "Average Turnaround Time: " << avg_turnaround << "\n";
    cout << "Average Response Time: " << avg_response << "\n";

    return 0;
}

/*
    Input:
        Enter number of processes: 4
        Enter arrival and CPU time for process 1: 0 6
        Enter arrival and CPU time for process 2: 1 4
        Enter arrival and CPU time for process 3: 2 2
        Enter arrival and CPU time for process 4: 3 3
    Output:
        Gantt Chart:
        [P1 | 0-6] [P3 | 6-8] [P4 | 8-11] [P2 | 11-15] 

        Average Waiting Time: 4.75
        Average Turnaround Time: 8.5
        Average Response Time: 4.75

        Press any key to continue...
    
*/