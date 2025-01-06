TH2D *hist = nullptr;
TCanvas *c1 = new TCanvas("c1", "TH2D Display", 800, 600);
void displayTH2D() {
    TFile *file = TFile::Open("houghbin.root");
    if (!file || file->IsZombie()) {
        return;
    }

    TTree *tree = (TTree *)file->Get("tree");
    if (!tree) {
        file->Close();
        return;
    }

    tree->SetBranchAddress("hist", &hist);
    tree->GetEntry(33);

    if (!hist) {
        file->Close();
        return;
    }

    hist->Draw("COLZ");
    TBox *box_1 = new TBox(0.3,468.91422,0.5,576.25598);
    box_1->SetFillColorAlpha(kRed, 0.30); // 半透明の赤
    box_1->Draw("same");
    
    TBox *box_2 = new TBox(0.3,533.97887,0.5,656.21494);
    box_2->SetFillColorAlpha(kBlue, 0.30); // 半透明の赤
    box_2->Draw("same");
   
   TBox *box_3 = new TBox(0.3,600.16533,0.5,737.55251);
    box_3->SetFillColorAlpha(kGreen, 0.30); // 半透明の赤
    box_3->Draw("same");
   
    TBox *box_4 = new TBox(0.3,670.83901,0.5,824.40449);
    box_4->SetFillColorAlpha(kYellow, 0.30); // 半透明の赤
    box_4->Draw("same");
    c1->Update();
   // file->Close();
}
