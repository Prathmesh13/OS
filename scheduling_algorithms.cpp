#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int finishTime;
    int turnaroundTime;
    int waitingTime;
    int remainingTime;
};

void calculateTimes(vector<Process>& processes) {
    for (auto& p : processes) {
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
    }
}

void fcfs(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.finishTime = currentTime + p.burstTime;
        currentTime += p.burstTime;
    }
    calculateTimes(processes);
}

void sjfNonPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    vector<bool> completed(processes.size(), false);
    for (int i = 0; i < processes.size(); ++i) {
        int idx = -1;
        int minBurstTime = INT_MAX;
        for (int j = 0; j < processes.size(); ++j) {
            if (!completed[j] && processes[j].arrivalTime <= currentTime && processes[j].burstTime < minBurstTime) {
                minBurstTime = processes[j].burstTime;
                idx = j;
            }
        }
        if (idx == -1) {
            currentTime++;
        } else {
            processes[idx].finishTime = currentTime + processes[idx].burstTime;
            currentTime += processes[idx].burstTime;
            completed[idx] = true;
        }
    }
    calculateTimes(processes);
}

void sjfPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    vector<bool> isCompleted(n, false);
    while (completed != n) {
        int idx = -1;
        int minRemainingTime = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i] && processes[i].remainingTime < minRemainingTime) {
                minRemainingTime = processes[i].remainingTime;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].remainingTime--;
            if (processes[idx].remainingTime == 0) {
                processes[idx].finishTime = currentTime + 1;
                isCompleted[idx] = true;
                completed++;
            }
            currentTime++;
        } else {
            currentTime++;
        }
    }
    calculateTimes(processes);
}

void priorityNonPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    vector<bool> completed(processes.size(), false);
    for (int i = 0; i < processes.size(); ++i) {
        int idx = -1;
        int highestPriority = INT_MAX;
        for (int j = 0; j < processes.size(); ++j) {
            if (!completed[j] && processes[j].arrivalTime <= currentTime && processes[j].priority < highestPriority) {
                highestPriority = processes[j].priority;
                idx = j;
            }
        }
        if (idx == -1) {
            currentTime++;
        } else {
            processes[idx].finishTime = currentTime + processes[idx].burstTime;
            currentTime += processes[idx].burstTime;
            completed[idx] = true;
        }
    }
    calculateTimes(processes);
}

void priorityPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    vector<bool> isCompleted(n, false);
    while (completed != n) {
        int idx = -1;
        int highestPriority = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i] && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].remainingTime--;
            if (processes[idx].remainingTime == 0) {
                processes[idx].finishTime = currentTime + 1;
                isCompleted[idx] = true;
                completed++;
            }
            currentTime++;
        } else {
            currentTime++;
        }
    }
    calculateTimes(processes);
}

void roundRobin(vector<Process>& processes, int quantum) {
    queue<int> q;
    vector<bool> inQueue(processes.size(), false);
    int currentTime = 0;
    int completed = 0;

    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].arrivalTime == currentTime) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed != processes.size()) {
        if (!q.empty()) {
            int idx = q.front();
            q.pop();
            inQueue[idx] = false;

            int execTime = min(quantum, processes[idx].remainingTime);
            currentTime += execTime;
            processes[idx].remainingTime -= execTime;

            for (int i = 0; i < processes.size(); i++) {
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }

            if (processes[idx].remainingTime > 0) {
                q.push(idx);
                inQueue[idx] = true;
            } else {
                processes[idx].finishTime = currentTime;
                completed++;
            }
        } else {
            currentTime++;
        }
    }
    calculateTimes(processes);
}

int main() {
    int n, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> processes(n);

    cout << "Enter Arrival Time, Burst Time, and Priority for each process:\n";
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime;
    }

    cout << "Enter time quantum for Round Robin: ";
    cin >> quantum;

    fcfs(processes);
    cout << "\nFCFS Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    sjfNonPreemptive(processes);
    cout << "\n\nSJF Non-Preemptive Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    sjfPreemptive(processes);
    cout << "\n\nSJF Preemptive Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    priorityNonPreemptive(processes);
    cout << "\n\nPriority Non-Preemptive Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    priorityPreemptive(processes);
    cout << "\n\nPriority Preemptive Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    roundRobin(processes, quantum);
    cout << "\n\nRound Robin Results:";
    for (const auto& p : processes)
        cout << "\nProcess " << p.id << " -> FT: " << p.finishTime << ", TAT: " << p.turnaroundTime << ", WT: " << p.waitingTime;

    return 0;
}
