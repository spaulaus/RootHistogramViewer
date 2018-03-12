#include <TFile.h>
#include <TH1F.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void AsyncFlush(TFile *file, std::mutex *lock) {
    std::cout << "AsyncFlush - Now calling file->Write" << std::endl;
    file->Write(0, TObject::kWriteDelete);
    std::cout << "AsyncFlush - Now unlocking the file for writing again" << std::endl;
    lock->unlock();
}

void WriteToDisk(TFile *file, const unsigned int &loopID, std::mutex *lock) {
    if (lock->try_lock()) {
        std::cout << "WriteToDisk - we're creating the new thread in loop number" << loopID << endl;
        std::thread worker0(AsyncFlush, file, lock);
        worker0.detach();
    }
}

void writer() {
    static std::mutex lock;
    TFile *f = new TFile("test.root", "RECREATE");
    TH1F *hist = new TH1F("hist", "", 100, -2, 2);
    unsigned int loopCounter = 0;
    while (loopCounter < 10000) {
        cout << "Working on loop number" << loopCounter << endl;
        hist->FillRandom("gaus", 10000);
        WriteToDisk(f, loopCounter++, &lock);
    }
    while(!lock.try_lock())
        sleep(1);
    f->Write(nullptr, TObject::kWriteDelete);
    f->Close();
}

void reader(TFile *file, const char *name) {
    TH1I *hist1 = nullptr;
    delete file->FindObject(name);
    file->ReadKeys();
    file->GetObject(name, hist1);
    hist1->Draw();
}