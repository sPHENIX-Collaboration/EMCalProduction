#include<ostream>
#include<string>
#include<fstream>

void getBetter2(const char* path = "pictures", const char* input = "result.csv", const char* outputfile = "result_1end.csv", const char* date = "20190426"){
	
	// code for get the better end of a block
	//Author: Xiaoning Wang, inspired by Anabel's Gaussian fit code
	
	ofstream file;
	file.open(Form("%s/temp_%s", path, input));
	
	string line;	ifstream inputfile;
	inputfile.open(Form("%s/%s", path, input));

 	std::string title;
	getline(inputfile,title);

	int st = 0;
	int ed = 0;

    while (!inputfile.eof()) {
    	getline(inputfile,line);
    	if (line.length() == 0 || line == "") continue;
    	else {
    		st = line.find(",");
    		if (line.substr(st + 1,1) == (string)"W"){
    			ed = 0;
    		} else {
    			ed = 1;
    		}
    		file << line.substr(0,st + 1) << ed << line.substr(st + 2, line.length() - (st + 1)) << endl;
    		}
    }
		
	TNtuple *nt= new TNtuple(input,input,"dbn:End:counts:percent:good:ok:bad:rms:t1:t2:t3:t4");
	nt->ReadFile(Form("%s/temp_%s", path, input),"dbn:End:counts:percent:good:ok:bad:rms:t1:t2:t3:t4",',');

	int n = nt->GetEntries();
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
	outfile << "DBN" << "," << "# of Fibers" << "," << "Good End" << "," << "Abs Difference" << "," << "Fiber (%)" << "," << "10-15 (%)" << "," << "15-50 (%)" << "," << "50-75 (%)" << "," << "r75(%)" << "," << "RMS" << "," << "Tower 1 (%)" << "," << "Tower 2 (%)" << "," << "Tower 3 (%)" << "," << "Tower 4 (%)" << "," << "Date" << endl;

	float perN, perW, diff;
	std::vector<int> DBN;
	for (int i = 0; i < n; i++){
		nt -> GetEntry(i);
		//cout << i << " loop" << ": " << "DBN size: " << DBN.size() << endl; 
		bool newBlock = true;
		for (int j = 0; j < DBN.size(); j++){
			//cout << "DBN[j]: " << DBN[j] << " dbn: " << dbn << endl;
			if (dbn == DBN[j]){
				newBlock = false;
			}
		}
		if (newBlock){
			DBN.push_back(dbn);
		}
	}
	int nn, nw;
	for (int k = 0; k < (int)n/2; k++){
		for (int l = 0; l < n; l++){
			nt -> GetEntry(l);
			if (dbn == DBN[k]){
				if (End == 1){
					perN = percent;
					nn = l;
				} else {
				if (End == 0){
					perW = percent;
					nw = l;
					}
				}
			}
		}
		diff = std::abs(perN - perW);
		if (perW >= perN) {
			nt -> GetEntry(nw);
			outfile << dbn << "," << counts << "," << "W" << "," << diff << "," << percent << "," << bad << "," << ok << "," << good << "," << (double)(good + ok + bad) << "," << (double)rms/100 << "," << t1 << "," << t2 << "," << t3 << "," << t4 << "," << date << endl;
		} else {
			nt -> GetEntry(nn);
			outfile << dbn << "," << counts << "," << "N" << "," << diff << "," << percent << "," << bad << "," << ok << "," << good << "," << (double)(good + ok + bad) << "," << (double)rms/100 << "," << t1 << "," << t2 << "," << t3 << "," << t4 << "," << date << endl;
		}
	}	

	outfile.close();
}
