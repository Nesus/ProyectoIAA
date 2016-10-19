#include <file_handler.hpp>

FileHandler::FileHandler(const string &input_path, const string &output_path){
	this->input_path = input_path;
	this->output_path = output_path;
}

FileHandler::~FileHandler(){
	
}

/*--------------------------------------
	Main Function (the one that parses)
---------------------------------------*/
Project * FileHandler::read() {
	string line;
	stringstream ss;
	unsigned weight;										
	unsigned long num_jobs,
				  			planification_time,			// 
	              job_id,									// 
	              num_operations,					// How many ops a job has.
								operation_id,						// Operation j
								completion_time,
				  			tw_size;	  
								
	this->input_file.open(this->input_path.c_str(), ios::in);
	if (!this->input_file.is_open())
		perror("error while opening file");
	
	// get the General Data from the header of the instance
	// file and initialize a new project using those parameters.
	getGeneralData(&num_jobs, &planification_time);						
	Project *project = new Project(num_jobs, planification_time);	

	// Read the file from beginning to the end
	// and get every specific job information.
	while (getline(this->input_file, line)) {
		if (isJob(line)) {
			// Get Job general information and
			// add it to the "project" object
			getJobData(line, &job_id, &num_operations, &weight);
			project->appendJob(job_id, num_operations, weight);
			
		} else if (isOperation(line)) {
			// Get Operation General Information 
			// and append it to last Job.
			getOperationData(line, &operation_id, &completion_time, &tw_size);
			project->appendOperation(operation_id, completion_time, tw_size);

			getline(this->input_file, line); // skip - TimeWindows -
			
			// get TimeWindows and append it to
			// the last Operation of the last Job.
			project->appendTimeWindows(getTimeWindows(tw_size)); 
			// get precedence list and append it to.
			project->appendPredecessors(getPredecessors());	
			} else
					continue;
	}

	this->input_file.close();
	
	// only for debugging (prints the resulting object).
	#ifdef DEBUG
	this->testResultingObject(project);
	#endif
	
	return project;
}


/*--------------------------------------
					Secondary Functions
---------------------------------------*/
void FileHandler::getGeneralData(unsigned long *num_jobs, unsigned long *planification_time){
	string line;
	stringstream ss;
	getline(this->input_file,line);
	getline(this->input_file,line);
	
	ss << line;			
	ss >> (*num_jobs) >> (*planification_time);
}

void FileHandler::getJobData(string line, unsigned long *job_id, unsigned long *num_operations,unsigned *weight){
	stringstream ss;
	
	// Get specific Job identifier
	// inside the Job Header:
	// --- Job %d ---
	boost::regex digit_regex("\\d+");
	boost::smatch matches;   
	boost::regex_search(line, matches, digit_regex);
	(*job_id) = boost::lexical_cast<unsigned long>(string(matches[0]));
	
	// Get Job Parameters.
	getline(this->input_file,line);
	ss << line;
	
	ss >> (*num_operations) >> (*weight);
}

void FileHandler::getOperationData(string line, unsigned long *operation_id, unsigned long *completion_time, unsigned long *tw_size){
	stringstream ss;

	// Get specific Operation identifier
	// inside the Operation Header:
	// -- Operation %d --	
	boost::regex digit_regex("\\d+");
	boost::smatch matches;   
	boost::regex_search(line, matches, digit_regex);
	(*operation_id) = boost::lexical_cast<unsigned long>(string(matches[0]));
		
	getline(this->input_file,line);
	ss << line;

	ss >> (*completion_time) >> (*tw_size);
}

vector<Project::timeWindows_t> FileHandler::getTimeWindows(unsigned long tw_size){
	string line;
	
	vector<Project::timeWindows_t> timeWindows;
	Project::timeWindows_t tw;

	for (unsigned long int i=0; i<tw_size; i++) {
		stringstream ss;
		getline(this->input_file,line);

		ss << line;
		ss >> tw.start_time >> tw.end_time;
		
		timeWindows.push_back(tw);
	}
	
	return timeWindows;
}

