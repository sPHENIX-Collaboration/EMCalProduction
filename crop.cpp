//sPHENIX EMCal fiber counting image auto-crop software
//Author: Xiaoning Wang

#include <vector>
#include <TASImage.h>
#include <TEllipse.h>
#include <fstream>

void crop(int dbn = 42, const char* ed = "N", const char* path = "IgnoreMe/test", const char * outputRoot = "IgnoreMe/test", const char * output = "IgnoreMe/test"){
	
	// crop the picture
	//Author: Xiaoning Wang
	TString pic = Form("%s/DBN_%d-%s.JPG",path,dbn,ed);
  TASImage* img = (TASImage*) TASImage::Open(pic);
  gStyle->SetOptStat(0);

  int bins1 = 20;
  int interv = 2500;
  int startx = 1500;
	int starty = 1000; 
  double ratio1 = 0.5;  
  double ratio2 = 0.1;

  TH2D *g = new TH2D("Intensity","Intensity", interv, startx, startx + interv, interv, starty, starty + interv);
  TH2D *xg = new TH2D("IntensityX", "IntensityX", interv/bins1, startx, startx + interv, interv/bins1, starty, starty + interv);
  //TH2D *yg = new TH2D("IntensityY", "IntensityY", interv/bins1, starty, starty + interv, interv/bins1, startx, startx + interv);
  TH1D *hx = new TH1D("Intenx","Intenx", interv/bins1, startx, startx + interv);
  TH1D *hy = new TH1D("Inteny","Inteny", interv/bins1, starty, starty + interv);

  UInt_t *argb = img -> GetArgbArray();

  UInt_t yPixels = img -> GetHeight();
  //cout << "Height: " << yPixels << endl;
  UInt_t xPixels = img -> GetWidth();
  //cout << "Width: " << xPixels << endl;


  for (int col = 0; col < interv/bins1; ++col) {
   	for (int row = 0; row < interv/bins1; ++row) {
   		int totInt = 0;
   		for (int i = 0; i < bins1; ++i){
        for (int j = 0; j < bins1; ++j){
   			int index = (row * bins1 + starty + i) * xPixels + col * bins1 + startx + j;
        //cout << index << endl;
      	int grey = argb[index]&0xff;
    		if (grey == 0) {
    		  grey = 1;
        }
    		g -> SetBinContent(col * bins1 + j + 1, row * bins1 + i + 1, grey);
    		totInt = totInt + grey;
      }
    	}
      //cout << totInt << endl;
      xg -> SetBinContent(col + 1, row + 1, totInt);
      //yg -> SetBinContent(row + 1, col + 1, totInt);
    }
  }


  for (int i = 1; i <= interv; i++){
  	int Intenx = 0;
  	int Inteny = 0;
  	for (int j = 1; j <= interv; j++){
  		Intenx = Intenx + xg -> GetBinContent(i,j);
  		Inteny = Inteny + xg -> GetBinContent(j,i);
  	}
  	hx -> SetBinContent(i, Intenx);
  	hy -> SetBinContent(i, Inteny);
  }

  	//TH2D* rg = (TH2D*)g -> Clone("rg");
  	//rg -> Rebin2D(bins1, bins1);
  int maxx = hx -> GetMaximum();
  //cout << "Maximum x bin is " << maxx << " pixels" << endl;
  int maxy = hy -> GetMaximum();
  //cout << "Maximum y bin is " << maxy << " pixels" << endl;

  int limx = hx -> FindFirstBinAbove(ratio1 * maxx) - 1;
  int edx = hx -> FindLastBinAbove(ratio1 * maxx);

  int limy = hy -> FindFirstBinAbove(ratio1 * maxy) - 1;
  int edy = hy -> FindLastBinAbove(ratio1 * maxy);

  TH1D *fx = new TH1D("FineX", "FineX", (edx - limx + 3) * bins1, startx + (limx - 1.5) * bins1, startx + (edx + 1.5) * bins1);
  TH1D *fy = new TH1D("FineY", "FineY", (edy - limy + 3) * bins1, starty + (limy - 1.5) * bins1, starty + (edy + 1.5) * bins1);

  for (int k = 1; k <= (edx - limx + 3) * bins1; k++){
    for (int l = 1; l <= (edy - limy + 3) * bins1; l++){
      fx -> SetBinContent(k, fx -> GetBinContent(k) + g -> GetBinContent(k + (limx - 1.5) * bins1,l + (limy - 1.5) * bins1));
      fy -> SetBinContent(l, fy -> GetBinContent(l) + g -> GetBinContent(k + (limx - 1.5) * bins1,l + (limy - 1.5) * bins1));
    }
  }

  int maxx2 = fx -> GetMaximum();
  //cout << "Maximum x bin is " << maxx2 << " pixels" << endl;
  int maxy2 = fy -> GetMaximum();
  //cout << "Maximum y bin is " << maxy2 << " pixels" << endl;

  int limx2 = fx -> FindFirstBinAbove(ratio2 * maxx2) - 1;
  int widthx = fx -> FindLastBinAbove(ratio2 * maxx2) - limx2;

  int limy2 = fy -> FindFirstBinAbove(ratio2 * maxy2) - 1;
  int widthy = fy -> FindLastBinAbove(ratio2 * maxy2) - limy2;


  TFile* outfile = new TFile(Form("%s/DBN_%d-%s.root", outputRoot, dbn, ed),"RECREATE");
  img -> SetName("original");
  img -> Write();

  img -> Crop(startx + (limx - 2.5) * bins1 + limx2, starty + (limy - 2.5) * bins1 + limy2, widthx + 2 * bins1, widthy + 2 * bins1);
  cout << "X range: " << startx + (limx - 2.5) * bins1 + limx2 << " pixels to " << startx + (limx - 2.5) * bins1 + limx2 + widthx + 2 * bins1 << " pixels." << endl;
  cout << "Y range: " << starty + (limy - 2.5) * bins1 + limy2 << " pixels to " << starty + (limy - 2.5) * bins1 + limy2 + widthy + 2 * bins1 << " pixels." << endl;



  	//We select the outermost tile with intensity within first 20% of intensity, and 
  	//for (int x = 0; x < interv/bins1; ++x){
  	//	for (int y = 0; x < interv/bins1; ++y){
  	//		
  	//	}
  	//}


  	//TH2D* fg = (TH2D*)g -> Clone("fg");
  TCanvas* c2 = new TCanvas("c2","", widthx + 5 * bins1, widthy + 5 * bins1);
  c2 -> SetMargin(0,0,0,0);
  c2 -> cd();
  img -> Draw();
  c2 -> SaveAs(Form("%s/DBN_%d-%s.png", output, dbn, ed));

  TCanvas* c0 = new TCanvas("c0","",700,700);
  c0 -> Divide(2,2);
  c0 -> cd(1);
  gStyle -> SetPalette(55);
  gStyle -> SetOptStat(0);
  //g -> GetXaxis()->SetRange(startx + limx * bins1, startx + edx * bins1);
  //g -> GetYaxis()->SetRange(starty + limy * bins1, starty + edy * bins1);
  //g->Draw("colz");
  xg -> SetAxisRange(0, 256 * bins1 * sqrt(bins1), "z");
  xg -> GetXaxis()->SetRangeUser((limx -1.5) * bins1 + startx, (edx + 1.5) * bins1 + startx);
  xg -> GetYaxis()->SetRangeUser((limy -1.5) * bins1 + starty, (edy + 1.5) * bins1 + starty);
  xg -> Draw("colz");

  c0 -> cd(2);
  //yg -> GetXaxis()->SetRange(limy + 1, edy);
  //yg -> GetYaxis()->SetRange(limx + 1, edx);
  //yg -> SetAxisRange(0, 256 *  bins1, "z");
  //yg -> Draw("colz");
  g -> SetAxisRange(0, 256, "z");
  g -> GetXaxis()->SetRangeUser((limx -1.5) * bins1 + startx, (edx + 1.5) * bins1 + startx);
  g -> GetYaxis()->SetRangeUser((limy -1.5) * bins1 + starty, (edy + 1.5) * bins1 + starty);
  g -> Draw("colz");

  c0 -> cd(3);
  //hx -> GetXaxis() -> SetRangeUser((limx - 1.5) * bins1 + startx, (edx + 1.5) * bins1 + startx);
  fx -> Draw();

  c0 -> cd(4);
  //hy -> GetXaxis() -> SetRangeUser((limy - 1.5) * bins1 + starty, (edy + 1.5) * bins1 + starty);
  fy -> Draw();

  c0 -> SetName("analysis");
  c0 -> Write();

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
  
