/*
RACHITA SOWLE
201505613
*/

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

class process
{
	public:
	int key, id, arrival_time, burst_time, Start_Time, Finish_Time, priority, remaining_time;
	bool operator<(const process &p) const
	{
        	return priority > p.priority;
	}
};

bool MyCompArrTime(const process &a, const process &b)
{
    return a.arrival_time < b.arrival_time;
}
bool MyCompByKey(const process &a, const process &b)
{
    return a.key < b.key;
}

int main()
{
	vector<process> vec;
	vector<process> vecOut;
	map<int,int> sflag;
	vector<process> tempVec;
	int n;
	cin >> n;
	int j;
	for(j=0; j<n; j++)
	{
		process* p = new process();
		cin >> p->id;
		cin >> p->arrival_time;
		cin >> p->burst_time;
		cin >> p->priority;

		p->remaining_time = p->burst_time;
		p->key = j;
		vec.push_back(*p);
		sflag[p->id] = 1;
	}
	
	
	priority_queue<process> priorityQueue;
	queue<process> RRQueue;
	int i=0, rrflag=0;
	bool allIn = false;

	int ticks = 0, currTime=0;
	sort(vec.begin(), vec.end(), MyCompArrTime);
	while(1)
	{
		if(priorityQueue.empty() && RRQueue.empty() && (int)vecOut.size()==n) //All processes are scheduled
			break;
		
		if(!allIn)
		{
			while(currTime == (vec.begin()+i)->arrival_time)
			{
				if(!priorityQueue.empty())
				{
					if(((vec.begin()+i)->priority < priorityQueue.top().priority) && ticks>0)
					{
						process p = priorityQueue.top();
						priorityQueue.pop();
						RRQueue.push(p);
						ticks=0;
						if(!priorityQueue.empty())
						{
							int currP = priorityQueue.top().priority;
							while(priorityQueue.top().priority==currP)
							{
								tempVec.push_back(priorityQueue.top());
								priorityQueue.pop();
								if(priorityQueue.empty())
									break;
							}
							sort(tempVec.begin(), tempVec.end(), MyCompByKey);
							vector<process>::iterator itt = tempVec.begin();
							while(itt!=tempVec.end())
							{
								priorityQueue.push(*itt);
								itt++;
							}
							tempVec.clear();
						}
					}
				}
				priorityQueue.push(*(vec.begin() + i));
				if(++i==n)
				{
					allIn = true;
					break;
				}
			}
		}
		if(priorityQueue.empty() && !RRQueue.empty())
		{
			rrflag = 1;
			ticks++;
			RRQueue.front().remaining_time--;
			if(RRQueue.front().remaining_time == 0)
			{
				// process termination
				process p = RRQueue.front();
				RRQueue.pop();
				p.Finish_Time = currTime + 1;
				vecOut.push_back(p);
				ticks=0;
			}
			
			else if(ticks == 4)
			{
				// Round robin time slice
				process p = RRQueue.front();
				RRQueue.pop();
				RRQueue.push(p);
				ticks = 0;
			}
		}
		else if(!priorityQueue.empty())
		{
			process current = priorityQueue.top();
			if(sflag[current.id])
			{
				current.Start_Time = currTime;
				sflag[current.id] = 0;
			}
			if((ticks>0)&&(!RRQueue.empty()) && rrflag)
			{
				ticks=0;
				RRQueue.push(RRQueue.front());
				RRQueue.pop();
				rrflag = 0;
			}
			current.remaining_time--;
			priorityQueue.pop();
			tempVec.push_back(current);
			if(!priorityQueue.empty())
			{
				while(priorityQueue.top().priority==current.priority)
				{
					tempVec.push_back(priorityQueue.top());
					priorityQueue.pop();
					if(priorityQueue.empty())
						break;
				}
				sort(tempVec.begin(), tempVec.end(), MyCompByKey);
			}
			vector<process>::iterator itt = tempVec.begin();
			while(itt!=tempVec.end())
			{
				priorityQueue.push(*itt);
				itt++;
			}
			tempVec.clear();
			ticks++;
			if(priorityQueue.top().remaining_time==0)
			{
				process p = priorityQueue.top();
				priorityQueue.pop();
				p.Finish_Time = currTime+1;
				vecOut.push_back(p);
				ticks = 0;
				if(!priorityQueue.empty())
				{
					int currP = priorityQueue.top().priority;
					while(priorityQueue.top().priority==currP)
					{
						tempVec.push_back(priorityQueue.top());
						priorityQueue.pop();
						if(priorityQueue.empty())
							break;
					}
					sort(tempVec.begin(), tempVec.end(), MyCompByKey);
					vector<process>::iterator itt = tempVec.begin();
					while(itt!=tempVec.end())
					{
						priorityQueue.push(*itt);
						itt++;
					}
					tempVec.clear();
				}
				
			}
			else if(ticks == 4)
			{	
				process p = priorityQueue.top();
				RRQueue.push(p);
				priorityQueue.pop();
				if(!priorityQueue.empty())
				{
					int currP = priorityQueue.top().priority;
					while(priorityQueue.top().priority==currP)
					{
						tempVec.push_back(priorityQueue.top());
						priorityQueue.pop();
						if(priorityQueue.empty())
							break;
					}
					sort(tempVec.begin(), tempVec.end(), MyCompByKey);
					vector<process>::iterator itt = tempVec.begin();
					while(itt!=tempVec.end())
					{
						priorityQueue.push(*itt);
						itt++;
					}
					tempVec.clear();
				}
				ticks = 0;	
			}
		}
		currTime++;
	}
	
	// Sorting for output display
        sort(vecOut.begin(), vecOut.end(), MyCompByKey);
	vector<process>::iterator it = vecOut.begin();
	
	while(it!=vecOut.end())
	{
		cout << it->id << " ";
		cout << it->Start_Time - it->arrival_time << " ";
		cout << it->Finish_Time << " ";
		cout << it->Finish_Time - it->burst_time - it->arrival_time << endl;	
		it++;
	}
}
