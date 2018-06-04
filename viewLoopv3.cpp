// To compile in Ubuntu:
// g++ viewLoopv3.cpp `root-config --libs --cflags --glibs` -o viewLoopv3
//
// To run as standalone code:
// ./viewLoopv3 "../Analysis/run_001-out-hist.root" 1210 0 1500
//
// Usage in ROOT: 
//  root [0] .x viewLoopv3.C(<filename>, <histID>, minx, maxx)
//  e.g.: root [0] .x viewLoopv3.C("out.root",1209,500,1000)
//
#ifndef __CINT__
#include "TROOT.h"
#include "TApplication.h"
#include "TBrowser.h"
#include "TFile.h"    
#include "TH1F.h"     
#include "TH2F.h"     
#include "TMath.h"
#include "TTree.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TSystem.h" 
#include "TObject.h"
#include "TGraph.h"
#include "TPaveText.h"
#include <TCanvas.h>
#include <TPad.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cstdio>
#include <cstring>
#include <cmath>

void viewLoopv2(const std::string &filename, const int &id, const int minx, const int maxx)
{
  const char *name = ("h" + std::to_string(id)).c_str();
  bool firstt = true;

  TFile *file=TFile::Open(filename.c_str());
  TCanvas *c1 = new TCanvas("c1","Online histogramming",20,10,700,500);
 
  TH1D *hist1d = nullptr;
  TH2D *hist2d = nullptr;
  TH1D *hist1d_draw = new TH1D("hist1d_draw","Histogram to update",15000,minx,maxx);
  TH2D *hist2d_draw = new TH2D("hist2d_draw","Histogram to update",15000,minx,maxx,128,0,128);

  TList *list = new TList;

    while(1) {
        file=TFile::Open(filename.c_str());
        file->GetObject(name, hist1d);

		if(hist1d) {
		    list->Add(hist1d_draw);
			list->Add(hist1d);
		    if(firstt) {
				hist1d_draw->Merge(list);
		        hist1d_draw->GetXaxis()->SetRangeUser(minx,maxx);
				hist1d_draw->Draw();
				firstt = false;
			}
		    hist1d_draw->Reset();
			hist1d_draw->Merge(list);
		    hist1d_draw->Draw();
		    c1->Update();
		    gPad->WaitPrimitive(); 	// waits for double clik before continuing loop...
		    gSystem->ProcessEvents(); 
		}
		else {
	        file->GetObject(name, hist2d);
	        if(hist2d) {
				list->Add(hist2d_draw);
				list->Add(hist2d);
				if(firstt) {
					hist2d_draw->Merge(list);
				    hist2d_draw->GetXaxis()->SetRangeUser(minx,maxx);
					hist2d_draw->Draw("col");
					firstt = false;
				}
				hist2d_draw->Reset();
				hist2d_draw->Merge(list);
				hist2d_draw->Draw("col");
				c1->Update();
				gPad->WaitPrimitive(); 	// waits for double clik before continuing loop...
				gSystem->ProcessEvents(); 
			}
	        else {
	        	std::cout<<"!! Unknown histogram : "<<name<<std::endl;
	        	return;
	        }
		}
    }
}

//----------------------------------------------------------------------------------------------------------------
// the "dressing" code for a stand-alone ROOT application starts here
#ifndef __CINT__
void StandaloneApplication(int argc, char** argv) {
  // ==>> here the ROOT macro is called
  std::string filename = argv[1];
  int id = atoi(argv[2]);
  int minx = atoi(argv[3]);
  int maxx = atoi(argv[4]);
  viewLoopv2(filename, id, minx, maxx);
//  return 0;
}

// This is the standard main of C++ starting a ROOT application
int main(int argc, char** argv) {
   TApplication app("My ROOT Application", &argc, argv);
   StandaloneApplication(app.Argc(), app.Argv());
   app.Run();
   return 0;
}
# endif
