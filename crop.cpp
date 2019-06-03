
  // crop the picture into block parts & number parts
  //Author: Xiaoning Wang
#include <string>

void crop(const char* name = "N", const char* path = "IgnoreMe/test", const char * output = "IgnoreMe/test", const char * outputNumber = "IgnoreMe/test"){

  TString pic = (Form("%s/%s.JPG",path,name));
  TASImage* img= (TASImage*) TASImage::Open(pic);
  TASImage* imgn = (TASImage*) TASImage::Open(pic);
  gStyle->SetOptStat(0);
	
  int bins1 = 20;
  int intervx = 2500;
  int startx = 1500;
  int intervy = 1400;
  int starty = 1800; 
  double ratio1 = 0.5;  
  double ratio2 = 0.1;

  int offsetx = 1460;
  int offsety = 560;
  int rangeup = 1000;
  int rangewide = 3900;

  TH2D *gcr = new TH2D("Intensity","Intensity", intervx, startx, startx + intervx, intervy, starty, starty + intervy);
  TH2D *xgcr = new TH2D("IntensityB", "IntensityB", intervx/bins1, startx, startx + intervx, intervy/bins1, starty, starty + intervy);
  //TH2D *yg = new TH2D("IntensityY", "IntensityY", interv/bins1, starty, starty + interv, interv/bins1, startx, startx + interv);
  TH1D *hxcr = new TH1D("Intenx","Intenx", intervx/bins1, startx, startx + intervx);
  TH1D *hycr = new TH1D("Inteny","Inteny", intervy/bins1, starty, starty + intervy);

  //cout << intervx/bins1 << endl;
  //cout << intervy/bins1 << endl;

  UInt_t *argbcr = img -> GetArgbArray();

  UInt_t yPixelscr = img -> GetHeight();
  //cout << "Height: " << yPixels << endl;
  UInt_t xPixelscr = img -> GetWidth();
  //cout << "Width: " << xPixels << endl;


  for (int col = 0; col < intervx/bins1; ++col) {
    for (int row = 0; row < intervy/bins1; ++row) {
      int totInt = 0;
      for (int i = 0; i < bins1; ++i){
        for (int j = 0; j < bins1; ++j){
        int index = (row * bins1 + starty + i) * xPixelscr + col * bins1 + startx + j;
        //cout << index << endl;
        int grey = argbcr[index]&0xff;
        if (grey == 0) {
          grey = 1;
        }
        gcr -> SetBinContent(col * bins1 + j + 1, row * bins1 + i + 1, grey);
        totInt = totInt + grey;
      }
      }
      //cout << totInt << endl;
      xgcr -> SetBinContent(col + 1, row + 1, totInt); 
      //yg -> SetBinContent(row + 1, col + 1, totInt);
    }
  }

  for (int i = 1; i <= intervx/bins1; i++){
    for (int j = 1; j <= intervy/bins1; j++){
      hxcr -> SetBinContent(i, hxcr -> GetBinContent(i) + xgcr -> GetBinContent(i,j));
      hycr -> SetBinContent(j, hycr -> GetBinContent(j) + xgcr -> GetBinContent(i,j));
    }
  }

 

    //TH2D* rg = (TH2D*)g -> Clone("rg");
    //rg -> Rebin2D(bins1, bins1);
  int maxxcr = hxcr -> GetMaximum();
  cout << "Maximum x bin is " << maxxcr << " pixels" << endl;
  int maxycr = hycr -> GetMaximum();
  cout << "Maximum y bin is " << maxycr << " pixels" << endl;

  //debug
  //TCanvas* debuggy = new TCanvas("debuggy","");
  //debuggy -> Divide(2,2);
  //debuggy -> cd(1);
  //hxcr -> Draw("colz");
  //debuggy -> cd(2);
  //hycr -> Draw("colz");
  //return 0;

  int limxcr = hxcr -> FindFirstBinAbove(ratio1 * maxxcr) - 1;
  int edxcr = hxcr -> FindLastBinAbove(ratio1 * maxxcr);

  int limycr = hycr -> FindFirstBinAbove(ratio1 * maxycr) - 1;
  int edycr = hycr -> FindLastBinAbove(ratio1 * maxycr);

  TH1D *fxcr = new TH1D("FineX", "FineX", (edxcr - limxcr + 3) * bins1, startx + (limxcr - 1.5) * bins1, startx + (edxcr + 1.5) * bins1);
  TH1D *fycr = new TH1D("FineY", "FineY", (edycr - limycr + 3) * bins1, starty + (limycr - 1.5) * bins1, starty + (edycr + 1.5) * bins1);

  for (int k = 1; k <= (edxcr - limxcr + 3) * bins1; k++){
    for (int l = 1; l <= (edycr - limycr + 3) * bins1; l++){
      fxcr -> SetBinContent(k, fxcr -> GetBinContent(k) + gcr -> GetBinContent(k + (limxcr - 1.5) * bins1,l + (limycr - 1.5) * bins1));
      fycr -> SetBinContent(l, fycr -> GetBinContent(l) + gcr -> GetBinContent(k + (limxcr - 1.5) * bins1,l + (limycr - 1.5) * bins1));
    }
  }

  int maxx2 = fxcr -> GetMaximum();
  //cout << "Maximum x bin is " << maxx2 << " pixels" << endl;
  int maxy2 = fycr -> GetMaximum();
  //cout << "Maximum y bin is " << maxy2 << " pixels" << endl;

  int limx2 = fxcr -> FindFirstBinAbove(ratio2 * maxx2) - 1;
  int widthx = fxcr -> FindLastBinAbove(ratio2 * maxx2) - limx2;

  int limy2 = fycr -> FindFirstBinAbove(ratio2 * maxy2) - 1;
  int widthy = fycr -> FindLastBinAbove(ratio2 * maxy2) - limy2;


  //TFile* outfile = new TFile(Form("%s/DBN_%d-%s.root", outputRoot, dbn, ed),"RECREATE");
  //img -> SetName("original");
  //img -> Vectorize();
  //img -> Write();

  img -> Crop(startx + (limxcr - 2.5) * bins1 + limx2, starty + (limycr - 2.5) * bins1 + limy2, widthx + 2 * bins1, widthy + 2 * bins1);
  cout << "X range: " << startx + (limxcr - 2.5) * bins1 + limx2 << " pixels to " << startx + (limxcr - 2.5) * bins1 + limx2 + widthx + 2 * bins1 << " pixels." << endl;
  cout << "Y range: " << starty + (limycr - 2.5) * bins1 + limy2 << " pixels to " << starty + (limycr - 2.5) * bins1 + limy2 + widthy + 2 * bins1 << " pixels." << endl;

  imgn -> Crop(startx + (limxcr - 2.5) * bins1 + limx2 - offsetx, starty + (limycr - 2.5) * bins1 + limy2 - rangeup - offsety, rangewide, rangeup);


    //We select the outermost tile with intensity within first 20% of intensity, and 
    //for (int x = 0; x < interv/bins1; ++x){
    //  for (int y = 0; x < interv/bins1; ++y){
    //    
    //  }
    //}


  TH2D* fgcr = (TH2D*)gcr -> Clone("fgcr");
  //TCanvas* c2cr = new TCanvas("c2","", 1200,1200);
  //c2cr -> SetMargin(0,0,0,0);
  //c2cr -> cd();
  //img -> Draw();
  //c2cr -> SaveAs(Form("%s/DBN_%d-%s.png", output, dbn, ed));
  img -> WriteImage(Form("%s/%s-block.JPG", output, name));
  imgn -> WriteImage(Form("%s/%s-number.JPG",outputNumber, name));

  TCanvas* c0cr = new TCanvas("c0cr","",700,700);
  c0cr -> Divide(2,2);
  c0cr -> cd(1);
  gStyle -> SetPalette(55);
  gStyle -> SetOptStat(0);
  //g -> GetXaxis()->SetRange(startx + limx * bins1, startx + edx * bins1);
  //g -> GetYaxis()->SetRange(starty + limy * bins1, starty + edy * bins1);
  //g->Draw("colz");
  xgcr -> SetAxisRange(0, 256 * bins1 * sqrt(bins1), "z");
  xgcr -> GetXaxis()->SetRangeUser((limxcr - 1.5) * bins1 + startx, (edxcr + 1.5) * bins1 + startx);
  xgcr -> GetYaxis()->SetRangeUser((limycr - 1.5) * bins1 + starty, (edycr + 1.5) * bins1 + starty);
  xgcr -> Draw("colz");

  c0cr -> cd(2);
  //yg -> GetXaxis()->SetRange(limy + 1, edy);
  //yg -> GetYaxis()->SetRange(limx + 1, edx);
  //yg -> SetAxisRange(0, 256 *  bins1, "z");
  //yg -> Draw("colz");
  gcr -> SetAxisRange(0, 256, "z");
  gcr -> GetXaxis()->SetRangeUser((limxcr - 1.5) * bins1 + startx, (edxcr + 1.5) * bins1 + startx);
  gcr -> GetYaxis()->SetRangeUser((limycr - 1.5) * bins1 + starty, (edycr + 1.5) * bins1 + starty);
  gcr -> Draw("colz");

  c0cr -> cd(3);
  //hx -> GetXaxis() -> SetRangeUser((limx - 1.5) * bins1 + startx, (edx + 1.5) * bins1 + startx);
  fxcr -> Draw();

  c0cr -> cd(4);
  //hy -> GetXaxis() -> SetRangeUser((limy - 1.5) * bins1 + starty, (edy + 1.5) * bins1 + starty);
  fycr -> Draw();
}
  
