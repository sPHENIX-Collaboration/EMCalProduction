void getBetter2(const char* path = "pictures", const char* input = "result.csv", const char* outputfile = "result_1end.csv"){
	
	// code for get the better end of a block
	//Author: Xiaoning Wang, inspired by Anabel's Gaussian fit code
	
	ofstream file;
	file.open(Form("%s/temp_%s", path, input));
	
	string line;	ifstream inputfile;
	inputfile.open(Form("%s/%s", path, input));

 	std::string title;
	getline(inputfile,title);

	int n = 0;
	int st = 0;

    while (!inputfile.eof()) {
    	getline(inputfile,line);
    	if (line.length() == 0 || line == "") continue;
    	else {
    		st = line.find(",");
    		file << line.substr(0,st + 1) << n % 2 << line.substr(st + 2, line.length() - (st + 1)) << endl;
    		}
    	n++;
    }
		
	TNtuple *nt= new TNtuple(input,input,"dbn:End:counts:percent:good:ok:bad:rms:t1:t2:t3:t4");
	nt->ReadFile(Form("%s/temp_%s", path, input),"dbn:End:counts:percent:good:ok:bad:rms:t1:t2:t3:t4",',');

	inputfile.close();
	file.close();

	float dbn,End,counts,percent,good,ok,bad,rms,t1,t2,t3,t4;
	nt->SetBranchAddress("dbn",&dbn);
	nt->SetBranchAddress("End",&End);
	nt->SetBranchAddress("counts",&counts);
	nt->SetBranchAddress("percent",&percent);
	nt->SetBranchAddress("good",&good);
	nt->SetBranchAddress("ok",&ok);
	nt->SetBranchAddress("bad",&bad);
	nt->SetBranchAddress("rms",&rms);
	nt->SetBranchAddress("t1",&t1);
	nt->SetBranchAddress("t2",&t2);
	nt->SetBranchAddress("t3",&t3);
	nt->SetBranchAddress("t4",&t4);


	ofstream outfile;
	outfile.open(outputfile);
	outfile << "DBN" << "," << "# of Fibers" << "," << "Good End" << "," << "Abs Difference" << "," << "Fiber (%)" << "," << "10-15 (%)" << "," << "15-50 (%)" << "," << "50-75 (%)" << "," << "r75(%)" << "," << "RMS" << "," << "Tower 1 (%)" << "," << "Tower 2 (%)" << "," << "Tower 3 (%)" << "," << "Tower 4 (%)" << endl;

	float perN, perW, diff;
	for (int i = 0; i < (int)(n/2); i++){
		nt -> GetEntry(2 * i);
		perN = percent;
		nt -> GetEntry(2 * i + 1);
		perW = percent;
		diff = std::abs(perN - perW);
		if (perW >= perN) {
			nt -> GetEntry(2 * i + 1);
			outfile << dbn << "," << counts << "," << "W" << "," << diff << "," << percent << "," << bad << "," << ok << "," << good << "," << (double)(good + ok + bad) << "," << (double)rms/100 << "," << t1 << "," << t2 << "," << t3 << "," << t4 << endl;
		} else {
			nt -> GetEntry(2 * i);
			outfile << dbn << "," << counts << "," << "N" << "," << diff << "," << percent << "," << bad << "," << ok << "," << good << "," << (double)(good + ok + bad) << "," << (double)rms/100 << "," << t1 << "," << t2 << "," << t3 << "," << t4 << endl;
		}
	}	

	outfile.close();
}
