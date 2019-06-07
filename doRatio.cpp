void doRatio(const char* path = "20190423", const char* input = "result.csv", const char* outputfile = "result_1end.csv", const char* tester = "Xiaoning"){
	
	// code for get ratio of scintillation given the starting and end reference
	//Author: Xiaoning Wang, inspired by Anabel's Gaussian fit code
	
	ofstream file;
	file.open(Form("%s/temp_%s", path, input));
	
	string line;	
	ifstream inputfile;
	inputfile.open(Form("%s/%s", path, input));

 	std::string title;
	getline(inputfile,title);

	int n = 0;

    while (!inputfile.eof()) {
    	getline(inputfile,line);
    	if (line.length() == 0 || line == "") break;
    	else {;
    		file << line << endl;
    		}
    	n++;
    }
		
	TNtuple *nt= new TNtuple(input,input,"dbn:sci");
	nt->ReadFile(Form("%s/temp_%s", path, input),"dbn:sci",',');

	inputfile.close();
	file.close();

	float dbn,sci;
	nt->SetBranchAddress("dbn",&dbn);
	nt->SetBranchAddress("sci",&sci);

	ofstream outfile;
	outfile.open(outputfile);
	outfile << "DBN" << "," << "Scintillation (mV)" << "," << "Reference (mV)" << "," << "Ratio" << "," << "Tester" << "," << "Date" << endl;


	float cal, cal1, cal2, ratio;
	for (int i = 0; i < n; i++){
		nt -> GetEntry(i);
		if (dbn == 42000){
			cal1 = sci;
		}
		if (dbn == 42111){
			cal2 = sci;
		}
	}	
	cal = (cal1 + cal2)/2;

	for (int j = 0; j < n; j++){
		nt -> GetEntry(j);
		if (dbn == 42000 || dbn == 42111){
			continue;
		}
		else {
			ratio = sci/cal;
			outfile << dbn << "," << sci << "," << cal << "," << ratio << "," << tester << "," << path << endl;
		}
	}

	outfile.close();
}
