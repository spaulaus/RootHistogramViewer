///@file example-28337.C
///@brief A program that will generate data and asynchronously write it to disk, then crash when the TTree::Write()
/// calls TTree::AutoSave()
/// https://root-forum.cern.ch/t/disable-ttree-autosave-when-calling-ttree-fill/28337
///@author S. V. Paulauskas
///@date March 16, 2018
///@copyright Copyright (c) 2018 S. V. Paulauskas.
///@copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TNtuple.h>

#include <iostream>
#include <mutex>
#include <random>
#include <thread>

struct DataStructure {
    double px;
    double py;
    double pz;
    double random;
    int i;
};

void AsyncFlush(TH1D *hist, TTree *tree, std::mutex *lock) {
    tree->AutoSave("overwrite");
    //hist->Write(nullptr, TObject::kWriteDelete);
    lock->unlock();
}

void WriteToDisk(TH1D *hist, TTree *tree, std::mutex *lock) {
    if (lock->try_lock()) {
        std::thread worker0(AsyncFlush, hist, tree, lock);
        worker0.detach();
    }
}

void GenerateData() {
    std::mutex lock;
    TFile *f = new TFile("test.root", "RECREATE");
    TH1D *hist = new TH1D("hist", "", 100, -2, 2);
    TTree *tree = new TTree("tree", "");
    static DataStructure data;
    tree->Branch("data", &data, "px/D:py:pz:random:i/I");
    tree->SetAutoSave(0);
    tree->SetAutoFlush(0);

    static std::default_random_engine generator;
    static std::normal_distribution<double> distribution(0.0, 1.0);

    unsigned int loopCounter = 0;
    while (loopCounter < 1000) {
        if (loopCounter % 100 == 0 || loopCounter == 999)
            cout << "Working on loop number " << loopCounter << endl;
        hist->FillRandom("gaus", 10000);
        WriteToDisk(hist, tree, &lock);
        loopCounter++;

        for (int i = 0; i < 500; i++) {
            data.px = distribution(generator);
            data.py = distribution(generator);
            data.random = distribution(generator);
            data.pz = data.px * data.px + data.py * data.py;
            data.i = i;
            tree->Fill();
        }
    }

    while (!lock.try_lock())
        sleep(1);

    tree->AutoSave("overwrite");
    sleep(2);
    //hist->Write();
    //f->Write(nullptr, TObject::kWriteDelete);
    f->Close();
}

// Adapted from
// https://root-forum.cern.ch/t/read-a-tfile-while-writing-with-another-process/18608/6
// and follow up comment
// https://root-forum.cern.ch/t/read-histogram-tree-from-tfile-while-actively-writing-to-it/28273/10
void ReadHistogram(TFile *file, const char *name = "hist") {
    TH1D *hist1 = nullptr;
    delete file->FindObject(name);
    file->ReadKeys();
    file->GetObject(name, hist1);
    if (hist1)
        hist1->Draw();
}

// Adapted from
// https://root-forum.cern.ch/t/read-a-tfile-while-writing-with-another-process/18608/7
void ReadHistogram() {
    TFile *file = TFile::Open("test.root");
    TH1D *hist = nullptr;
    file->GetObject("hist", hist);
    if (hist)
        hist->Draw();
}

void ReadTree() {

}