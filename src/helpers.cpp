#include <helpers.hpp>

float norm_random(){
	return ((double) rand() / (RAND_MAX));
}

int calculate_max_time(vector<Project::job_t> *jobs, int starting){
	int max_time = starting;
	//Calculating Minimum time
	for(unsigned i = 0; i < (*jobs).size(); i++){
		for(unsigned j = 0; j < (*jobs)[i].num_operations ; j++){
			for(unsigned k = 0; k < (*jobs)[i].operations[j].tw_size ; k++ ){
				if((*jobs)[i].operations[j].time_windows[k].end_time > max_time){
					max_time = (*jobs)[i].operations[j].time_windows[k].end_time;
				}
			}
		}
			
	}
	return max_time;
}

int calculate_min_time(vector<Project::job_t> *jobs, int starting){
	int min_time = starting;
	//Calculating Minimum time
	for(unsigned i = 0; i < (*jobs).size(); i++){
		for(unsigned j = 0; j < (*jobs)[i].num_operations ; j++){
			for(unsigned k = 0; k < (*jobs)[i].operations[j].tw_size ; k++ ){
				if((*jobs)[i].operations[j].time_windows[k].start_time < min_time){
					min_time = (*jobs)[i].operations[j].time_windows[k].start_time;
				}
			}
		}
			
	}
	return min_time;
}

int calculate_min_weight(vector<Project::job_t> *jobs, int starting){
	unsigned min_weight = starting;
	for(unsigned i = 0; i < (*jobs).size(); i++){
		if((*jobs)[i].weight < min_weight){
			min_weight = (*jobs)[i].weight;
		}
	}
	return min_weight;
}

void initialize_solutions(solution_t **solutions, int ant_number, int scheduling_time){
	(*solutions) = new solution_t[ant_number];
	for(int i = 0; i < ant_number; i++){
		(*solutions)[i].jobs_completed.begin();
		(*solutions)[i].weights.begin();
		(*solutions)[i].total_weight = 0;
		(*solutions)[i].schedule = new job_operation[scheduling_time];
		for(int j =0; j < scheduling_time; j++){
			(*solutions)[i].schedule[j].job_id = 0;
			(*solutions)[i].schedule[j].operation_id = 0;
		}
	}
}


void initialize_feromone(trace_t **feromone, vector<Project::job_t> *jobs, int scheduling_time, int min_weight,  int min_time, float starting_feromone){
	(*feromone) = new trace_t[scheduling_time];

	for(int h = 0; h < scheduling_time; h++){
		(*feromone)[h].fo_size = 1;
		job_operation_fer tmp;
		tmp.jo.job_id = 0;
		tmp.jo.operation_id = 0;
		tmp.fer_amount = starting_feromone;
		tmp.job_weight = 1;
		(*feromone)[h].feasible_operation.push_back(tmp);

		for(unsigned i = 0; i < (*jobs).size(); i++){
			for(unsigned j = 0; j < (*jobs)[i].num_operations ; j++){
				for(unsigned k = 0; k < (*jobs)[i].operations[j].tw_size ; k++ ){
					if((*jobs)[i].operations[j].time_windows[k].start_time == (min_time + h)){
						job_operation_fer tmp;
						tmp.jo.job_id = i+1;
						tmp.jo.operation_id = j+1;
						tmp.fer_amount = starting_feromone;
						tmp.job_weight = (*jobs)[i].weight;
						(*feromone)[h].feasible_operation.push_back(tmp);
						(*feromone)[h].fo_size++;
					}
				}
			}
				
		}		
	}
}
