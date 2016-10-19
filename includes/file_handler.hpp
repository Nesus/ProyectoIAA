#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <project.hpp>

using namespace std;

class FileHandler{
private:
	string input_path;
	ifstream input_file;
	string output_path;
	ofstream output_file;
public:
	struct outputFields_t{
		string filename;
		string model;
		bool is_optimal;
		double exec_time;
		long upper_bound;
		long lower_bound;
		long nodes_num;
		
		bool linear_relaxation;
		double relaxation_optimal;
	};
	
	FileHandler(const string &input_path,const string &output_path);
	virtual ~FileHandler();
	
	bool isJob(string line);
	bool isOperation(string line);
	
	void getGeneralData(unsigned long *num_projects, unsigned long *planification_time);
	void getJobData(string line, unsigned long *job_id,unsigned long *num_operations, unsigned *weight);
	void getOperationData(string line, unsigned long *operation_id,unsigned long *completion_time, unsigned long *tw_size);
	
	vector<Project::timeWindows_t> getTimeWindows(unsigned long tw_size);
	Project::predecessor_t getPredecessors();
		
	void testResultingObject(Project *project);	
	
	Project * read();
	void writeOutput(outputFields_t output);
};
#endif
