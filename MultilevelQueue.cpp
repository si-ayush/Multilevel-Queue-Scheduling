#include <iostream>

#include<stdio.h>

#include<bits/stdc++.h>

using namespace std;
static int queue_type = -1;
static int num_queue = 3;

struct process {
  int priority;
  int burst_time;
  int tt_time;
  int total_time = 0;
};

struct queues {

  int pri_start;
  int pri_end;

  process *proc;
  int total_time = 0;
  int length = 0;
  bool executed = false;
};

bool notComplete(queues process_Q[]) {
  bool ans = false;
  int countInc = 0;

  for (int i = 0; i < num_queue; i++) {
    countInc = 0;
    for (int j = 0; j < process_Q[i].length; j++) {
      if (process_Q[i].proc[j].burst_time != 0) {
        ans = true;
      } else {
        countInc++;
      }
    }

    if (countInc == process_Q[i].length)
      process_Q[i].executed = true;
  }

  return ans;
}

void checkCompleteTimer(queues process_Q[]) {
  bool ans = notComplete(process_Q);
  for (int i = 0; i < num_queue; i++) {
    if (process_Q[i].executed == false) {
      for (int j = 0; j < process_Q[i].length; j++) {
        if (process_Q[i].proc[j].burst_time != 0) {
          process_Q[i].proc[j].total_time += 1;
        }
      }

      process_Q[i].total_time += 1;
    }
  }
}

void roundRobin(queues process_Q[], int & rr_time, int & counterRR, int & timer) {
  int l = queue_type;
  for (int i = 0; i < process_Q[l].length; i++) {
    if (process_Q[l].proc[i].burst_time == 0) {
      counterRR++;
      continue;
    }

    if (counterRR == process_Q[l].length)
      return;

    while (rr_time > 0 && process_Q[l].proc[i].burst_time != 0 && timer != 10) {
      cout << "Executing queue 1 and " << i + 1 << " process for a unit time. Process has priority of " << process_Q[l].proc[i].priority << "\n";
      process_Q[l].proc[i].burst_time--;
      checkCompleteTimer(process_Q);
      rr_time--;
      timer++;
    }

    if (timer == 10)
      return;

    if (rr_time == 0)
      rr_time = 4;

    if (process_Q[l].proc[i].burst_time == 0) {
      if (i == process_Q[l].length - 1)
        i--;
      continue;
    }

    if (rr_time <= 0) {
      rr_time = 4;
      if (i == process_Q[l].length - 1)
        i--;
      continue;
    }
  }
}

void sorting_priority(queues pQ) {
  for (int i = 1; i < pQ.length; i++) {
    for (int j = 0; j < pQ.length - 1; j++) {
      if (pQ.proc[j].priority < pQ.proc[j + 1].priority) {
        process temp = pQ.proc[j + 1];
        pQ.proc[j + 1] = pQ.proc[j];
        pQ.proc[j] = temp;
      }

    }
  }
}

void priorityScheduling(queues process_Q[], int & counterPS, int & timer) {
  int l = 1; // queue 1 is for PS
  sorting_priority(process_Q[l]);

  for (int i = 0; i < process_Q[l].length; i++) {

    if (process_Q[l].proc[i].burst_time == 0) {
      counterPS++;
      continue;
    }
    if (counterPS == process_Q[l].length)
      return;

    while (process_Q[l].proc[i].burst_time != 0 && timer != 10) {
      cout << "Executing queue 2 and " << i + 1 << " process for a unit time. Process has priority of " << process_Q[l].proc[i].priority << "\n";
      process_Q[l].proc[i].burst_time--;
      checkCompleteTimer(process_Q);
      timer++;
    }

    if (timer == 10)
      return;
    if (process_Q[l].proc[i].burst_time == 0)
      continue;
  }
  return;
}

void fcfsSceduling(queues process_Q[], int & counterFCFS, int & timer) {
  int l = 2;
  for (int i = 0; i < process_Q[l].length; i++) {
    if (process_Q[l].proc[i].burst_time == 0) {
      counterFCFS++;
      continue;
    }
    if (counterFCFS == process_Q[l].length)
      return;

    while (process_Q[l].proc[i].burst_time != 0 && timer != 10) {
      cout << "Executing queue 3 and " << i + 1 << " process for a unit time. Process has priority of " << process_Q[l].proc[i].priority << "\n";
      process_Q[l].proc[i].burst_time--;
      checkCompleteTimer(process_Q);
      timer++;
    }

    if (timer == 10)
      return;
    if (process_Q[l].proc[i].burst_time == 0)
      continue;
  }

  return;
}

