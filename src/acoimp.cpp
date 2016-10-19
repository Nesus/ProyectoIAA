#include <acoimp.hpp>


void update_probability(trace_t **feromone, int scheduling_time, float alpha, float beta){
	for(int h = 0; h < scheduling_time; h++){
		float total_node = 0.0;
		for(int i = 0; i < (*feromone)[h].fo_size; i++){
			float tmp;
			tmp = pow((*feromone)[h].feasible_operation[i].fer_amount, alpha) * pow((*feromone)[h].feasible_operation[i].job_weight, beta);
			total_node += tmp;
			(*feromone)[h].feasible_operation[i].prob = tmp;
		}
		for(int i = 0; i < (*feromone)[h].fo_size; i++){
			(*feromone)[h].feasible_operation[i].prob = (*feromone)[h].feasible_operation[i].prob/total_node;
		}
	}
}

void aco(trace_t **feromone,solution_t **solutions,solution_t *best_solution, vector<Project::job_t> *jobs ,aco_parameters *params){
	int ant_number = params -> ant_number;
	int scheduling_time = params -> scheduling_time;
	int min_time  = params -> min_time;
	float alpha = params -> alpha;
	float beta = params -> beta;
	float evaporation = params -> evaporation;
	int feromone_add = params -> feromone_add;
	int max_iter = params -> max_iter;

	int step = scheduling_time/ant_number;
	vector<int> ant_start;
	for(int ant = 0; ant < ant_number; ant++){
		ant_start.push_back(ant*step);
	}

	solution_t best_iter_solution;
	best_iter_solution.schedule = new job_operation[scheduling_time]; 

	//Solution Generation
	for(int iter = 0; iter < max_iter; iter++){
		//Ant construction

		//cout << "============================================== ITERATION " << iter << " ===============================================\n"; 	
		
		for(int ant = 0; ant < ant_number; ant++){

			//Creating array of unused jobs
			unsigned *unused_jobs;
			int tmp_size = (*jobs).size();
			unused_jobs = new unsigned[tmp_size];
			for(unsigned i = 0; i < (*jobs).size(); i++){
				unused_jobs[i] = 0;
			}

			//Going through solution feromone
			for(int index = 0; index < scheduling_time; index++){
				int t = (ant_start[ant] + index) % scheduling_time; 
				//Look if there is a job that can be added
				if((*solutions)[ant].schedule[t].job_id == 0){
					for(int feasible_job = 0; feasible_job < (*feromone)[t].fo_size; feasible_job++ ){
						
						//Using probability to assign job
						float random = norm_random();
						float probability = (*feromone)[t].feasible_operation[feasible_job].prob;
						int tmp_job = (*feromone)[t].feasible_operation[feasible_job].jo.job_id - 1;
						int tmp_operation = (*feromone)[t].feasible_operation[feasible_job].jo.operation_id; 
						if( random< probability && tmp_job+1 != 0 && unused_jobs[tmp_job] == 0){
							
							Project::job_t job = (*jobs)[tmp_job];

							unsigned total_inserted = 0;
							//Tryingo to assing all the operations in the first slot available
							for(unsigned operation_id = 0; operation_id < job.num_operations; operation_id++){
								Project::operation_t operation = job.operations[operation_id];
								
								//Try to add every operation
								unsigned long processing_time = operation.processing_time;
								for(unsigned tw = 0 ; tw < operation.tw_size; tw++){
									Project::timeWindows_t timewindow = operation.time_windows[tw];

									unsigned inserted = 0;
									unsigned start_index = timewindow.start_time - min_time;
									unsigned flag = 0;

									if((unsigned) tmp_operation == operation.operation_id && start_index !=(unsigned) t){
										continue;
									}

									//Checking if the operation can be inserted
									for(unsigned time = start_index; time < start_index + processing_time; time++){

										if((*solutions)[ant].schedule[time].job_id != 0){
											flag = 1;
											break;
										}
									}
									if(flag == 0){
										inserted = 1;
										for(unsigned time = start_index; time < start_index + processing_time; time++){
											(*solutions)[ant].schedule[time].job_id = tmp_job + 1;
											(*solutions)[ant].schedule[time].operation_id = operation_id;
										}									
									}
									if(inserted == 1){
										total_inserted += 1;
										break;
									}
								}

							}
							//If all operations inserted add to jobs completed
							if(total_inserted == job.num_operations){
								unused_jobs[tmp_job] = 1;
								(*solutions)[ant].jobs_completed.push_back(tmp_job);
								int weight = job.weight;
								(*solutions)[ant].weights.push_back(weight);
								(*solutions)[ant].total_weight += weight;
							}
							//Remove not completed job operations
							else{
								for(int time = 0; time < scheduling_time; time ++){
									if((*solutions)[ant].schedule[time].job_id == tmp_job + 1){
										(*solutions)[ant].schedule[time].job_id = 0;
										(*solutions)[ant].schedule[time].operation_id = 0; 
									}
								}

							}
						}
					}
				}
			}
			delete [] unused_jobs;
 		}
		//debugPrint(solutions,ant_number,scheduling_time);

		getBestSolution(solutions, &best_iter_solution, best_solution, ant_number,scheduling_time);
		updateFeromone(feromone, best_solution, alpha,beta,evaporation,feromone_add, scheduling_time);
		resetSolutions(solutions, ant_number,scheduling_time);
	}

}


