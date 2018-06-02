/// @file MainFrame.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date June 02, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "MainFrame.hpp"

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

void MainFrame::Initialize(TApplication *app) {
    app_ = app;
    file_ = new TFile("../issue126-pulser.root");
    canvas_ = new TCanvas("canvas", "");
    hist1d = nullptr;
    hist2d = nullptr;
    hist3d = nullptr;

    canvas_->ToggleEditor();
    canvas_->ToggleEventStatus();
    canvas_->ToggleToolBar();

    fListBox = new TGListBox(this, 89);
    fSelected = new TList;
}

void MainFrame::PopulateHistogramList() {
    for (auto &&keyAsTObj : *(file_->GetListOfKeys())) {
        auto key = (TKey *) keyAsTObj;

        if (key->IsFolder()) //TTrees are registered as folders, we want to skip those.
            continue;

        auto obj = key->ReadObj();
        fListBox->AddEntry(obj->GetTitle(), std::stoi(std::string(obj->GetName(), 1, std::string::npos)));
    }
}

void MainFrame::AddHistogramListToFrame() {
    fListBox->Resize(frameWidth_, histogramListHeight_);
    AddFrame(fListBox, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
}

void MainFrame::SetupControlButtons() {
    // Create a horizontal frame containing button(s)
    auto *hframe = new TGHorizontalFrame(this, frameWidth_, 20, kFixedWidth);

    auto *show = new TGTextButton(hframe, "&Show");
    show->SetToolTipText("Click here to plot the selected histogram");
    show->Connect("Pressed()", "MainFrame", this, "PlotSelected()");
    hframe->AddFrame(show, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));

    auto *exit = new TGTextButton(hframe, "&Exit ");
    exit->Connect("Pressed()", "MainFrame", this, "Exit()");
    hframe->AddFrame(exit, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));

    AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); //Expands the buttons to fill the width
    AddFrame(hframe, new TGLayoutHints(kLHintsBottom | kLHintsRight, 2, 2, 5, 1)); // positions buttons bottom right
}

MainFrame::MainFrame(const TGWindow *p, TApplication *app) : TGMainFrame(p) {
    Initialize(app);
    PopulateHistogramList();
    AddHistogramListToFrame();

    auto boundsLabel = new TGLabel(this, "Histogram Bounds");
    AddFrame(boundsLabel,  new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));

    auto *boundsFrame = new TGHorizontalFrame(this, frameWidth_, 20, kFixedWidth);

    auto lowBoundLabel = new TGLabel(this, "Low :");
    boundsFrame->AddFrame(lowBoundLabel, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

    fNumber = new TGNumberEntry(this, 0, 9, 999, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,
                                TGNumberFormat::kNELLimitMinMax, -99999, 99999);
    fNumber->Connect("ValueSet(Long_t)", "MainFrame", this, "SetHistogramBounds()");
    (fNumber->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "SetHistogramBounds()");
    boundsFrame->AddFrame(fNumber, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

    auto highBoundLabel = new TGLabel(this, "High :");
    boundsFrame->AddFrame(highBoundLabel, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

    fNumber1 = new TGNumberEntry(this, 0, 9, 999, TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,
                                TGNumberFormat::kNELLimitMinMax, -99999, 99999);
    fNumber1->Connect("ValueSet(Long_t)", "MainFrame", this, "SetHistogramBounds()");
    (fNumber->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "SetHistogramBounds()");
    boundsFrame->AddFrame(fNumber, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

    AddFrame(boundsFrame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    AddFrame(boundsFrame, new TGLayoutHints(kLHintsBottom, 2, 2, 2, 2));

    SetupControlButtons();

    SetWindowName(frameName_);
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

void MainFrame::Exit() {
    app_->Terminate(0);
}

void MainFrame::PlotSelected() {
    fSelected->Clear();
    auto name = ("h" + std::to_string(fListBox->GetSelected())).c_str();

    delete file_->FindObject(name);
    file_->ReadKeys();

    canvas_->cd();
    file_->GetObject(name, hist1d);
    if (hist1d) {
        hist1d->Draw();
    } else {
        file_->GetObject(name, hist2d);
        if (hist2d)
            hist2d->Draw("COLZ");
        else {
            file_->GetObject(name, hist3d);
            if (hist3d)
                hist3d->Draw("COLZ");
            else
                std::cout << "Couldn't figure out how to draw histogram with ID " << name << std::endl;
        }
    }
    canvas_->Update();
}

void MainFrame::SetHistogramBounds() {
    // Slot method connected to the ValueSet(Long_t) signal.
    // It displays the value set in TGNumberEntry widget.
    std::cout << fNumber->GetNumberEntry()->GetIntNumber() << std::endl;
}