///@file example-28273.C
///@brief A program that will generate data and asynchronously write it to disk.
/// https://root-forum.cern.ch/t/read-histogram-tree-from-tfile-while-actively-writing-to-it-with-example/28273
///@author S. V. Paulauskas
///@date March 12, 2018
///@copyright Copyright (c) 2018 S. V. Paulauskas.
///@copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include <TFile.h>
#include <TH1D.h>

#include <iostream>
#include <mutex>
#include <thread>

void AsyncFlush(TFile *file, std::mutex *lock, unsigned int *loopId) {
    std::cout << "AsyncFlush - Now calling file->Write" << std::endl;
    file->Write(0, TObject::kWriteDelete);
    std::cout << "AsyncFlush - Now unlocking the file for writing in loop " << *loopId << std::endl;
    lock->unlock();
}

void WriteToDisk(TFile *file, unsigned int *loopID, std::mutex *lock) {
    if (lock->try_lock()) {
        std::cout << "WriteToDisk - we're creating the new thread in loop number " << *loopID << endl;
        std::thread worker0(AsyncFlush, file, lock, loopID);
        worker0.detach();
    }
}

void GenerateHistogram() {
    std::mutex lock;
    TFile *f = new TFile("test.root", "RECREATE");
    TH1D *hist = new TH1D("hist", "", 100, -2, 2);
    unsigned int loopCounter = 0;
    while (loopCounter < 40000) {
        hist->FillRandom("gaus", 10000);
        WriteToDisk(f, &loopCounter, &lock);
        loopCounter++;
    }
    while(!lock.try_lock())
        sleep(1);
    f->Write(nullptr, TObject::kWriteDelete);
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
    if(hist1)
       hist1->Draw();
}

// Adapted from
// https://root-forum.cern.ch/t/read-a-tfile-while-writing-with-another-process/18608/7
void ReadHistogram() {
    TFile *file = TFile::Open("test.root");
    TH1D *hist = nullptr;
    file->GetObject("hist", hist);
    if(hist)
        hist->Draw();
}