int main() {

  queues process_Q[num_queue];
  //RR QUEUE
  process_Q[0].pri_start = 7;
  process_Q[0].pri_end = 9;
  //PS QUEUE
  process_Q[1].pri_start = 4;
  process_Q[1].pri_end = 6;
  //FCFS QUEUE
  process_Q[2].pri_start = 1;
  process_Q[2].pri_end = 3;

  int total_proc;
  int burst_proc;
  int pri_proc;

  cout << "Enter Total number of process" << "\n";
  cin >> total_proc;

  process p[total_proc];

  for (int i = 0; i < total_proc; i++) {
    cout << "Enter Burst time of the process" << "\n";
    cin >> p[i].burst_time;

    cout << "Enter priority of the process" << "\n";
    cin >> p[i].priority;

    p[i].tt_time = p[i].burst_time;

    for (int j = 0; j < num_queue; j++) {
      if (process_Q[j].pri_start <= p[i].priority && p[i].priority <= process_Q[j].pri_end) {
        process_Q[j].length++;
      }
    }
  }

  for (int i = 0; i < num_queue; i++) {
    process_Q[i].proc = new process[process_Q[i].length];
  }

  int a = 0, b = 0, c = 0;

  for (int i = 0; i < num_queue; i++) {
    for (int j = 0; j < total_proc; j++) {
      if ((process_Q[i].pri_start <= p[j].priority) && (p[j].priority <= process_Q[i].pri_end)) {
        if (i == 0) {
          process_Q[i].proc[a++] = p[j];
        } else if (i == 1) {
          process_Q[i].proc[b++] = p[j];
        } else {
          process_Q[i].proc[c++] = p[j];
        }
      }
    }
  }

  for (int i = 0; i < num_queue; i++) {
    cout << "Queue" << i + 1 << " : \t";
    for (int j = 0; j < process_Q[i].length; j++) {
      cout << process_Q[i].proc[j].priority << "->";
    }
    cout << "NULL \n";
  }

  //MAIN ROUND ROBIN ALGO ON ALL QUEUES 
  int timer = 0; // time Quantum of 10 will be used for all queues scheduling

  //int queue_type = -1; // QUEUE Type
  int rr_time = 4; // Time quantum for Round Robin

  int counterRR = 0;
  int counterPS = 0;
  int counterFCFS = 0;

  while (notComplete(process_Q)) {
    if (timer == 10) {
      timer = 0;
    }

    queue_type = (queue_type + 1) % num_queue;

    if (process_Q[queue_type].executed == true) {
      cout << "Queue " << queue_type + 1 << " completed" << "\n";

      queue_type = (queue_type + 1) % num_queue;
      continue;
    }

    switch (queue_type) {
    case 0: // ROUND ROBIN FOR QUEUE FIRST QUEUE
      cout << "Working on Queue " << queue_type + 1 << " in Round Robin" << "\n";
      if (rr_time == 0)
        rr_time = 4;

      roundRobin(process_Q, rr_time, counterRR, timer);
      break;
    case 1: // PRIORITY SCHEDULING ON SECOND QUEUE
      cout << "Working on Queue " << queue_type + 1 << " in Priority Scheduling" << "\n";
      priorityScheduling(process_Q, counterPS, timer);
      break;
    default: //FCFS SCHEDULING ON THIRD QUEUE
      cout << "Working on Queue " << queue_type + 1 << " in FCFS Scheduling" << "\n";
      fcfsSceduling(process_Q, counterFCFS, timer);

    }

    cout << "Exiting from the Queue" << queue_type + 1 << "\n";

  }

  // PRINTING THE RESULTS
  int t = 0;
  cout << "\n\n";
  for (int i = 0; i < num_queue; i++) {

    for (int j = 0; j < process_Q[i].length; j++) {
      cout << "Process " << j + 1 << " of queue " << i + 1 << " took " << process_Q[i].proc[j].total_time << "\n";
      t += process_Q[i].proc[j].total_time;
    }
    process_Q[i].total_time = t;
    //cout << "Time taken for queue " << i + 1 << " to execute: " << process_Q[i].total_time << "\n";
    cout << "\n";
  }

  int sum_tt = 0;
  int sum_wt = 0;

  cout << "\n\nProcess     | Turn Around Time | Waiting Time\n";
  for (int i = 0; i < num_queue; i++) {
    cout << "Queue " << i + 1 << "\n";
    for (int j = 0; j < process_Q[i].length; j++) {
      cout << "Process P" << j + 1 << "\t" << process_Q[i].proc[j].total_time << "\t\t    " << process_Q[i].proc[j].total_time - process_Q[i].proc[j].tt_time << "\n";
      sum_tt += process_Q[i].proc[j].total_time;
      sum_wt += process_Q[i].proc[j].total_time - process_Q[i].proc[j].tt_time;
    }
  }

  cout << "\n The average turn around time is : " << sum_tt / total_proc << endl;
  cout << "\n The average waiting time is : " << sum_wt / total_proc << endl;

}
