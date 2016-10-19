#ifndef helpers_H
#define helpers_H

#include <iostream>
#include <cmath>
#include <stdlib.h>

#include <vector>
#include <project.hpp>
#include <representation.hpp>

#include <typeinfo>

using namespace std;

float norm_random();

int calculate_min_time(vector<Project::job_t> *jobs, int starting);

int calculate_min_weight(vector<Project::job_t> *jobs, int starting);

void initialize_solutions(solution_t **solutions, int ant_number, int scheduling_time);

void initialize_feromone(trace_t **feromone, vector<Project::job_t> *jobs, int scheduling_time, int min_weight, int min_time, float starting_feromone);

int calculate_max_time(vector<Project::job_t> *jobs, int starting);

#endif