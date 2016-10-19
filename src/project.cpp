#include <project.hpp>

Project::Project(const unsigned num_jobs,const unsigned planification_time){
	this->num_jobs = num_jobs;
	this->planification_time = planification_time;
}

Project::~Project(){
	
}

void Project::appendJob(unsigned job_id,unsigned num_operations, unsigned weight){
	Project::job_t new_job;
	
	new_job.job_id = job_id;
	new_job.num_operations = num_operations;
	new_job.weight = weight;
	
	(this->jobs).push_back(new_job);
}

void Project::appendOperation(unsigned long operation_id, unsigned long processing_time, unsigned long tw_size){
	Project::operation_t new_operation;
	
	new_operation.operation_id = operation_id;
	new_operation.processing_time = processing_time;
	new_operation.tw_size = tw_size;

	(this->jobs).back().operations.push_back(new_operation);			
}

void Project::appendTimeWindows(vector<Project::timeWindows_t> time_windows){	
	((this->jobs.back()).operations.back()).time_windows = vector<Project::timeWindows_t>(time_windows.begin(),
																																												time_windows.end());
}

void Project::appendPredecessors(Project::predecessor_t predecessors){
	((this->jobs.back()).operations.back()).predecessors.num_predecessors = predecessors.num_predecessors;
	((this->jobs.back()).operations.back()).predecessors.predecessor_list = vector<unsigned>(predecessors.predecessor_list.begin(),
																																													 predecessors.predecessor_list.end());
}	


unsigned Project::getNumJobs(){
	return this->num_jobs;
}
unsigned Project::getPlanificationTime(){
	return this->planification_time;
}
vector<Project::job_t> Project::getJobs(){
	return this->jobs;
}
