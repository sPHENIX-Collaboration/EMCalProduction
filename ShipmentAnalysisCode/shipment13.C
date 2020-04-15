void shipment13()
{

  TTree* tree1 = new TTree("tree1","");
  tree1->ReadFile("shipment13.csv","",',');

//Fiber count histogram
  TH1D* fiber_c = new TH1D("Fiber_Percentage",";Fiber_Percentage;",8,96,100); // defines a histogram with number of bins over a specified range
  fiber_c->Sumw2();
  TCanvas*fibercount = new TCanvas("fibercount","",700,500);
  tree1->Draw("FiberCount>>Fiber_Percentage"); //Draw from the tree and fill it into the histograms

  TLine* fibLine = new TLine(98, 0, 98, 29.1); //tolerance line
  fibLine->SetLineColor(kGreen+2); fibLine->SetLineStyle(2); fibLine->SetLineWidth(2); fibLine->Draw();
  TLine* fibLine2 = new TLine(97, 0, 97, 29.1); //tolerance line
  fibLine2->SetLineColor(kOrange+1); fibLine2->SetLineStyle(2); fibLine2->SetLineWidth(2); fibLine2->Draw();

//Tower1 percent histogram
  TH1D* t1 = new TH1D("Tower1_Percentage",";Tower1_Percentage;",8,93,101);
  t1->Sumw2();
  TCanvas*tower1 = new TCanvas("tower1","",700,500);
  tree1->Draw("Tower1>>Tower1_Percentage");

  TLine* t1Line = new TLine(96, 0, 96, 29.1);
  t1Line->SetLineColor(kGreen+2); t1Line->SetLineStyle(2); t1Line->SetLineWidth(2); t1Line->Draw();
  TLine* t1Line2 = new TLine(94, 0, 94, 29.1);
  t1Line2->SetLineColor(kOrange+1); t1Line2->SetLineStyle(2); t1Line2->SetLineWidth(2); t1Line2->Draw();

//Tower2 percent histogram
  TH1D* t2 = new TH1D("Tower2_Percentage",";Tower2_Percentage;",8,93,101);
  t2->Sumw2();
  TCanvas*tower2 = new TCanvas("tower2","",700,500);
  tree1->Draw("Tower2>>Tower2_Percentage");

  TLine* t2Line = new TLine(96, 0, 96, 35);
  t2Line->SetLineColor(kGreen+2); t2Line->SetLineStyle(2); t2Line->SetLineWidth(2); t2Line->Draw();
  TLine* t2Line2 = new TLine(94, 0, 94, 35);
  t2Line2->SetLineColor(kOrange+1); t2Line2->SetLineStyle(2); t2Line2->SetLineWidth(2); t2Line2->Draw();

//Tower3 percent histogram
  TH1D* t3 = new TH1D("Tower3_Percentage",";Tower3_Percentage;",8,93,101);
  t3->Sumw2();
  TCanvas*tower3 = new TCanvas("tower3","",700,500);
  tree1->Draw("Tower3>>Tower3_Percentage");

  TLine* t3Line = new TLine(96, 0, 96, 37.1);
  t3Line->SetLineColor(kGreen+2); t3Line->SetLineStyle(2); t3Line->SetLineWidth(2); t3Line->Draw();
  TLine* t3Line2 = new TLine(94, 0, 94, 37.1);
  t3Line2->SetLineColor(kOrange+1); t3Line2->SetLineStyle(2); t3Line2->SetLineWidth(2); t3Line2->Draw();

//Tower4 percent histogram
  TH1D* t4 = new TH1D("Tower4_Percentage",";Tower4_Percentage;",8,93,101);
  t4->Sumw2();
  TCanvas*tower4 = new TCanvas("tower4","",700,500);
  tree1->Draw("Tower4>>Tower4_Percentage");

  TLine* t4Line = new TLine(96, 0, 96, 36);
  t4Line->SetLineColor(kGreen+2); t4Line->SetLineStyle(2); t4Line->SetLineWidth(2); t4Line->Draw();
  TLine* t4Line2 = new TLine(94, 0, 94, 36);
  t4Line2->SetLineColor(kOrange+1); t4Line2->SetLineStyle(2); t4Line2->SetLineWidth(2); t4Line2->Draw();

//Density histogram
  TH1D* dens = new TH1D("Block_Density",";Block_Density (g/mL);",10,8.7,9.2); // defines a histogram with number of bins over a specified range
  dens->Sumw2(); //error bars
  TCanvas*density = new TCanvas("density","",700,500);
  tree1->Draw("Density>>Block_Density"); //Draw from the tree and fill it into the histograms

  TLine* denLine = new TLine(9, 0, 9, 16.2); //nominal line
  denLine->SetLineColor(kGreen+2); denLine->SetLineStyle(2); denLine->SetLineWidth(2); denLine->Draw();
  TLine* denLine2 = new TLine(8.8, 0, 8.8, 16.2); //5a tolerance line
  denLine2->SetLineColor(kYellow+1); denLine2->SetLineStyle(2); denLine2->SetLineWidth(2); denLine2->Draw();
  TLine* denLine3 = new TLine(8.7, 0, 8.7, 16.2); //5b tolerance line
  denLine3->SetLineColor(kOrange+1); denLine3->SetLineStyle(2); denLine3->SetLineWidth(2); denLine3->Draw();

//DeltaL histogram
  TH1D* deltaL = new TH1D("Delta_Length",";Delta_Length (in);",6,-.02,.01); // defines a histogram with number of bins over a specified range
  deltaL->Sumw2(); //error bars
  deltaL->SetLineWidth(2);
  TCanvas*delta_L = new TCanvas("delta_L","",700,500);
  tree1->Draw("DeltaL>>Delta_Length"); //Draw from the tree and fill it into the histograms

  TLine* delLine1 = new TLine(0,0,0,37.1); //zero line
  delLine1->SetLineColor(2); delLine1->SetLineStyle(2); delLine1->SetLineWidth(2); delLine1->Draw();
  TLine* delLine2 = new TLine(-.03,0,-.03,37.1); //tolerance line
  delLine2->SetLineColor(kGreen+2); delLine2->SetLineStyle(2); delLine2->SetLineWidth(2); delLine2->Draw();
  TLine* delLine3 = new TLine(.03,0,.03,37.1); //tolerance line
  delLine3->SetLineColor(kGreen+2); delLine3->SetLineStyle(2); delLine3->SetLineWidth(2); delLine3->Draw();

//DeltaBT histogram
  TH1D* deltaBT = new TH1D("Delta_BigTop",";Delta_BigTop (in);",6,-.03,.03); // defines a histogram with number of bins over a specified range
  deltaBT->Sumw2(); //error bars
  deltaBT->SetLineWidth(2);
  TCanvas*BT = new TCanvas("BT","",700,500);
  tree1->Draw("DeltaBT>>Delta_BigTop"); //Draw from the tree and fill it into the histograms

  TLine* BTline1 = new TLine(0,0,0,23.5); //zero line
  BTline1->SetLineColor(2); BTline1->SetLineStyle(2); BTline1->SetLineWidth(2); BTline1->Draw();
  TLine* BTline2 = new TLine(-.02,0,-.02,23.5); //tolerance line
  BTline2->SetLineColor(kGreen+2); BTline2->SetLineStyle(2); BTline2->SetLineWidth(2); BTline2->Draw();
  TLine* BTline3 = new TLine(.02,0,.02,23.5); //tolerance line
  BTline3->SetLineColor(kGreen+2); BTline3->SetLineStyle(2); BTline3->SetLineWidth(2); BTline3->Draw();

//DeltaBB histogram
  TH1D* deltaBB = new TH1D("Delta_BigBottom",";Delta_BigBottom (in);",6,-.03,0); // defines a histogram with number of bins over a specified range
  deltaBB->Sumw2(); //error bars
  deltaBB->SetLineWidth(2);
  TCanvas*BB = new TCanvas("BB","",700,500);
  tree1->Draw("DeltaBB>>Delta_BigBottom"); //Draw from the tree and fill it into the histograms

  TLine* BBline1 = new TLine(0,0,0,44); //zero line
  BBline1->SetLineColor(2); BBline1->SetLineStyle(2); BBline1->SetLineWidth(2); BBline1->Draw();
  TLine* BBline2 = new TLine(-.02,0,-.02,44); //tolerance line
  BBline2->SetLineColor(kGreen+2); BBline2->SetLineStyle(2); BBline2->SetLineWidth(2); BBline2->Draw();
  TLine* BBline3 = new TLine(.02,0,.02,44); //tolerance line
  BBline3->SetLineColor(kGreen+2); BBline3->SetLineStyle(2); BBline3->SetLineWidth(2); BBline3->Draw();

//DeltaBH histogram
  TH1D* deltaBH = new TH1D("Delta_BigHeight",";Delta_BigHeight (in);",10,-.01,.01); // defines a histogram with number of bins over a specified range
  deltaBH->Sumw2(); //error bars
  deltaBH->SetLineWidth(2);
  TCanvas*BH = new TCanvas("BH","",700,500);
  tree1->Draw("DeltaBH>>Delta_BigHeight"); //Draw from the tree and fill it into the histograms

  TLine* BHline1 = new TLine(0,0,0,19.8); //zero line
  BHline1->SetLineColor(2); BHline1->SetLineStyle(2); BHline1->SetLineWidth(2); BHline1->Draw();
  TLine* BHline2 = new TLine(-.02,0,-.02,19.8); //tolerance line
  BHline2->SetLineColor(kGreen+2); BHline2->SetLineStyle(2); BHline2->SetLineWidth(2); BHline2->Draw();
  TLine* BHline3 = new TLine(.02,0,.02,19.8); //tolerance line
  BHline3->SetLineColor(kGreen+2); BHline3->SetLineStyle(2); BHline3->SetLineWidth(2); BHline3->Draw();

//DeltaST histogram
  TH1D* deltaST = new TH1D("Delta_SmallTop",";Delta_SmallTop (in);",6,-.03,.03); // defines a histogram with number of bins over a specified range
  deltaST->Sumw2(); //error bars
  deltaST->SetLineWidth(2);
  TCanvas*ST = new TCanvas("ST","",700,500);
  tree1->Draw("DeltaST>>Delta_SmallTop"); //Draw from the tree and fill it into the histograms

  TLine* STline1 = new TLine(0,0,0,24.5); //zero line
  STline1->SetLineColor(2); STline1->SetLineStyle(2); STline1->SetLineWidth(2); STline1->Draw();
  TLine* STline2 = new TLine(-.02,0,-.02,24.5); //tolerance line
  STline2->SetLineColor(kGreen+2); STline2->SetLineStyle(2); STline2->SetLineWidth(2); STline2->Draw();
  TLine* STline3 = new TLine(.02,0,.02,24.5); //tolerance line
  STline3->SetLineColor(kGreen+2); STline3->SetLineStyle(2); STline3->SetLineWidth(2); STline3->Draw();

//DeltaSB histogram
  TH1D* deltaSB = new TH1D("Delta_SmallBottom",";Delta_SmallBottom (in);",8,-.03,.01); // defines a histogram with number of bins over a specified range
  deltaSB->Sumw2(); //error bars
  deltaSB->SetLineWidth(2);
  TCanvas*SB = new TCanvas("SB","",700,500);
  tree1->Draw("DeltaSB>>Delta_SmallBottom"); //Draw from the tree and fill it into the histograms

  TLine* SBline1 = new TLine(0,0,0,41.2); //zero line
  SBline1->SetLineColor(2); SBline1->SetLineStyle(2); SBline1->SetLineWidth(2); SBline1->Draw();
  TLine* SBline2 = new TLine(-.02,0,-.02,41.2); //tolerance line
  SBline2->SetLineColor(kGreen+2); SBline2->SetLineStyle(2); SBline2->SetLineWidth(2); SBline2->Draw();
  TLine* SBline3 = new TLine(.02,0,.02,41.2); //tolerance line
  SBline3->SetLineColor(kGreen+2); SBline3->SetLineStyle(2); SBline3->SetLineWidth(2); SBline3->Draw();

//DeltaSH histogram
  TH1D* deltaSH = new TH1D("Delta_SmallHeight",";Delta_SmallHeight (in);",6,-.03,.03); // defines a histogram with number of bins over a specified range
  deltaSH->Sumw2(); //error bars
  deltaSH->SetLineWidth(2);
  TCanvas*SH = new TCanvas("SH","",700,500);
  tree1->Draw("DeltaSH>>Delta_SmallHeight"); //Draw from the tree and fill it into the histograms

  TLine* SHline1 = new TLine(0,0,0,28); //zero line
  SHline1->SetLineColor(2); SHline1->SetLineStyle(2); SHline1->SetLineWidth(2); SHline1->Draw();
  TLine* SHline2 = new TLine(-.02,0,-.02,28); //tolerance line
  SHline2->SetLineColor(kGreen+2); SHline2->SetLineStyle(2); SHline2->SetLineWidth(2); SHline2->Draw();
  TLine* SHline3 = new TLine(.02,0,.02,28); //tolerance line
  SHline3->SetLineColor(kGreen+2); SHline3->SetLineStyle(2); SHline3->SetLineWidth(2); SHline3->Draw();

//Scintillation plot
  TH1D* scintillation = new TH1D("Scintillation_Ratio",";Scintillation_Ratio;",10,.5,3);
  scintillation->Sumw2();
  scintillation->SetLineWidth(2);
  TCanvas*scint = new TCanvas("scint","",700,500);
  tree1->Draw("Scint_ratio>>Scintillation_Ratio");

  TLine* scinLine = new TLine(.7,0,.7,13.9);
  scinLine->SetLineColor(kGreen+2); scinLine->SetLineStyle(2); scinLine->SetLineWidth(2); scinLine->Draw();  

//fibercount->SaveAs("plots/fibercount.png");
//tower1->SaveAs("plots/tower1.png");
//tower2->SaveAs("plots/tower2.png");
//tower3->SaveAs("plots/tower3.png");
//tower4->SaveAs("plots/tower4.png");
//delta_L->SaveAs("plots/deltaL.png");
//BT->SaveAs("plots/deltaBT.png");
//BB->SaveAs("plots/deltaBB.png");
//BH->SaveAs("plots/deltaBH.png");
//ST->SaveAs("plots/deltaST.png");
//SB->SaveAs("plots/deltaSB.png");
//SH->SaveAs("plots/deltaSH.png");
//scint->SaveAs("plots/scint.png");
}

