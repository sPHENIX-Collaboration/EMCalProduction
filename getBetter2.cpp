void getBetter2(const char* input = "result.csv", const char* outputfile = "result_1end.csv"){
	
	// code for get the better end of a block
	//Author: Xiaoning Wang, inspired by Anabel's Gaussian fit code
	
	ofstream file;
	file.open(Form("temp_%s",input));
	
	string line;	ifstream inputfile;
	inputfile.open(input);

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
		
	TNtuple *nt= new TNtuple(input,input,"dbn:End:counts:percent:good:ok:bad:rms");
	nt->ReadFile(Form("temp_%s",input),"dbn:End:counts:percent:good:ok:bad:rms",',');

	inputfile.close();
	file.close();

	float dbn,End,counts,percent,good,ok,bad,rms;
	nt->SetBranchAddress("dbn",&dbn);
	nt->SetBranchAddress("End",&End);
	nt->SetBranchAddress("counts",&counts);
	nt->SetBranchAddress("percent",&percent);
	nt->SetBranchAddress("good",&good);
	nt->SetBranchAddress("ok",&ok);
	nt->SetBranchAddress("bad",&bad);
	nt->SetBranchAddress("rms",&rms);

	nt -> GetEntry(0);
	cout << counts << endl;

	ofstream outfile;
	outfile.open(outputfile);
	outfile << "Difference" << "," << title << endl;

	float perN, perW, diff;
	for (int i = 0; i <= (int)(n/2); i++){
		nt -> GetEntry(2 * i);
		perN = percent;
		nt -> GetEntry(2 * i + 1);
		perW = percent;
		diff = std::abs(perN - perW);
		outfile << diff << ",";
		if (perW >= perN) {
			nt -> GetEntry(2 * i + 1);
			outfile << dbn << "," << "W" << "," << counts << "," << percent << "," << good << "," << ok << "," << bad << "," << rms << endl;
		} else {
			nt -> GetEntry(2 * i);
			outfile << dbn << "," << "N" << "," << counts << "," << percent << "," << good << "," << ok << "," << bad << "," << rms << endl;
		}
	}	

	outfile.close();
}
