#ifndef project_H
#define project_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Project{			
	public:
		struct timeWindows_t{
			time_t start_time;
			time_t end_time;
		};

		struct succesor_t{ // Filled before reading (preprocessing) 
			unsigned num_succesors;
			vector<unsigned> succesor_list;
		};	
		
		struct predecessor_t{
			unsigned num_predecessors;
			vector<unsigned> predecessor_list;
		};

		struct operation_t{
			unsigned operation_id;
			unsigned long processing_time;
			unsigned long tw_size;
			vector<timeWindows_t> time_windows;
			predecessor_t predecessors;
			succesor_t succesors;
		};

		struct job_t{
			unsigned job_id;
			unsigned num_operations;
			unsigned weight;
			vector<operation_t> operations;
		};
							
		Project(const unsigned num_jobs, const unsigned planification_time);
		virtual ~Project();
						
		void appendJob(unsigned job_id,unsigned num_operations, unsigned weight);
		void appendOperation(unsigned long operation_id, unsigned long processing_time, unsigned long tw_size);
		void appendTimeWindows(vector<Project::timeWindows_t> time_windows);
		void appendPredecessors(Project::predecessor_t predecessors);
	
		unsigned getNumJobs();
		unsigned getPlanificationTime();
		vector<job_t> getJobs();
		
	private:
		unsigned num_jobs;
		unsigned planification_time;
		vector<job_t> jobs;
};
#endif
