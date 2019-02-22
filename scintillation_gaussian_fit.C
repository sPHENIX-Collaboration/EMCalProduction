void scintillation_gaussian_fit(int dbn = 42, const char* folder = "20190131"){
	
	// code for fitting a gaussian to the data of the txt file of the scintillation test
	
	ofstream file;
	file.open (Form("%s/DBN_%d.txt",folder,dbn));
	
	string line;	ifstream inputfile;
	inputfile.open(Form("%s/dbn%d.txt",folder,dbn));

     while (!inputfile.eof()) {
     getline(inputfile,line);

     if (line.length() == 0 || line[0] == 'P') continue;
     else file << line << endl;
     }
     
    inputfile.close();
   	file.close();
		
	TNtuple *nt= new TNtuple(Form("nt_%d",dbn),Form("nt_%d",dbn),"time1:time2:volt");
	nt->ReadFile(Form("%s/DBN_%d.txt",folder,dbn),"time1:time2:volt",'	');

	float time1,time2,volt;
	nt->SetBranchAddress("time1",&time1);
	nt->SetBranchAddress("time2",&time2);
	nt->SetBranchAddress("volt",&volt);
		
	int n = nt->GetEntries();
	nt->GetEntry(0); float min=time1;
	nt->GetEntry(n-2); float max=time2;
		
	TH1F *h = new TH1F(Form("pulseheight_DBN_%d",dbn),Form("DBN %d;Pulse Height (mV);Counts",dbn),n-1,min,max);
	h->GetXaxis()->SetRangeUser(150,490);
	TF1 *f = new TF1(Form("f_%d",dbn),"gaus",180,490);
	//f = new TF1(Form("f_%d",dbn),"gaus",230,450);

	for (int k=0;k<=n;k++) {
		nt->GetEntry(k);
		h->SetBinContent(k,volt);
		//h->SetBinError(k,volt*0.1);
	}
	TCanvas *c = new TCanvas("c","c",500,500);

	h->Fit(f,"RQ");
	
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1);
	

	h->Draw();
	
	cout << "DBN = " << dbn << " Scintillation (mV) = " << f->GetParameter(1) << endl;


}
