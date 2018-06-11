// To compile in Ubuntu:
// g++ viewLoopv3.cpp `root-config --libs --cflags --glibs` -o viewLoopv3
//
// To run as standalone code:
// ./viewLoopv3 "../Analysis/run_001-out-hist.root" 1210
//
// Usage in ROOT: 
//  root [0] .x viewLoopv3.C(<filename>, <histID>)
//  e.g.: root [0] .x viewLoopv3.C("out.root",1209)
//
// Edited by JJ van Zyl, 6 jun 2018.
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
#include "TStyle.h"
#include "TSystem.h" 
#include "TObject.h"
#include "TGraph.h"
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

void viewLoopv3(const std::string &filename, const int &id)
{
    const char *name = ("h" + std::to_string(id)).c_str();

    TFile *file=TFile::Open(filename.c_str());
    TCanvas *c1 = new TCanvas("c1","Online histogramming",20,10,700,500);
 
    TH1D *hist1d = nullptr;
    TH2D *hist2d = nullptr;
    TH1D *hist1d_draw = new TH1D(name,"Histogram to update",16384,0,16384);
    TH2D *hist2d_draw = new TH2D(name,"Histogram to update",16384,0,16384,128,0,128);

    TList *list = new TList;

    while(1) {
        file=TFile::Open(filename.c_str());
        file->GetObject(name, hist1d);

	if(hist1d) {			
    	    hist1d_draw->Reset();
    	    list->Clear();
            list->Add(hist1d_draw);
	    list->Add(hist1d);
	    hist1d_draw->Merge(list);
	    hist1d_draw->Draw();
	    c1->Update();

	    std::cout<<"\n **** UPDATE: Double-click on histogram pad... ****\n"<< 
		" **** Quit: Press 'Quit ROOT' from the File menu... ****\n"<<
		" **** Axes: Use SetRangeUser in axes options to change the axis range... ****"<<std::endl;
	    gPad->WaitPrimitive(); 	// waits for double clik before continuing loop...
	    gSystem->ProcessEvents(); 
        }
        else {
            file->GetObject(name, hist2d);
            if(hist2d) {
	        hist2d_draw->Reset();
     	        list->Clear();
	        list->Add(hist2d_draw);
	        list->Add(hist2d);
	        hist2d_draw->Merge(list);
	        hist2d_draw->Draw("col");
	        c1->Update();

		std::cout<<"\n **** UPDATE: Double-click on histogram pad... ****\n"<< 
		    " **** Quit: Press 'Quit ROOT' from the File menu... ****\n"<<
		    " **** Axes: Use SetRangeUser in axes options to change the axis range... ****"<<std::endl;
	        gPad->WaitPrimitive(); 	// waits for double clik before continuing loop...
	        gSystem->ProcessEvents(); 
            }
            else {
	        std::cout<<"!! Unknown histogram ID : "<< name <<std::endl;
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------
#ifndef __CINT__
void StandaloneApplication(int argc, char** argv) {
// ==>> here the ROOT macro is called
    std::string filename = argv[1];
    int id = atoi(argv[2]);
    viewLoopv3(filename, id);
}

int main(int argc, char** argv) {
    TApplication app("My ROOT Application", &argc, argv);
    StandaloneApplication(app.Argc(), app.Argv());
    app.Run();
    return 0;
}
# endif
