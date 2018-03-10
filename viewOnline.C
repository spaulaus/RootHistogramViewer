#include <TFile.h>
#include <TH1.h>

#include <iostream>

void viewOnline(TFile *file, const int &id) {
    const char *name = ("h"+std::to_string(id)).c_str();
    file->ReadKeys();
    delete file->FindObject(name);
    TH1I *hist1;
    file->GetObject(name, hist1);
    hist1->Draw();
}

//void dd(unsigned int &id) {
//    _file0->ReadKeys();
//    delete f->FindObject(name);
//    TH2I *h2;
//    f->GetObject(name, h);
//    h->Draw();
//}
