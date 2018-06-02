// Compile with:
// g++ code_GUI.cpp -Wall `root-config --libs --cflags` -o code_GUI

// include ALL header files needed
#ifndef __CINT__
#include "TROOT.h"
#include "TApplication.h"
#include "TBrowser.h"
#include "TFile.h"    
#include "TH1F.h"     
#include "TCanvas.h"  
#include "TMath.h"
#include "TH2F.h"     
#include "TTree.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TSystem.h" 
#include "TObject.h"
#include "TWbox.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TGInputDialog.h"
#include "TGClient.h"
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <cstring>
#include <cmath>



// GLOBAL VARIABLES:

void NextHisto(const char *command)
{
   std::cout<<command<<std::endl;
   if (command[0]=='q') gApplication->Terminate(0);

}


void UserInput() 
{ 
  char command[80]; 
  char *text;
  TH1F *h1 = 0;

  h1 = new TH1F("h1", "histo from a gaussian", 100, -3, 3); 
  h1->FillRandom("gaus", 10000); 
  h1->Draw(); 
  printf("\nDouble click in the bottom right corner of the pad to continue\n"); 
//  std::cin>>text;
  
   gApplication->ReturnPressed(text);
//  gApplication->Connect("ReturnPressed(const char*)", 0, 0, "NextHisto(const char*)");
//   if (gSystem->ProcessEvents()) gApplication->Terminate(0); 
//   gApplication->ReturnPressed(text);
//  gApplication->Terminate(0);

  while (1) { 
      h1->FillRandom("gaus", 10000); 
      gPad->Modified(); 
      gPad->Update(); 
      gPad->WaitPrimitive(); 	// waits for double clik before continuing loop...
//      new TGInputDialog(gClient->GetRoot(), gClient->GetRoot(), "Please type a command:", "q", command); 
      std::cout<<command<<std::endl; 
      if (command[0]=='q') gApplication->Terminate(0); 
  } 
}
 
//     ==>>  put the code of your macro here 
void code_GUI() { 
// VARIABLES:     
	const Int_t NN=8;
    Float_t xd[NN]={25.,30.,35.,40.,45.,50.,55.,60.};
	Float_t qtrd[NN];

	Float_t E_a=100., Estar=10.0;
	Char_t text1[128];
	Float_t rg;


	TRandom3 *rgaus = new TRandom3(0);

	rg = rgaus->Rndm();


	TCanvas *JJC=new TCanvas("JJC","code histogram",20,20,630,430);

	std::cout <<std::endl;
	std::cout << "\n *********************************************"<< std::endl;
	std::cout << "        Stuff - results        "<< std::endl;
	std::cout << " *********************************************"<< std::endl;
	std::cout <<std::endl;

// **********************************************************************************

//Drawing the function:

	for( Int_t i=0; i<NN; i++) {
		rg = rgaus->Rndm();
		qtrd[i]=rg*xd[i];
	}
		  
	gStyle->SetOptLogy(1); 
	TGraph *gr_d = new TGraph(8,xd,qtrd);
	gr_d->SetTitle("Graph of q-transfer vs. scattering angle");
	gr_d->GetXaxis()->SetTitle("#theta [deg]");
	gr_d->GetYaxis()->SetTitle("q-transfer [fm^{-1}]");
	gr_d->GetXaxis()->SetTitleOffset(1.2); 
	gr_d->GetYaxis()->SetTitleOffset(1.5); 
	gr_d->GetXaxis()->CenterTitle();
	gr_d->GetYaxis()->CenterTitle();
	gr_d->SetMarkerStyle(20);
	gr_d->SetMarkerSize(0.7);
	gr_d->Draw("AP");
// gr_sig->Draw("AC");
 
    // Some details on graphs:
	TPaveText *leg1=new TPaveText(0.35,0.82,0.45,0.90,"NDC");
	sprintf(text1,"E_{in} = %.0f MeV, E* = %4.3f MeV",E_a,Estar);
	leg1->SetTextSize(0.035);
	leg1->SetTextFont(42);
	leg1->SetBorderSize(0);
	leg1->SetFillColor(0);
	leg1->SetFillStyle(0);
	leg1->AddText(text1);
	leg1->Draw();
	JJC->Update();

	std::cout <<   " Press Ctrl+C to exit this code..."<< std::endl;
	std::cout <<   " ... or from the canvas menue, File -> Quit ROOT ..."<< std::endl;

    UserInput();

}

// the "dressing" code for a stand-alone ROOT application starts here
#ifndef __CINT__
void StandaloneApplication(int argc, char** argv) {
  // ==>> here the ROOT macro is called
  code_GUI();
}

// This is the standard main of C++ starting a ROOT application
int main(int argc, char** argv) {
   TApplication app("My ROOT Application", &argc, argv);
   StandaloneApplication(app.Argc(), app.Argv());
   app.Run();
   return 0;
}
#endif
/*
char answer[128]; 
strcpy(answer, "(empty)"); 
new TGInputDialog(gClient->GetRoot(), GetMainFrame(), "Enter directory name:", answer"(empty)", answer); 
while (strcmp(answer, "(empty)") == 0) { 
	new TGMsgBox(gClient->GetRoot(), GetMainFrame(), "Error", "Please enter a valid directory name.", kMBIconStop, kMBOk); 
	new TGInputDialog(gClient->GetRoot(), GetMainFrame(), "Enter directory name:", answer, answer); 
} 
if (strcmp(answer, "") == 0) { 
// Cancel button was pressed 
} else { 
// your code here... 
}
*/
