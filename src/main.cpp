#include <file_handler.hpp>
#include <project.hpp>

#include <iostream>

#include <representation.hpp>
#include <helpers.hpp>
#include <acoimp.hpp>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[]){
	int SEED = 12345;

	srand(SEED);
    
	//Input and output path
	string input_path = argv[1];
	string output_path = argv[2];

	float starting_feromone = atof(argv[3]);

	float alpha = atof(argv[4]);
	float beta = atof(argv[5]);
	float evaporation =(float) (1.0 - atof(argv[6]));
	float feromone_add = atof(argv[7]);

	int ant_number = atoi(argv[8]);
	int max_iter = atoi(argv[9]);

	//cout << "ACS Parameters:\n";
	//cout << "starting_feromone: " << starting_feromone << " alpha: "<< alpha << " beta: " << beta << " evaporation: " << evaporation;
	//cout << " feromone_add: " << feromone_add << '\n' << "ant_number: "<< ant_number << " max_iter: " << max_iter;

	//Reading and parsing file
	FileHandler fh(input_path, output_path);
	Project *projects;
	projects = fh.read();
	vector<Project::job_t> jobs =  projects->getJobs(); 


	//Other variables
	int min_time;
	int max_time;
	min_time = calculate_min_time(&jobs, projects-> getPlanificationTime());
	max_time = calculate_max_time(&jobs, 0);
	trace_t *feromone;
	solution_t *solutions;
	solution_t best_solution;

	aco_parameters params;

	int min_weight; 
	min_weight = calculate_min_weight(&jobs,10000000);
	int scheduling_time = max_time- min_time;

	//cout << " Planification Horizon: "<< projects -> getPlanificationTime(); 
	//cout << " Minimium Starting Time: " << min_time;
	//cout << " Total Scheduling Time: " << scheduling_time << '\n';

	//cout << "Min Job Weight: " << min_weight << '\n';



	best_solution.schedule = new job_operation[scheduling_time];
	best_solution.total_weight = 0;
	for(int j =0; j < scheduling_time; j++){
		best_solution.schedule[j].job_id = 0;
		best_solution.schedule[j].operation_id = 0;
	}	

	params.ant_number = ant_number;
	params.scheduling_time= scheduling_time;
	params.min_time = min_time;
	params.min_weight = min_weight;
	params.alpha = alpha;
	params.beta = beta;
	params.max_iter = max_iter;
	params.feromone_add = feromone_add;
	params.evaporation = evaporation;


	initialize_solutions(&solutions,ant_number,scheduling_time);
	initialize_feromone(&feromone,&jobs,scheduling_time,min_weight,min_time,starting_feromone);
	update_probability(&feromone, scheduling_time,alpha,beta);
	aco(&feromone, &solutions, &best_solution,&jobs, &params);


	//cout << "Best Solution: " << '\n';
	cout << input_path << ',' << best_solution.jobs_completed.size() << "," << best_solution.total_weight<< '\n';   

	int used_slot = 0;
	float used_per = 0.0;
	for(int i= 0; i < scheduling_time; i++){
			if(best_solution.schedule[i].job_id != 0){
				used_slot += 1;
			}
	}
	used_per = (float) used_slot / (float)scheduling_time;
	//cout << "Used %: " << used_per << '\n';

	int max_weight = 0;

	for(int  i = 0; i < jobs.size(); i++){
		max_weight += jobs[i].weight;
	}

	delete [] feromone;
	return 0;
}