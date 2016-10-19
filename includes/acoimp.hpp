#ifndef aco_H
#define aco_H

#include <project.hpp>
#include <representation.hpp>
#include <helpers.hpp>
#include <iostream>
#include <algorithm> 


using namespace std;

void update_probability(trace_t **feromone, int scheduling_time, float alpha, float beta);

void aco(trace_t **feromone,solution_t **solutions,solution_t *best_solution, vector<Project::job_t> *jobs ,aco_parameters *params);

void getBestSolution(solution_t **solutions,solution_t *best_iter_solution, solution_t *best_solution, int ant_number, int scheduling_time);

void updateFeromone(trace_t **feromone, solution_t *best_solution, float alpha, float beta, float evaporation,float feromone_add,int scheduling_time);

void resetSolutions(solution_t **solutions, int ant_number, int scheduling_time);


void debugPrint(solution_t **solutions, int ant_number, int scheduling_time);

#endif