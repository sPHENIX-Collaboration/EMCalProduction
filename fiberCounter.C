//sPHENIX EMCal fiber counting software
//Authors: Yongsun Kim, Anabel Romero， Xiaoning Wang
//Based on a macro by Olivier Couet that converts an image to a 2D histogram

#include <vector>
#include "commonUtility.h"
#include <TASImage.h>
#include <TEllipse.h>
#include <fstream>

void fiberCounter(int dbn = 42, const char* end = "N", const char * input_folder = "sector0_small", const char * output_folder = "sector0_small", const char * output_csv = "sector0_small/result.csv"){

  //short seedThr = 100;
  //short bkgThr = 80;
  short absBkg = 30;
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
  
  TString picture = Form("%s/DBN_%d-%s.JPG",input_folder,dbn,end);
  TASImage image(picture);
 
  UInt_t yPixels = image.GetHeight();
  UInt_t xPixels = image.GetWidth();
  cout << "xPixels = " << xPixels << endl;
  cout << "yPixels = " << yPixels << endl;
  
  /*if ( (maxX<xPixels) || ( maxY <yPixels) )   {
    cout <<"  (maxX<xPixels) || ( maxY <yPixels) ! " << endl;
    return;
  }*/
  
  TH1D* hNfib = new TH1D("hNfib",";Number of clusters;Entries",4000,0,4000);

  UInt_t *argb   = image.GetArgbArray();
  
  TH2D* hOriginal = new TH2D("hOriginal","Spectrum of Green Light Picture",xPixels,.5,xPixels+1,yPixels+1,.5,yPixels);
  TH2D* h = hOriginal;

  TH1D* h1d = new TH1D("h1d","Light Intensity of Pixels;Intensity of Pixels;Counts",256,0,256);
  TH1D* henergy = new TH1D(Form("henergy_%d",dbn),"Intensity of clusters;Intensity;Counts",50,0,25000);
  TH1D* henergyNorm = new TH1D(Form("henergyNorm_%d",dbn),"Light Intensity of Fibers;Self-normalized Intensity of Fibers;Counts",50,0,3);
  TH1D* hsize = new TH1D(Form("hsize_%d",dbn),"Size of fibers;Pixels;Counts",50,0,150);
  TH1D* hsizeNorm = new TH1D(Form("hsizeNorm_%d",dbn),"Size of Fibers;Self-normalized Size of Fibers;Counts",50,0,3);
  //TH1D* hMeanE = (TH1D*)henergy->Clone("hMeanE");
  //hMeanE->SetXTitle("<cluster intensity>");

  //TH1D* hRMSE = new TH1D("hRMSE",";RMS;Entries",1000,0,100000);
  //TH1D* hRMSNorm = new TH1D("hRMSNorm",";RMS normalized by mean;Counts",1000,0,1);

  TH2D* seCor = new TH2D("seCorrelation","Intensity Size Correlation;Self-normalized Size;Self-normalized Intensity",50,0,3,50,0,3);

  TH2D* gFibers = new TH2D("nFibers","Number of Fibers",25,0,xPixels, 25, 0, yPixels);
  TH2D* gInten = (TH2D*)gFibers->Clone("gInten");
  TH2D* densityInten   = (TH2D*)gFibers->Clone("densityInten");
  //TH2D* gNpix  = (TH2D*)gFibers->Clone("gNpix");
  //TH2D* densityNpix   = (TH2D*)gFibers->Clone("densityNpix");

  //TH1D* hDefDist = new TH1D("hDefDist",";RMS xy normalized by sqrt(area);", 200,0,200);


  h1d->Sumw2();
  for (int row=0; row<xPixels; ++row) {
    for (int col=0; col<yPixels; ++col) {
      int index = col*xPixels+row;
      short grey = argb[index]&0xff ;
      h->SetBinContent(row+1,yPixels-col,grey);
      h1d->Fill(grey);
    }
  }
  
  TCanvas* c0 = new TCanvas("c0","",700,700);
  c0->Divide(2,3);
  c0->cd(1);
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
  //c0->cd(2);
  //h->SetAxisRange(0,256,"z");
  //h->Draw("colz");
  //gPad->SetRightMargin(0.2);
     
   
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

  
  // Clean up backgrounds ;
  short **arrInten = new short*[xPixels+1];
  short **arrFlag = new short*[xPixels+1];
  short **arrLocThr = new short*[xPixels+1];
  for (int i = 0; i < xPixels+1;i++)
      {
      arrInten[i] = new short[yPixels+1];
      arrFlag[i] = new short[yPixels+1];
      arrLocThr[i] = new short[yPixels+1];
      }

  int kIn=1;  int kOut=-1;   int kUnde=0; 
  int nXbins = h3->GetNbinsX() ; 
  int nYbins = h3->GetNbinsY() ; 

  for ( int ix0=1 ; ix0<=nXbins ; ix0++) {
    for ( int iy0=1 ; iy0<=nYbins ; iy0++) {
      arrLocThr[ix0][iy0] = bkgThr;

      short val0 = h3->GetBinContent(ix0,iy0);
      arrInten[ix0][iy0] = val0;
      arrFlag[ix0][iy0] = kUnde;	
      if (val0 < bkgThr )  {
		h3->SetBinContent(ix0, iy0, zeroInt);
		arrFlag[ix0][iy0] =  kOut;
      }
      
    }
  }
  
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

  int aveEnergy = (henergy->GetMaximumBin())*(henergy->GetBinWidth(henergy->GetMaximumBin()));
  //cout << "<Most occupied energy is " << aveEnergy;
  int aveSize = (hsize->GetMaximumBin())*(hsize->GetBinWidth(hsize->GetMaximumBin()));
  cout << "Most occupied size is " << aveSize << endl;
  int ngood = 0,ndrop = 0, nbad = 0, nmid = 0, nCorrected = nClst;
  for ( int ci = 0 ; ci< vClstE.size() ; ci++) {
	  double energy = (double) vClstE[ci] /aveEnergy;
    double thisSize = (double) vClstS[ci] /aveSize;
    int multi = 1;
    while (thisSize > (multi+0.6)){
      cout << "found bundle" << endl;
      multi++;
    }
    energy = (double)energy/multi;
    thisSize = (double)thisSize/multi;
    if ((energy < dropBar) || (thisSize < dropBar)) {
      ndrop++; 
      cout << "found drop" << endl;
      nCorrected = nCorrected-1; 
      continue;
    }
    nCorrected = nCorrected + multi - 1;
    if (energy < badBar) {nbad++;}
    if ((energy >= badBar) && (energy < midBar)) {nmid++;}
    if ((energy >= midBar) && (energy < goodBar)) {ngood++;}
	  henergyNorm -> Fill(energy, multi);
    hsizeNorm -> Fill(thisSize, multi); 
    seCor -> Fill(thisSize, energy, multi);
  }
  
  double f = nCorrected/2668.;
  double rgood = (double)(ngood)/nCorrected;
  double R = henergyNorm->GetRMS()/henergyNorm->GetMean();
  double rbad = (double)(nbad)/nCorrected;
  double rmid = (double)(nmid)/nCorrected;
  
  cout << endl<< "DBN = " << dbn << endl;
  cout << "Number of fibers = " << nCorrected << endl;
  cout << "#Fibers/#Holes (%)= " << f*100 << endl;
  cout << "r below (75%) (%) = " << (rgood+rmid+rbad)*100 <<endl;
  cout << "r below (15%) (%) = " << rbad*100 << endl;
  cout << "r (15%) - (50%) (%) = " << rmid*100 << endl;
  cout << "r (50%) - (75%) (%) = " << rgood*100 << endl;
  cout << "R (%) = " << R*100 <<endl<<endl;

  
  /*hNfib->Fill(nCorrected);
  hMeanE->Fill( henergy->GetMean() );
  hRMSE->Fill( henergy->GetRMS() );
  hRMSNorm->Fill(  henergy->GetRMS() / henergy->GetMean() ) ;*/
  

  c0->cd(2);
  h->SetAxisRange(0,256,"z");
  h->Draw("colz");
  gPad->SetRightMargin(0.11);
  for ( int ci = 0 ; ci< vClstX.size() ; ci++) {
    TEllipse *el3 = new TEllipse( vClstX[ci], vClstY[ci], 5);
    el3->SetLineColor(kBlack);
    el3->SetLineWidth(1);
    el3->SetFillStyle(0);
    el3->Draw();
  }


  handsomeTH1(henergy,1);
  //henergy->Draw();
  
  c0->cd(3);
  henergyNorm->Draw("HIST");
  henergyNorm->SetTitleOffset(1.4,"Y");
  drawText(Form("DBN = %d",dbn),0.45,0.8);
  //if (end=="N") drawText("Narrow end",0.45,0.7);
  //if (end=="W") drawText("Wide end",0.45,0.7);
  drawText(Form("# Fibers = %d",nCorrected),0.45,0.6);
  drawText(Form("Fibers (%%) = %1.3f",f*100),0.45,0.5);
  drawText(Form("rgood (%%) = %1.3f",(1-rgood)*100),0.45,0.4);
  drawText(Form("rbad (%%) = %1.3f",rbad*100),0.45,0.3);
  drawText(Form("R (%%)= %1.3f",R*100),0.45,0.2);


  //This section is used to set range for viewing only the block surface
  /*int startPixelx = 0;
  int startPixely = 0;
  int endPixelx = 0;
  int endPixely = 0;*/



  c0->cd(4);
  cleverRangeZ(densityInten);
  densityInten->SetTitle("2D Light Intensity Distribution");
  densityInten->Scale(1/densityInten->GetMaximum());
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
  drawText(Form("DBN = %d",dbn),0.45,0.8);
  //if (end=="N") drawText("Narrow end",0.45,0.7);
  //if (end=="W") drawText("Wide end",0.45,0.7);
  drawText(Form("# Fibers = %d",nCorrected),0.45,0.6);
  drawText(Form("Fibers (%%) = %1.3f",f*100),0.45,0.5);
  drawText(Form("rogood (%%) = %1.3f",(1-rgood)*100),0.45,0.4);
  drawText(Form("rbad (%%) = %1.3f",rbad*100),0.45,0.3);
  drawText(Form("R (%%)= %1.3f",R*100),0.45,0.2);



  
  c0->SaveAs(Form("%s/DBN_%d-%s_histograms.pdf",output_folder,dbn,end));


  std::ifstream infile(output_csv);
  std::ofstream outfile;
  outfile.open(output_csv, std::ios_base::app);

  std::string str;
  std::getline(infile, str);

  if (str == ""){
    outfile << "DBN" << "," << "End" << "," << "# of Fibers" << "," << "Fiber (%)" << "," << "50-75 (%)" << "," << "15-50 (%)" << "," << "10-15 (%)" << "," << "RMS" << endl;
  }
  outfile << dbn << "," << end << "," << nCorrected << "," << f*100 << "," << rgood*100 << "," << rmid*100 << "," << rbad*100 << "," << R*100 << endl;
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
 



