/// @file MainFrame.hpp
/// @brief
/// @author S. V. Paulauskas
/// @date June 02, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#ifndef ISSUE126_MAINFRAME_HPP
#define ISSUE126_MAINFRAME_HPP

#include <TGFrame.h>
#include <TGMenu.h>
#include <TGApplication.h>
#include <TApplication.h>
#include <TGClient.h>
#include <TGTab.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TRootEmbeddedCanvas.h>
#include <TH1S.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TF1.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGFileDialog.h>
#include <TCanvas.h>
#include <TGListBox.h>
#include <TList.h>
#include <TApplication.h>
#include <TFile.h>

///@brief
class MainFrame : public TGMainFrame {
public:
    /// Default Constructor
    MainFrame();

    /// Default Destructor
    ~MainFrame();

    MainFrame(const TGWindow *p, UInt_t w, UInt_t h, TApplication *a);

    void DoExit();

    void PlotSelected();
private:
    TGListBox *fListBox;
    TGCheckButton *fCheckMulti;
    TList *fSelected;
    TApplication *app_;
    TFile *file_;
    TCanvas *canvas_;

ClassDef(MainFrame, 1)
};

#endif //ISSUE126_MAINFRAME_HPP