/// @file MainFrame.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date June 02, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "MainFrame.hpp"

#include <TKey.h>
#include <TH2D.h>
#include <TH3D.h>

ClassImp(MainFrame)

MainFrame::MainFrame() = default;

MainFrame::~MainFrame() {
    // Clean up main frame...
    Cleanup();
    if (fSelected) {
        fSelected->Delete();
        delete fSelected;
    }
}

MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h, TApplication *a) : TGMainFrame(p, w, h) {
    app_ = a;
    file_ = new TFile("../issue126-pulser.root");
    canvas_ = new TCanvas("canvas", "");

    canvas_->ToggleEditor();
    canvas_->ToggleEventStatus();
    canvas_->ToggleToolBar();

    fListBox = new TGListBox(this, 89);
    fSelected = new TList;
    char tmp[40];
    unsigned int counter = 0;
    for (auto &&keyAsTObj : *(file_->GetListOfKeys())) {
        auto key = (TKey*) keyAsTObj;

        if(key->IsFolder()) //TTrees are registered as folders, we want to skip those.
            continue;

        auto obj = key->ReadObj();
        fListBox->AddEntry(obj->GetName(), counter + 1);
        counter++;
    }
    fListBox->Resize(300, 150);
    AddFrame(fListBox, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

    // Create a horizontal frame containing button(s)
    auto *hframe = new TGHorizontalFrame(this, 150, 20, kFixedWidth);
    auto *show = new TGTextButton(hframe, "&Show");

    show->SetToolTipText("Click here to print the selection you made");
    show->Connect("Pressed()", "MainFrame", this, "PlotSelected()");
    hframe->AddFrame(show, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));

    auto *exit = new TGTextButton(hframe, "&Exit ");
    exit->Connect("Pressed()", "MainFrame", this, "DoExit()");

    hframe->AddFrame(exit, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
    AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    // Set a name to the main frame
    SetWindowName("List Box");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();
    fListBox->Select(1);
}

void MainFrame::DoExit() {
    app_->Terminate(0);
}

void MainFrame::PlotSelected() {
    fSelected->Clear();
    auto name = fListBox->GetEntry(fListBox->GetSelected())->GetTitle();

    delete file_->FindObject(name);
    file_->ReadKeys();

    TH1D *hist1d = nullptr;
    TH2D *hist2d = nullptr;
    TH3D *hist3d = nullptr;

    canvas_->cd();
    file_->GetObject(name, hist1d);
    if(hist1d) {
        hist1d->Draw();
    } else {
        file_->GetObject(name, hist2d);
        if(hist2d)
            hist2d->Draw("COLZ");
        else {
            file_->GetObject(name, hist3d);
            if(hist3d)
                hist3d->Draw("COLZ");
            else
                std::cout << "Couldn't figure out how to draw histogram with ID " << name << std::endl;
        }
    }
    canvas_->Update();
}