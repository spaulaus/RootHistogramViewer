#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include <iostream>

// Adapted from
// https://root-forum.cern.ch/t/read-a-tfile-while-writing-with-another-process/18608/6
// and follow up comment
// https://root-forum.cern.ch/t/read-histogram-tree-from-tfile-while-actively-writing-to-it/28273/10
void ReadHistogram(TFile *file, const unsigned int &id) {
    const char *name = ("h"+std::to_string(id)).c_str();
    delete file->FindObject(name);
    file->ReadKeys();

    TH1D *hist1d = nullptr;
    TH2D *hist2d = nullptr;
    TH3D *hist3d = nullptr;

    file->GetObject(name, hist1d);
    if(hist1d)
        hist1d->Draw();
    else {
        file->GetObject(name, hist2d);
        if(hist2d)
            hist2d->Draw("COLZ");
        else {
            file->GetObject(name, hist3d);
            if(hist3d)
                hist3d->Draw();
            else
                std::cout << "Couldn't figure out how to draw histogram with ID " << id << std::endl;
        }
    }
}

// Adapted from
// https://root-forum.cern.ch/t/read-a-tfile-while-writing-with-another-process/18608/7
void ReadHistogram(const std::string &filename, const int &id) {
    const char *name = ("h"+std::to_string(id)).c_str();
    TFile *file = TFile::Open(filename.c_str());

    TH1D *hist1d = nullptr;
    TH2D *hist2d = nullptr;
    TH3D *hist3d = nullptr;

    file->GetObject(name, hist1d);
    if(hist1d)
        hist1d->Draw();
    else {
        file->GetObject(name, hist2d);
        if(hist2d)
            hist2d->Draw("COLZ");
        else {
            file->GetObject(name, hist3d);
            if(hist3d)
                hist3d->Draw();
            else
                std::cout << "Couldn't figure out how to draw histogram with ID " << id << std::endl;
        }
    }
}