Project::predecessor_t FileHandler::getPredecessors(){
	unsigned pred_i, num_predecessors;
	string line;
	stringstream ss;
	Project::predecessor_t predecessor;
	
	getline(this->input_file,line); // skip - predecessors - 
	getline(this->input_file,line); // get num_predecessors
	ss << line;
	ss >> num_predecessors;
	predecessor.num_predecessors=num_predecessors;
	
	getline(this->input_file,line); // get predecessor list
	ss.clear();
	ss << line;
	while(ss >> pred_i){
		predecessor.predecessor_list.push_back(pred_i);
	}
	return predecessor;
}



/*--------------------------------------
					Section Checkers
---------------------------------------*/
bool FileHandler::isJob(string line){
	boost::regex regexp("--- \\w+ \\d+ ---");
	return boost::regex_match(line, regexp);
}
bool FileHandler::isOperation(string line){
	boost::regex regexp("-- \\w+ \\d+ --");
	return boost::regex_match(line, regexp);
}


/*--------------------------------------
						Write Output
---------------------------------------*/
string getFilename(const string &str){
	return str.substr(str.find_last_of("/\\")+1);
}

bool fileNotExists(const string &filepath){
	ifstream file;
	file.open(filepath.c_str());
	return !file || file.peek() == ifstream::traits_type::eof();	
}

void FileHandler::writeOutput(FileHandler::outputFields_t output){
	output.filename = getFilename(this->input_path);
	

	if(fileNotExists(output_path)){
		this->output_file.open(this->output_path.c_str());
		this->output_file << "Method,Instance,\"Optimal Status\",\"Execution Time\",\"Number Nodes\",\"Upper Bound\",\"Lower Bound\",\"Linear Relaxation Optimal Value\""<<endl;		
	}else{
		this->output_file.open(this->output_path.c_str(),ios::app);		
	}
	
	this->output_file << output.model << ","
										<< output.filename << ","
	 								 	<< output.is_optimal << ","
										<< output.exec_time << ","
										<< output.nodes_num << ",";
	
	if(output.linear_relaxation){
		this->output_file << "-" << ","
											<< "-" << ","
											<< output.relaxation_optimal << endl; 
	}else{
		this->output_file << output.lower_bound << ","
											<< output.upper_bound << ","
											<< "-" << endl; 
	}
		
	this->output_file.close();							
}


/*--------------------------------------
						Debugging
---------------------------------------*/

void FileHandler::testResultingObject(Project *project){
	cout << "---- General data ----" <<endl;
	cout << project->getNumJobs() << " " << project->getPlanificationTime() <<endl;		
	vector<Project::job_t> jobs=project->getJobs();

	for(vector<Project::job_t>::iterator it=jobs.begin(); it!=jobs.end(); ++it){
		cout << "--- Job " << (*it).job_id <<" ---" << endl;
		cout << (*it).num_operations << " " << (*it).weight << endl;
			
		for(unsigned i=0; i< (*it).num_operations; i++){
			cout << "-- Operation "  << (*it).operations[i].operation_id << " --" << endl;
			cout << (*it).operations[i].processing_time << " " << (*it).operations[i].tw_size << endl;
			
			cout << "- time windows -" << endl;
			for(unsigned j=0;j<(*it).operations[i].tw_size;j++){
				cout << (*it).operations[i].time_windows[j].start_time << " " << (*it).operations[i].time_windows[j].end_time << endl;
			}
	
			cout << "- predecessors -" << endl;
			cout << (*it).operations[i].predecessors.num_predecessors << endl;
			for(unsigned j=0; j<(*it).operations[i].predecessors.num_predecessors; j++){
				cout << (*it).operations[i].predecessors.predecessor_list[j] << " ";
			}
			cout << endl;
			
		}
	}
}