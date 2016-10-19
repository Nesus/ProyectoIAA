#ifndef representation_H
#define representation_H

#include <iostream>
#include <vector>
#include <string>

struct job_operation
{
	int job_id;
	int operation_id;
	
};

struct job_operation_fer{
	job_operation jo;
	double fer_amount;
	float job_weight;
	float prob;
};


struct solution_t{
	job_operation *schedule;
	vector<int> jobs_completed;
	vector<int> weights;
	int total_weight;
};


struct trace_t{
	vector<job_operation_fer> feasible_operation;
	int fo_size;
};

struct aco_parameters{
	int ant_number;
	int scheduling_time;
	int min_time;
	int min_weight;
	float alpha;
	float beta;
	float evaporation;
	float feromone_add;
	int max_iter;
};

#endif