//sPHENIX EMCal fiber counting software
//Authors: Yongsun Kim, Anabel Romero， Xiaoning Wang
//Based on a macro by Olivier Couet that converts an image to a 2D histogram, thank you Tim Rinn for fixing the memory issues

#include <vector>
#include "commonUtility.h"
#include <TASImage.h>
#include <TEllipse.h>
#include <fstream>

void fiberCounter_number(const char* dbn = "42", const char* ed = "N", const char * input_folder = "test", const char * output_folder = "test", const char * output_csv = "test/result.csv"){

//This part of code automatically crops the picture.
  TString pic = Form("%s/DBN_%s-%s.JPG",input_folder,dbn,ed);
  TASImage* img= (TASImage*) TASImage::Open(pic);
  gStyle->SetOptStat(0);

  //c0 -> SetName("analysis");
  //c0 -> Write();

    //c0 -> cd(2);
    //gPad->SetLogy();  
    //h -> Draw();

  //img -> SetName("cropped");
  //img -> Vectorize();
  //img -> Write();



  short **arrInten = new short*[10000];
  short **arrFlag = new short*[10000];
  //short **arrLocThr = new short*[xPixels+1];
  for (int i = 0; i < 10000;i++)
  {
     arrInten[i] = new short[10000];
     arrFlag[i] = new short[10000];
      //arrLocThr[i] = new short[yPixels+1];
  }

  //short seedThr = 100;
  //short bkgThr = 80;
  short absBkg = 80;
  int absAreaCut = 20;
  double intShift = 0.4;
  
  double zeroInt = 1;
  double dropBar = 0.1;
  double midBar = 0.5;
  double goodBar = 0.75;
  double badBar = 0.15;
  const int searchRange = 100;
  //const int maxX = 3000;
  //const int maxY = 3000;
  gStyle->SetPalette(52);
  gStyle->SetOptStat(0);
  gROOT->SetBatch(kTRUE);
  
  //TFile* picfile = TFile::Open(Form("%s/DBN_%d-%s.root",input_folder,dbn,ed));
  //TASImage* img = (TASImage*)picfile -> Get("cropped");
 
  UInt_t yPixels = img -> GetHeight();
  UInt_t xPixels = img -> GetWidth();
  cout << "xPixels = " << xPixels << endl;
  cout << "yPixels = " << yPixels << endl;

  int xc = (int)xPixels/2;
  int yc = (int)yPixels/2;

  //cout << "xc: " << xc << " " << "yc: " << yc << endl;

  
    /*if ( (maxX<xPixels) || ( maxY <yPixels) )   {
    cout <<"  (maxX<xPixels) || ( maxY <yPixels) ! " << endl;
    return;
  }*/
  
  TH1D* hNfib = new TH1D("hNfib",";Number of clusters;Entries",4000,0,4000);

  UInt_t* argb   = img -> GetArgbArray();
  
  TH2D* hOriginal = new TH2D("hOriginal", "Spectrum of Green Light Picture",xPixels+1,.5,xPixels,yPixels+1,.5,yPixels);
  TH2D* h = hOriginal;

  TH1D* h1d = new TH1D("h1d","Light Intensity of Pixels;Intensity of Pixels;Counts",256,0,256);
  TH1D* henergy = new TH1D(Form("henergy_%s",dbn),"Intensity of clusters;Intensity;Counts",50,0,25000);
  TH1D* henergyNorm = new TH1D(Form("henergyNorm_%s",dbn),"Light Intensity of Fibers;Self-normalized Intensity of Fibers;Counts",50,0,3);
  TH1D* hsize = new TH1D(Form("hsize_%s",dbn),"Size of fibers;Pixels;Counts",50,0,150);
  TH1D* hsizeNorm = new TH1D(Form("hsizeNorm_%s",dbn),"Size of Fibers;Self-normalized Size of Fibers;Counts",50,0,3);
  //TH1D* hMeanE = (TH1D*)henergy->Clone("hMeanE");
  //hMeanE->SetXTitle("<cluster intensity>");

  //TH1D* hRMSE = new TH1D("hRMSE",";RMS;Entries",1000,0,100000);
  //TH1D* hRMSNorm = new TH1D("hRMSNorm",";RMS normalized by mean;Counts",1000,0,1);

  TH2D* seCor = new TH2D("seCorrelation","Intensity Size Correlation;Self-normalized Size;Self-normalized Intensity",50,0,3,50,0,3);

  TH2D* gFibers = new TH2D("nFibers","Number of Fibers",25,0,xPixels,25,0,yPixels);
  TH2D* gInten = (TH2D*)gFibers->Clone("gInten");
  TH2D* densityInten   = (TH2D*)gFibers->Clone("densityInten");
  //TH2D* gNpix  = (TH2D*)gFibers->Clone("gNpix");
  //TH2D* densityNpix   = (TH2D*)gFibers->Clone("densityNpix");

  //TH1D* hDefDist = new TH1D("hDefDist",";RMS xy normalized by sqrt(area);", 200,0,200);
  //cout << "line 71" << endl;

  h1d->Sumw2();
  for (int col = 0; col < xPixels; ++col) {
    for (int row = 0; row < yPixels; ++row) {
      int index = row * xPixels + col;
      short grey = argb[index]&0xff;
      h->SetBinContent(col + 1, yPixels - row, grey);
      h1d->Fill(grey);
    }
  }
  
  TCanvas* c1 = new TCanvas("c1","",700,700);
  h1d->SetAxisRange(0.5,2e6,"Y");
  h1d->Draw();
  h1d->SetTitleOffset(1.4,"Y");
  
  TH1D* h1dmax = (TH1D*)h1d->Clone("h1dmax");
  h1dmax->GetXaxis()->SetRangeUser(absBkg,256);
  TH1D* h1dmin = (TH1D*)h1d->Clone("h1dmin");
  h1dmin->GetXaxis()->SetRangeUser(absBkg,h1dmax->GetMaximumBin()*h1dmax->GetBinWidth(h1dmax->GetMaximumBin()));
  short bkgThr = (h1dmin->GetMinimumBin()*h1dmin->GetBinWidth(h1dmin->GetMinimumBin()))*intShift;
  short seedThr = bkgThr-5;
  
  drawText(Form("bkgThr = %d",bkgThr),0.5,0.8);
  drawText(Form("absBkg = %d",absBkg),0.5,0.7);
  //drawText(Form("seedThr = %d",seedThr),0.6,0.8);
  jumSun(absBkg,0.5,absBkg,2e6);
  jumSun(bkgThr,0.5,bkgThr,2e6);
  gPad->SetLogy();  
  c1->SaveAs(Form("%s/DBN_%s-%s_pixels.pdf",output_folder,dbn,ed));

  TCanvas* c0 = new TCanvas("c0","",700,700);
  c0->Divide(2,3);
  c0->cd(1);
  //c0->cd(2);
  //h->SetAxisRange(0,256,"z");
  //h->Draw("colz");
  //gPad->SetRightMargin(0.2);
     
  //cout << "line 108" << endl;
  TH2D* h3 = (TH2D*)h->Clone("h3");
  
  TH2D* h4 = (TH2D*)h->Clone("h4");  
  for ( int ix0=1 ; ix0<=h4->GetNbinsX() ; ix0++) {
    for ( int iy0=1 ; iy0<=h4->GetNbinsY() ; iy0++) {
      h4->SetBinContent(ix0,iy0,0.0001);
    }}
  
  
  std::vector<int> px;
  std::vector<int> py;
  std::vector<int> pinten;
  std::vector<int> vClstE;
  std::vector<int> vClstS;  
  std::vector<float> vClstX;  
  std::vector<float> vClstY;
  //  std::vector<short> inten;   

  //cout << "line 127" << endl;
  // Clean up backgrounds ;

  //short arrInten[xPixels+1][yPixels+1];
  //short arrFlag[xPixels+1][yPixels+1];

  //arrInten= new short*[xPixels+1];
  //arrFlag = new short*[xPixels+1];
  //short **arrLocThr = new short*[xPixels+1];
  //for (int i = 0; i < xPixels+1;i++)
  //    {
  //   arrInten[i] = new short[yPixels+1];
  //   arrFlag[i] = new short[yPixels+1];
      //arrLocThr[i] = new short[yPixels+1];
  //      }

  int kIn=1;  int kOut=-1;   int kUnde=0; 
  int nXbins = h3->GetNbinsX() ; 
  int nYbins = h3->GetNbinsY() ; 

  for ( int ix0=1 ; ix0<=nXbins ; ix0++) {
    for ( int iy0=1 ; iy0<=nYbins ; iy0++) {
      //arrLocThr[ix0][iy0] = bkgThr;

      short val0 = h3->GetBinContent(ix0,iy0);
      arrInten[ix0][iy0] = val0;
      arrFlag[ix0][iy0] = kUnde;	
      if (val0 < bkgThr )  {
		h3->SetBinContent(ix0, iy0, zeroInt);
		arrFlag[ix0][iy0] =  kOut;
      }
      
    }
  }
  //cout << "line 157" << endl;

  // Local threshold 
  for ( int ix0=1 ; ix0<=nXbins ; ix0 = ix0 + searchRange) {
    for ( int iy0=1 ; iy0<=nYbins ; iy0 = iy0 + searchRange) {
      
      short localMax = 0;
      for ( int ix = ix0 ; ix <= ix0 + searchRange - 1 ; ix++ ) { 
		if (ix > nXbins ) continue;
		for ( int iy = iy0 ; iy <= iy0 + searchRange - 1 ; iy++ ) { 
			if (iy > nYbins ) continue;
			if ( arrInten[ix][iy] > localMax )  { 
				localMax = arrInten[ix][iy]; 
			}
		}
      }

      short localMin = localMax * 0.7;
      for ( int ix = ix0 ; ix <= ix0 + searchRange - 1 ; ix++ ) {
        if (ix > nXbins ) continue;
        for ( int iy = iy0 ; iy <= iy0 + searchRange - 1 ; iy++ ) {
          if (iy > nYbins ) continue;

	  //if ( ( arrFlag[ix][iy] == kOut )  && (arrInten[ix][iy] >= localMin) )
	  if ( (arrInten[ix][iy] >= localMin) )
	    arrFlag[ix][iy] = kUnde;
	  
	  if ( arrInten[ix][iy] < absBkg )
	    arrFlag[ix][iy] = kOut;
	}
      }
    }}
  
  int nClst = 0;
    //add an array tracking each cluster's size
  //std::vector<int> nSize;
  
  for ( int ix0=1 ; ix0<=nXbins ; ix0++) {
    for ( int iy0=1 ; iy0<=nYbins ; iy0++) {
      short val0 = arrInten[ix0][iy0];
      
      if ( val0 < seedThr ) 
		continue;
      if ( arrFlag[ix0][iy0] != kUnde ) // if it is already included in other clusters.
      	continue;

      //cout << " found new seed!   " ;
      //cout <<"(x,y,intensity) = " <<  ix0<<", "<<iy0<<", "<<val0<<endl;


      // Found a seed!     (ix0, iy0, val0) are the seed! 
      
      // Begin Clustering 
      //      inten.clear();
      px.clear();
      py.clear();
      pinten.clear();

      //      inten.push_back(val0);
      px.push_back(ix0);   
      py.push_back(iy0);
      pinten.push_back( val0 ) ;

      arrFlag[ix0][iy0] = kIn;
      
      bool completeFlag = false;
      int nIter = 0;
      int countedHits=0;
      int itrBegin=0;  // <== this number will evolve over the iteration :  itrBegin = itrEnd;
      int itrEnd;

      while ( completeFlag==false )  
	{
	  //	  cout << " px.size() = " << px.size() << endl;
	  nIter++;
	  completeFlag = true;
	  
	  itrEnd = int(px.size());
	  //	  cout << " itrBegin = " << itrBegin ;
	  //	  cout << ", itrEnd = " << itrEnd<<endl;
	  for ( int vi = itrBegin ; vi < itrEnd ; vi++) {
	    
	    int xCand = px[vi];     int yCand = py[vi];      short intenCand = pinten[vi];

	    //	    if ( (xCand < nXbins) &&  ( arrFlag[xCand+1][yCand]==kUnde)   ) {	       // right end 
	    if ( (xCand < nXbins) &&   (arrFlag[xCand+1][yCand]==kUnde) ){
	      completeFlag=false; 
	      countedHits++;
	      px.push_back( xCand+1 ) ;
	      py.push_back( yCand   ) ;
	      pinten.push_back( arrInten[xCand+1][yCand]   ) ;
	      arrFlag[xCand+1][yCand]=kIn; 
	    }
	    
	    if ( (xCand > 1 ) && (arrFlag[xCand-1][yCand]==kUnde) ){
	      completeFlag=false; 
	      countedHits++;
	      px.push_back( xCand-1 ) ;
	      py.push_back( yCand   ) ;
	      pinten.push_back( arrInten[xCand-1][yCand]   ) ;
	      arrFlag[xCand-1][yCand]=kIn; 
	    }
	    if ( (yCand < nYbins) && (arrFlag[xCand][yCand+1]==kUnde)) {
	      completeFlag=false; 
	      countedHits++;
	      px.push_back( xCand ) ;
	      py.push_back( yCand+1   ) ;
	      pinten.push_back( arrInten[xCand][yCand+1]   ) ;
	      arrFlag[xCand][yCand+1]=kIn; 
	    }
	    if ( (yCand > 1    ) && ( arrFlag[xCand][yCand-1]==kUnde) ) {
	      completeFlag=false; 
	      countedHits++;
	      px.push_back( xCand ) ;
	      py.push_back( yCand-1   ) ;
	      pinten.push_back( arrInten[xCand][yCand-1]  ) ;
	      arrFlag[xCand][yCand-1]=kIn; 
	    }
	  }
	  itrBegin = itrEnd;

	  /*	  if ( nClst==2 ) {
            TCanvas* ctemp = new TCanvas("ctemp","", 400,400);
            TH2D* htemp = new TH2D("htemp","",50, px[0]-15, px[0]+35, 50, py[0]-25, py[0]+25);
            for ( int ii=0 ; ii<px.size(); ii++)   {
              int theBin = htemp->FindBin( px[ii], py[ii]) ;
              htemp->SetBinContent( theBin, pinten[ii] );
            }
	    htemp->SetAxisRange(200,256,"Z");
	    htemp->Draw("colz");
	    drawText(Form("Iter. %d",nIter),0.2,0.8);
	    ctemp->SaveAs(Form("clustering_itr%d.png",nIter));
	      
	    }*/
	}

      /*if ( px.size() < absAreaCut )  {
		//cout << " The cluster's area is too small.  Smaller than " << absAreaCut << ", so this is skipped! " << endl;
		continue;}
      */

    
      nClst++;
      
      //cout << "Number of hits in "<<nClst<<"th cluster: " << px.size() << ",  nIteration = "<<nIter<<endl;  
      int sumEnergy = 0;
      for ( int vi=0 ; vi < px.size() ; vi++) {
		    sumEnergy = sumEnergy + pinten[vi];
      }
      //cout << "Cluster energy = " << sumEnergy << endl;
      //      if ( sumEnergy < absSumEnergyThr ) continue;

      float xmean=0;
      float ymean=0;
      int nPixels= px.size();
      float xsquare = 0 ;
      float ysquare = 0 ;
      for ( int vi=0 ; vi < nPixels ; vi++) { 
		    xmean = xmean + px[vi]*pinten[vi] ;
		    ymean = ymean + py[vi]*pinten[vi] ;
		    xsquare = xsquare + px[vi]*px[vi]*pinten[vi] ; 
		    ysquare = ysquare + py[vi]*py[vi]*pinten[vi] ; 
      }
      xmean = xmean / sumEnergy ;
      ymean = ymean / sumEnergy ;
      xsquare = xsquare / sumEnergy ;
      ysquare = ysquare / sumEnergy ;
      //This is an intensity weighted average position, i.e., intensity center
      
      /*      float xRMS = sqrt ( xsquare - xmean*xmean ) / sqrt ( nPixels) ; 
      float yRMS = sqrt ( ysquare - ymean*ymean ) / sqrt ( nPixels) ; 
      float rRMS = sqrt( xRMS*xRMS + yRMS*yRMS) ;
      hDefDist->Fill(rRMS);*/
      // Anabel can add the RMS of each clusters here :  
      // for example, xRMS, yRMS divided by the sqrt(area) = sqrt( nPixels) 
      
      henergy->Fill(sumEnergy);
      hsize->Fill(nPixels);
      vClstS.push_back(nPixels);
      vClstE.push_back(sumEnergy);
      vClstX.push_back(xmean);
      vClstY.push_back(ymean);
      
      gFibers->Fill(xmean, ymean);
      gInten-> Fill(xmean, ymean,sumEnergy);
      //gNpix-> Fill(xmean, ymean,nPixels);
      
      //      for ( int vi=0 ; vi < px.size() ; vi++) {
      //	h4->SetBinContent( px[vi],  py[vi], pinten[vi]);
      //      }
      //      TCanvas* c11 = new TCanvas("c11","",400,400);
      //      h4->Draw("colz");
      //      c11->SaveAs(Form("Cluster_%d.png",nClst));
      
    }}
  
//  TCanvas* cRMS =  new TCanvas("cRMS","",400,400);
//  hDefDist->Draw();

  densityInten->Add(gInten);
  densityInten->Divide(gFibers);
  //densityNpix->Add(gNpix);
  //densityNpix->Divide(gFibers);

  /*gStyle->SetPalette(1);
  c0->cd(5);
  cleverRangeZ(gInten);
  gInten->SetTitle("Light intensity");
  gInten->Draw("colz");*/

  /*c0->cd(6);
  cleverRangeZ(gFibers);
  gFibers->SetTitle("Number of Fibers");
  gFibers->Draw("colz");*/


  /*c0->cd(8);
  cleverRangeZ(densityNpix);
  densityNpix->SetXTitle("Number of Pixels per Fiber");
  densityNpix->Draw("colz");*/
  
  
  /*int totSize = 0;
  int totValid = nSize.size();


  for (int di = 0; di < nSize.size(); di++){
    int localSize = nSize[di];
    //clusters less than 40 pixels or greater than 120 are not normal fibers
    if ((localSize >= (2*absAreaCut)) && (localSize < 6*absAreaCut)){
      totSize = totSize + nSize[di];
    } else {
      totValid = totValid-1;
    }
  }
  double aveSize = (double) totSize/totValid;
  cout << aveSize << endl;
  cout << nClst << endl;*/

//ngood: # of fibers with light intensity less than goodBar
//nbad: # of fibers with light intensity less than badBar
//less than dropBar are those to be ignored entirely.
//Supposedly, measuring from either ends, the number of bad fibers, good fibers and total fibers should be the same
//fibers are bad if either its area or its light intensity is below bad bar, it's a good fiber, if it's above good bar
//
  cout << "Uncorrected count is " << nClst << endl;

  TF1 *fe = new TF1("energyFit","gaus", 5000, 20000);
  TF1* fs = new TF1("sizeFit", "gaus", 50, 150);

  henergy -> Fit(fe, "RQ");
  hsize -> Fit(fs, "RQ");

  int aveEnergy = (int)fe -> GetParameter(1);
  cout << "Most occupied energy is " << aveEnergy << endl;
  int aveSize = (int)fs -> GetParameter(1);
  cout << "Most occupied size is " << aveSize << endl;
  int nbundle = 0, ngood = 0, ndrop = 0, nbad = 0, nmid = 0, nCorrected = nClst;

  c0->cd(2);
  h->SetAxisRange(0,256,"z");
  h->Draw("colz");
  gPad->SetRightMargin(0.11);

  int nClst1 = 0, nClst2 = 0, nClst3 = 0, nClst4 = 0;
  int cent = 0;
  for ( int ci = 0 ; ci < vClstE.size() ; ci++) {
	  double energy = (double) vClstE[ci] /aveEnergy;
    double thisSize = (double) vClstS[ci] /aveSize;
    int multi = 1;
    while (thisSize > (multi+0.6)){
      multi++;
    }
    if (multi > 1) {
      //cout << "Found a bundle of " << multi << " fibers." << endl;
      nbundle =  nbundle + 1;
    }
    energy = (double)energy/multi;
    thisSize = (double)thisSize/multi;
    if ((energy < dropBar) || (thisSize < dropBar)) {
      ndrop++; 
      nCorrected = nCorrected - 1; 
      continue;
    }
    nCorrected = nCorrected + multi - 1; 
    TEllipse *el3 = new TEllipse(vClstX[ci], vClstY[ci], 5);
    el3->SetLineColor(kBlack);
    el3->SetLineWidth(1);
    el3->SetFillStyle(0);
    el3->Draw();
    //cout << multi << endl; 
    if (vClstX[ci] <= xc && vClstY[ci] <= yc){
    	nClst1 = nClst1 + multi;
    	//cout << ci << ": 1; " << nClst1 << endl;
    }
    if (vClstX[ci] > xc && vClstY[ci] <= yc){ 
    	nClst2 = nClst2 + multi;
    	//cout << ci << ": 2; " << nClst2 << endl;
	}
    if (vClstX[ci] <= xc && vClstY[ci] > yc){ 
    	nClst3 = nClst3 + multi;
    	//cout << ci << ": 3; " << nClst3 << endl;
    }
	if (vClstX[ci] > xc && vClstY[ci] > yc){ 
		nClst4 = nClst4 + multi;
    	//cout << ci << ": 4; " << nClst4 << endl;
	}

    if (energy < badBar) {nbad++;}
    if ((energy >= badBar) && (energy < midBar)) {nmid++;}
    if ((energy >= midBar) && (energy < goodBar)) {ngood++;}
	  henergyNorm -> Fill(energy, multi);
    hsizeNorm -> Fill(thisSize, multi); 
    seCor -> Fill(thisSize, energy, multi);
  }
  cout << "Found " << nbundle << " bundles of fibers" << endl;
  cout << "Dropped " << ndrop << " fibers of small area or intensity" << endl;

  if (nCorrected >= 2561) {
    cout << "Passed the overall bar" << endl;
    bool redo = false;
    if ((nClst1 > 667) || (nClst2 > 667) || (nClst3 > 667) || (nClst4 > 667)){
      if ((nClst1 + nClst2)/2 - (nClst3 + nClst4)/2 > 26.68){
        cout << "Bottom towers have too many more fibers than top towers, shifting down half a row." << endl;
        yc = yc - (yPixels - 40)/100; 
        redo = true;
      } else {
        if ((nClst3 + nClst4)/2 - (nClst1 + nClst2)/2 > 26.68){
        cout << "Top towers have too many more fibers than bottom towers, shifting up half a row." << endl;
        yc = yc + (yPixels - 40)/100; 
        redo = true;
        } else {
          if (nClst1 - nClst3 > 26.68 || nClst2 - nClst4 > 26.68){
            cout << "One bottom tower has too many more fibers than its top tower, shifting down a quarter of a row." << endl;
            yc = yc - (yPixels - 40)/200;  
            redo = true;
          }
          if (nClst4 - nClst2 > 26.68 || nClst3 - nClst1 > 26.68){
            cout << "One right tower has too many more fibers than its left tower, shifting up a quarter of a column." << endl;
            yc = yc + (yPixels - 40)/200; 
            redo = true;
          } 
        }
      } 
      if ((nClst1 + nClst3)/2 - (nClst2 + nClst4)/2 > 26.68){
        cout << "Left towers have too many more fibers than right towers, shifting left half a column." << endl;
        xc = xc - (xPixels - 40)/100; 
        redo = true;
      } else {
        if ((nClst2 + nClst4)/2 - (nClst1 + nClst3)/2 > 26.68){
          cout << "Right towers have too many more fibers than left towers, shifting right half a column." << endl;
          xc = xc + (xPixels - 40)/100; 
          redo = true;
        } else {
          if (nClst1 - nClst2 > 26.68 || nClst3 - nClst4 > 26.68){
            cout << "One left tower has too many more fibers than its right tower, shifting left a quarter of a column." << endl;
            xc = xc - (xPixels - 40)/200; 
            redo = true;
          } 
          if (nClst2 - nClst1 > 26.68 || nClst4 - nClst3 > 26.68){
            cout << "One right tower has too many more fibers than its left tower, shifting right a quarter of a column." << endl;
            xc = xc + (xPixels - 40)/200; 
            redo = true;
          } 
        }
      } 

      if (redo) {

        nClst1 = 0, nClst2 = 0, nClst3 = 0, nClst4 = 0;
        for ( int ci = 0 ; ci < vClstE.size() ; ci++) {
          double energy = (double) vClstE[ci] /aveEnergy;
          double thisSize = (double) vClstS[ci] /aveSize;
          int multi = 1;
          while (thisSize > (multi+0.6)){
            multi++;
          }
          thisSize = (double)thisSize/multi;
          if ((energy < dropBar) || (thisSize < dropBar)) {
            continue;
          }
          if (vClstX[ci] <= xc && vClstY[ci] <= yc){
            nClst1 = nClst1 + multi;
            //cout << ci << ": 1; " << nClst1 << endl;
          }
          if (vClstX[ci] > xc && vClstY[ci] <= yc){ 
            nClst2 = nClst2 + multi;
            //cout << ci << ": 2; " << nClst2 << endl;
          }
          if (vClstX[ci] <= xc && vClstY[ci] > yc){ 
            nClst3 = nClst3 + multi;
            //cout << ci << ": 3; " << nClst3 << endl;
          }
          if (vClstX[ci] > xc && vClstY[ci] > yc){ 
            nClst4 = nClst4 + multi;
            //cout << ci << ": 4; " << nClst4 << endl;
          }
        }
      }
    }
  }

  double f = nCorrected/2668.;
  double rgood = (double)(ngood)/nCorrected;
  double R = henergyNorm->GetRMS()/henergyNorm->GetMean();
  double rbad = (double)(nbad)/nCorrected;
  double rmid = (double)(nmid)/nCorrected;

  double n1 = (double)nClst1/667;
  double n2 = (double)nClst2/667;
  double n3 = (double)nClst3/667;
  double n4 = (double)nClst4/667;

  TString ind = "N";
  if (ed == ind){
    cout << "Flip horizontal for narrow end" << endl;
    double temp = n1;
    n1 = n2;
    n2 = temp;
    temp = n3;
    n3 = n4;
    n4 = temp;
  }
  
  cout << endl<< "DBN = " << dbn << endl;
  cout << "Number of fibers = " << nCorrected << endl;
  cout << "#Fibers/#Holes (%)= " << f * 100 << endl;
  cout << "r below (75%) (%) = " << (rgood + rmid + rbad) * 100 <<endl;
  cout << "r below (15%) (%) = " << rbad * 100 << endl;
  cout << "r (15%) - (50%) (%) = " << rmid * 100 << endl;
  cout << "r (50%) - (75%) (%) = " << rgood * 100 << endl;
  cout << "R (%) = " << R * 100 << endl << endl;

  cout << "Bottom Left Quadrant of Wide End has " << n1 * 100 << "(%)" << endl;
  cout << "Bottom Right Quadrant of Wide End has " << n2 * 100 << "(%)" << endl;
  cout << "Top Left Quadrant of Wide End has " << n3 * 100 << "(%)" << endl;
  cout << "Top Right Quadrant of Wide End has " << n4 * 100 << "(%)" << endl;

  
  /*hNfib->Fill(nCorrected);
  hMeanE->Fill( henergy->GetMean() );
  hRMSE->Fill( henergy->GetRMS() );
  hRMSNorm->Fill(  henergy->GetRMS() / henergy->GetMean() ) ;*/
  c0 -> cd(2);

  TLine *linex = new TLine(xc,0,xc,yPixels);
  linex->SetLineColor(kRed);
  linex->Draw();

  TLine *liney = new TLine(0,yc,xPixels,yc);
  liney->SetLineColor(kRed);
  liney->Draw(); 


  //TEllipse *el1 = new TEllipse( xc/2, yc/2, 30);
  //el1->SetLineColor(2);
  //el1->SetLineWidth(5);
  //el1->SetFillStyle(0);
  //el1->Draw();

  //TEllipse *el2 = new TEllipse( xc*3/2, yc/2, 30);
  //el2->SetLineColor(3);
  //el2->SetLineWidth(5);
  //el2->SetFillStyle(0);
  //el2->Draw();

  //TEllipse *el3 = new TEllipse( xc/2, yc*3/2, 30);
  //el3->SetLineColor(5);
  //el3->SetLineWidth(5);
  //el3->SetFillStyle(0);
  //el3->Draw();

  //TEllipse *el4 = new TEllipse( xc*3/2, yc*3/2, 30);
  //el4->SetLineColor(7);
  //el4->SetLineWidth(5);
  //el4->SetFillStyle(0);
  //el4->Draw();


  handsomeTH1(henergy,1);
  //henergy->Draw();
  
  c0->cd(3);
  henergyNorm->Draw("HIST");
  henergyNorm->SetTitleOffset(1.4,"Y");
  drawText(Form("DBN = %s",dbn),0.45,0.8);
  //if (end=="N") drawText("Narrow end",0.45,0.7);
  //if (end=="W") drawText("Wide end",0.45,0.7);
  drawText(Form("# Fibers = %d",nCorrected),0.45,0.6);
  drawText(Form("Fibers (%%) = %1.3f",f * 100),0.45,0.5);
  drawText(Form("rgood (%%) = %1.3f",(1 - rgood) * 100),0.45,0.4);
  drawText(Form("rbad (%%) = %1.3f",rbad * 100),0.45,0.3);
  drawText(Form("R (%%)= %1.3f",R * 100),0.45,0.2);


  //This section is used to set range for viewing only the block surface
  /*int startPixelx = 0;
  int startPixely = 0;
  int endPixelx = 0;
  int endPixely = 0;*/



  c0->cd(4);
  cleverRangeZ(densityInten);
  densityInten->SetTitle("2D Light Intensity Distribution");
  densityInten->Scale(1/densityInten -> GetMaximum());
  densityInten->SetMaximum(1.0);
  //densityInten->SetTitle("Light intensity/Number of Fibers;X;Y");
  densityInten->Draw("colz");

  c0->cd(6);
  cleverRangeZ(seCor);
  gStyle->SetPalette(1);
  seCor->SetTitle("Size Energy Correlation");
  seCor->Scale(1/seCor->GetMaximum());
  seCor->SetMaximum(1.0);
  //densityInten->SetTitle("Light intensity/Number of Fibers;X;Y");
  seCor->Draw("colz");


  c0->cd(5);
  hsizeNorm->Draw("HIST");
  hsizeNorm->SetTitleOffset(1.4,"Y");
  drawText(Form("DBN = %s",dbn),0.45,0.8);
  //if (end=="N") drawText("Narrow end",0.45,0.7);
  //if (end=="W") drawText("Wide end",0.45,0.7);
  drawText(Form("# Fibers = %d",nCorrected),0.45,0.6);
  drawText(Form("Fibers (%%) = %1.3f",f*100),0.45,0.5);
  drawText(Form("rogood (%%) = %1.3f",(1-rgood)*100),0.45,0.4);
  drawText(Form("rbad (%%) = %1.3f",rbad*100),0.45,0.3);
  drawText(Form("R (%%)= %1.3f",R*100),0.45,0.2);

  
  c0->SaveAs(Form("%s/DBN_%s-%s_histograms.pdf",output_folder,dbn,ed));

  std::ifstream infile(output_csv);

  std::string str;
  std::getline(infile, str);
  infile.close();

  std::ofstream outfile;
  outfile.open(output_csv, std::ios_base::app);

  if (str == ""){
    outfile << "DBN" << "," << "End" << "," << "# of Fibers" << "," << "Fiber (%)" << "," << "50-75 (%)" << "," << "15-50 (%)" << "," << "10-15 (%)" << "," << "RMS" << "," << "Tower 1 (%)" << "," << "Tower 2(%)" << "," << "Tower 3(%)" << "," << "Tower 4(%)" << endl;
  }
  outfile << dbn << "," << ed << "," << nCorrected << "," << f*100 << "," << rgood*100 << "," << rmid*100 << "," << rbad*100 << "," << R*100 << "," << n1*100 << "," << n2*100 << "," << n3*100 << "," << n4*100 << endl;
  outfile.close();


  /*
  TFile* fout = new TFile(Form("%s/DBN_%d_outfile.root",folder,dbn),"RECREATE");
  henergy->Write();
  henergyNorm->Write();
  hNfib->Write();
  hMeanE->Write();
  hRMSE->Write();
  hRMSNorm->Write();
  fout->Close();
  */
  
}
 



