#include <bits/stdc++.h>
using namespace std;

struct Task{
    int pid;        // Process ID
    int tid;        // Task ID
    int period;     // Period (time between arrivals)
    int execution;  // Execution time (time to complete a job)
    int deadline;   // Absolute deadline (arrival time + period)
    int remaining;  // Remaining execution time (for preemption)
    int policy;     // Scheduling policy
};

int lcm(int a, int b) {
  int greater = max(a, b);
  int smallest = min(a, b);
  for (int i = greater; ; i += greater) {
    if (i % smallest == 0)
      return i;
  }
}

bool sortcol(const vector<int>& v1, const vector<int>& v2) {
  return v1[1] < v2[1];
}

class MyClass
{
  public:
    int operator() (const struct Task& t1, const struct Task& t2)
    {
      if (t1.policy == 0) {
        if (t1.period == t2.period)
          return t1.pid > t2.pid;
        else
          return t1.period > t2.period;
      }
      else {
        if (t1.deadline == t2.deadline)
          return t1.pid > t2.pid;
        else
          return t1.deadline > t2.deadline;
      }
    }
};

int main() {
  int R,  //number of runs 
      S,  //scheduling policy
      D,  //Display schedule
      N;  //number of tasks
  cin >> R >> S >> D >> N;
  vector<vector<int> > vec(N, vector<int> (2));
  for (int i = 0; i < N; i++) {
    cin >> vec[i][0] >> vec[i][1];
  }

  /*for (int i = 0; i < N; i++)
    cout << vec[i][0] << " " << vec[i][1] << '\n';
  */
  sort(vec.begin(), vec.end(), sortcol);
  /*
  for (int i = 0; i < N; i++)
    cout << vec[i][0] << " " << vec[i][1] << '\n';
  */
  int hyperperiod = vec[0][1];
  for (int i = 1; i < N; i++) {
    hyperperiod = lcm(hyperperiod, vec[i][1]);
  }
  //cout << hyperperiod << '\n';
  int pid = 0;
  
  priority_queue<Task, vector<Task>, MyClass> task_queue;
  
  for (int i = 0; i < N; i++) {
      pid++;
      Task job = {pid,            //Process ID
                  i + 1,          //Task ID
                  vec[i][1],      //Period 
                  vec[i][0],      //Execution time
                  0 + vec[i][1],  //Absolute deadline
                  vec[i][0],      //Remaining Execution Time
                  S};             //Scheduling Policy
      task_queue.push(job);
  }

  struct Task current_task = task_queue.top();
  task_queue.pop();
  cout << "0 " << current_task.tid << " 0\n";
  current_task.remaining--;
  if (current_task.remaining != 0)
    task_queue.push(current_task);
  int preempt = 0;
  for (int t = 1; t < hyperperiod; t++) {
    for (int i = 0; i < N; i++) {
      if (t % vec[i][1] == 0) {
        pid++;
        Task job = {pid,            //Process ID
                    i + 1,          //Task ID
                    vec[i][1],      //Period 
                    vec[i][0],      //Execution time
                    t + vec[i][1],  //Absolute deadline
                    vec[i][0],      //Remaining Execution Time
                    S};
        task_queue.push(job);
      }
    }
    if (current_task.remaining ==  0) {
      int previous = current_task.tid;
      if (task_queue.empty()) {
        cout << t << " " << previous << " 1\n";
        continue;
      }
      else {
        current_task = task_queue.top();
        if (current_task.remaining == current_task.execution)
          cout << t << " " << previous << " 1 " << current_task.tid << " 0\n";
        else
          cout << t << " " << previous << " 1 " << current_task.tid << " 3\n";
      }
    }
    else if (current_task.pid != task_queue.top().pid) {
      preempt++;
      cout << t << " " << current_task.tid << " 2 " << task_queue.top().tid << " 0\n";
    }
    current_task = task_queue.top();
    task_queue.pop();
    current_task.remaining--;
    if (current_task.remaining != 0)
      task_queue.push(current_task);
  }
  cout << hyperperiod << " " << preempt << '\n';
  
  return 0;
}