void getBestSolution(solution_t **solutions,solution_t *best_iter_solution, solution_t *best_solution, int ant_number, int scheduling_time){
	int max_gain = 0;
	int max_index = 0;
	for(int ant = 0; ant < ant_number; ant++){
		if((*solutions)[ant].total_weight > max_gain){
			max_gain = (*solutions)[ant].total_weight;
			max_index = ant;
		}
	}

	//Copying best_solution
	(*best_iter_solution).total_weight = (*solutions)[max_index].total_weight;
	(*best_iter_solution).jobs_completed = vector<int>((*solutions)[max_index].jobs_completed);
	(*best_iter_solution).weights= vector<int>((*solutions)[max_index].weights);
	for(int time = 0; time < scheduling_time; time++){
		(*best_iter_solution).schedule[time].job_id = (*solutions)[max_index].schedule[time].job_id;
		(*best_iter_solution).schedule[time].operation_id = (*solutions)[max_index].schedule[time].operation_id;
	}


	if((*best_iter_solution).total_weight > (*best_solution).total_weight){
		(*best_solution).total_weight = (*solutions)[max_index].total_weight;
		(*best_solution).jobs_completed = vector<int>((*solutions)[max_index].jobs_completed);
		(*best_solution).weights= vector<int>((*solutions)[max_index].weights);
		for(int time = 0; time < scheduling_time; time++){
			(*best_solution).schedule[time].job_id = (*solutions)[max_index].schedule[time].job_id;
			(*best_solution).schedule[time].operation_id = (*solutions)[max_index].schedule[time].operation_id;
		}		
	}

}

void updateFeromone(trace_t **feromone, solution_t *best_solution, float alpha, float beta, float evaporation,float feromone_add,int scheduling_time){
	for(int time = 0; time < scheduling_time; time++){
		int job_id = (*best_solution).schedule[time].job_id;
		int operation_id = (*best_solution).schedule[time].operation_id;

		for(int j = 0; j < (*feromone)[time].fo_size; j++){
			double result = (double) (*feromone)[time].feasible_operation[j].fer_amount*(double)evaporation;
			(*feromone)[time].feasible_operation[j].fer_amount = result;

			if((*feromone)[time].feasible_operation[j].jo.job_id == job_id && (*feromone)[time].feasible_operation[j].jo.operation_id == operation_id ){
				(*feromone)[time].feasible_operation[j].fer_amount =(*feromone)[time].feasible_operation[j].fer_amount + feromone_add;
			}
		}
	}
	update_probability(feromone,scheduling_time,alpha,beta);
}


void resetSolutions(solution_t **solutions, int ant_number, int scheduling_time){
	for(int ant = 0; ant < ant_number; ant++){
		(*solutions)[ant].jobs_completed.clear();
		(*solutions)[ant].weights.clear();
		(*solutions)[ant].total_weight = 0;

		for(int time = 0; time < scheduling_time; time++){
			(*solutions)[ant].schedule[time].job_id = 0;
			(*solutions)[ant].schedule[time].operation_id = 0;
		}

	}


}


void debugPrint(solution_t **solutions, int ant_number, int scheduling_time){
	for(int j=0; j < ant_number; j++){
		/*for(int i= 0; i < scheduling_time; i++){
			cout << (*solutions)[j].schedule[i].job_id << " ";
		}*/
		cout << (*solutions)[j].jobs_completed.size();
		cout << '\n';
		cout << "Total weight: " << (*solutions)[j].total_weight <<'\n';
	}
}