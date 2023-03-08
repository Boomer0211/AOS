#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> iPair;
struct Task{
    int pid;        // Process ID
    int tid;        // Task ID
    int period;     // Period (time between arrivals)
    int execution;  // Execution time (time to complete a job)
    int deadline;   // Absolute deadline (arrival time + period)
    int remaining;  // Remaining execution time (for preemption)
    int policy;     // Scheduling policy
};

unsigned long long lcm(unsigned long long a, int b) {
  unsigned long long greater, smallest;
  if (a > b) {
    greater = a;
    smallest = b;
  } else {
    greater = b;
    smallest = a;
  }
  //unsigned long long greater = max(a, b);
  //int smallest = min(a, b);
  for (unsigned long long i = greater; ; i += greater) {
    if (i % smallest == 0)
      return i;
  }
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
  int R;  //number of runs
  cin >> R;
  int S[R], D[R], N[R];
  vector<vector<iPair> > vec(R);
  for (int i = 0; i < R; i++) {
    cin >> S[i] >> D[i] >> N[i];
    for (int j = 0; j < N[i]; j++) {
      int e, p;
      cin >> e >> p;
      vec[i].push_back(make_pair(e, p));
      //cout << vec[i][j].first << " " << vec[i][j].second << '\n';
    }
  }
  /*
  unsigned long long hyperperiod = vec[0][0].second;
    for (int i = 1; i < 4; i++) {
      hyperperiod = lcm(hyperperiod, (unsigned long long)vec[0][i].second);
    }
    cout << hyperperiod << '\n';
    */
  /*
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < N[i]; j++) {
      cout << vec[i][j].first << " " << vec[i][j].second << '\n';
    }
  }
  */
  
  for (int run = 1; run <= R; run++) {
    cout << run << '\n'; 
    unsigned long long hyperperiod = vec[run - 1][0].second;
    for (int i = 1; i < N[run - 1]; i++) {
      hyperperiod = lcm(hyperperiod, (unsigned long long)vec[run - 1][i].second);
    }
    
    //cout << hyperperiod << '\n';
    
    int pid = 0;
  
    priority_queue<Task, vector<Task>, MyClass> task_queue;
  
    for (int i = 0; i < N[run - 1]; i++) {
        pid++;
        Task job = {pid,                          //Process ID
                    i + 1,                        //Task ID
                    vec[run - 1][i].second,       //Period 
                    vec[run - 1][i].first,        //Execution time
                    0 + vec[run - 1][i].second,   //Absolute deadline
                    vec[run - 1][i].first,        //Remaining Execution Time
                    S[run - 1]};                  //Scheduling Policy
        task_queue.push(job);
    }

    struct Task current_task = task_queue.top();
    task_queue.pop();
    if (D[run - 1] == 1)
      cout << "0 " << current_task.tid << " 0\n";
    current_task.remaining--;
    if (current_task.remaining != 0)
      task_queue.push(current_task);
    unsigned long long preempt = 0;
    for (unsigned long long t = 1; t < hyperperiod; t++) {
      for (int i = 0; i < N[run - 1]; i++) {
        if (t % vec[run - 1][i].second == 0) {
          pid++;
          Task job = {pid,                        //Process ID
                      i + 1,                      //Task ID
                      vec[run - 1][i].second,     //Period 
                      vec[run - 1][i].first,      //Execution time
                      t + vec[run - 1][i].second, //Absolute deadline
                      vec[run - 1][i].first,      //Remaining Execution Time
                      S[run - 1]};
          task_queue.push(job);
        }
      }
      if (current_task.remaining ==  0) {
        int previous = current_task.tid;
        if (task_queue.empty()) {
          if (D[run - 1] == 1)
            cout << t << " " << previous << " 1\n";
          continue;
        }
        else {
          current_task = task_queue.top();
          if (current_task.remaining == current_task.execution) {
            if (D[run - 1] == 1)
              cout << t << " " << previous << " 1 " << current_task.tid << " 0\n";
          } else {
            if (D[run - 1] == 1)
              cout << t << " " << previous << " 1 " << current_task.tid << " 3\n";
          }
        }
      }
      else if (current_task.pid != task_queue.top().pid) {
        preempt++;
        if (D[run - 1] == 1)
          cout << t << " " << current_task.tid << " 2 " << task_queue.top().tid << " 0\n";
      }
      current_task = task_queue.top();
      task_queue.pop();
      current_task.remaining--;
      if (current_task.remaining > 0)
        task_queue.push(current_task);
    }
    cout << hyperperiod << " " << preempt << '\n';
  }
  
  return 0;
}
