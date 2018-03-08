#include <TFile.h>
#include <TH1.h>

void viewOnline(TFile *file, const int &id) {
    file->ReadKeys();
    delete file->FindObject(("h"+std::to_string(id)).c_str());
    TH1I *h1;
    file->GetObject(("h"+std::to_string(id)).c_str(), h1);
    h1->Draw();
}

//void dd(unsigned int &id) {
//    _file0->ReadKeys();
//    delete f->FindObject(name);
//    TH2I *h2;
//    f->GetObject(name, h);
//    h->Draw();
//}
