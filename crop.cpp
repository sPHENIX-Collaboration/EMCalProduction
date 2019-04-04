void crop(const char* path = "IgnoreMe/test", const char* ed = "N", int dbn = 42, const char * output = "IgnoreMe/test"){
	
	// crop the picture
	//Author: Xiaoning Wang
	TString pic = Form("%s/DBN_%d-%s.JPG",path,dbn,ed);
  	TASImage* img = (TASImage*) TASImage::Open(pic);

  	int bins1 = 1;
  	int tol = 10;
  	int interv = 2500;
  	int startx = 1500;
  	int starty = 1000;

  	TH2D *g = new TH2D("Intensity2","Intensity2", interv, startx, startx + interv, interv, starty, starty + interv);
  	TH2D *rg = new TH2D("Intensityb", "Intensityb", interv/bins1, startx, startx + interv, interv/bins1, starty, starty + interv);
  	TH1D *hx = new TH1D("Intenx","Intenx", interv/bins1, startx, startx + interv);
  	TH1D *hy = new TH1D("Inteny","Inteny", interv/bins1, starty, starty + interv);

  	UInt_t *argb   = img -> GetArgbArray();

  	UInt_t yPixels = img -> GetHeight();
  	cout << yPixels << endl;
  	UInt_t xPixels = img -> GetWidth();
   	cout << xPixels << endl;


  	for (int col = 0; col < interv/bins1; ++col) {
   		for (int row = 0; row < interv/bins1; ++row) {
   			short totInt = 0;
   			for (int i = 0; i < bins1; ++i){
   				int index = (row * bins1 + starty) * xPixels + col * bins1 + startx + i;
      			short grey = argb[index]&0xff;
    			if (grey == 0) {
    			grey = 1;}
    			g -> SetBinContent(col * bins1 + i + 1, row * bins1 + i + 1, grey);
    			totInt = totInt + grey;
    		}
      		rg -> SetBinContent(col + 1, row + 1, totInt);
    	}
  	}


  	for (int i = 1; i <= interv; i++){
  		int Intenx = 0;
  		int Inteny = 0;
  		for (int j = 1; j <= interv; j++){
  			Intenx = Intenx + rg -> GetBinContent(i,j);
  			Inteny = Inteny + rg -> GetBinContent(j,i);
  		}
  		hx -> SetBinContent(i, Intenx);
  		hy -> SetBinContent(i, Inteny);
  	}
  	//TH2D* rg = (TH2D*)g -> Clone("rg");
  	//rg -> Rebin2D(bins1, bins1);
  	int maxx = hx -> GetMaximum();
  	cout << "Maximum x bin is " << maxx << "pixels" << endl;
  	int maxy = hy -> GetMaximum();
  	cout << "Maximum y bin is " << maxy << "pixels" << endl;

  	int limx = hx -> FindFirstBinAbove(0.5 * maxx) - 1;
  	int widthx = hx -> FindLastBinAbove(0.5 * maxx) - limx;

  	int limy = hy -> FindFirstBinAbove(0.5 * maxy) - 1;
  	int widthy = hy -> FindLastBinAbove(0.5 * maxy) - limy;

  	TFile* outfile = new TFile(Form("%s/DBN_%d-%s.root",output,dbn,ed),"RECREATE");
  	img -> SetName("original");
  	img -> Write();

  	img -> Crop(limx* bins1 + startx - tol, limy * bins1 + starty - tol, widthx * bins1 + 2 * tol, widthy * bins1 + 2 * tol);



  	//We select the outermost tile with intensity within first 20% of intensity, and 
  	//for (int x = 0; x < interv/bins1; ++x){
  	//	for (int y = 0; x < interv/bins1; ++y){
  	//		
  	//	}
  	//}


  	//TH2D* fg = (TH2D*)g -> Clone("fg");
  	TCanvas* c2 = new TCanvas("c2","", widthx * bins1 + 2 * tol, widthy * bins1 + 4 * tol);
  	c2 -> SetMargin(0,0,0,0);
	c2 -> cd();
	img -> Draw();
	c2 -> SaveAs(Form("%s/DBN_%d-%s.png",output,dbn,ed));

  	TCanvas* c0 = new TCanvas("c0","",700,700);
  	c0 -> Divide(2,2);
  	c0 -> cd(1);
  	gStyle -> SetPalette(55);
  	gStyle -> SetOptStat(0);
  	g -> SetAxisRange(0, 256, "z");
  	g -> Draw("colz");

  	c0 -> cd(2);
  	rg -> SetAxisRange(0, 256 * bins1, "z");
  	rg -> Draw("colz");

  	c0 -> cd(3);
  	hx -> Draw();

  	c0 -> cd(4);
  	hy -> Draw();
  	//c0 -> cd(2);
  	//gPad->SetLogy();	
  	//h -> Draw();

	img -> SetName("cropped");
	img -> Write();

	TCanvas* c1 = new TCanvas("c1","",1400,700);
	c1 -> Divide(2,1);
	c1 -> cd(1);
	TASImage* original = (TASImage*)outfile -> Get("original");
	original -> Draw();

	TASImage* cropped = (TASImage*)outfile -> Get("cropped");
	c1 -> cd(2);
	cropped -> Draw();

	outfile -> Close();
  }
