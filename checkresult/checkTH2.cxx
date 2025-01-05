#include <TFile.h>
#include <TH2D.h>
#include <TH2I.h>
#include <TTree.h>
#include <iostream>

int checkTH2() {
    // 1つ目のROOTファイルを開き、TH2Dヒストグラムを取得
    TFile* file1 = TFile::Open(Form("sample/localresult_new.root"));
    if (!file1 || file1->IsZombie()) {
        std::cerr << "Error opening TH2D file" << std::endl;
        return 1;
    }
    TH2D* histD = dynamic_cast<TH2D*>(file1->Get("localresult_event0"));
    if (!histD) {
        std::cerr << "Error retrieving TH2D from file1.root" << std::endl;
        file1->Close();
        return 1;
    }

    // 2つ目のROOTファイルを開き、TTreeからTH2Iを取得
    TFile* file2 = TFile::Open(Form("sample/forShibata.root"));
    if (!file2 || file2->IsZombie()) {
        std::cerr << "Error opening file2.root" << std::endl;
        file1->Close();
        return 1;
    }
    TTree* tree = dynamic_cast<TTree*>(file2->Get("FPGATrackSimHoughRootOutput"));
    if (!tree) {
        std::cerr << "Error retrieving TTree from file2.root" << std::endl;
        file1->Close();
        file2->Close();
        return 1;
    }
    // TTreeからTH2Iを取り出す（例: "histI"がTH2Iのブランチと仮定）
    TH2I* histI = nullptr;
    tree->SetBranchAddress("image", &histI);
    tree->GetEntry(0); // 最初のエントリを取得（例）

    if (!histI) {
        std::cerr << "Error retrieving TH2I from TTree in file2.root" << std::endl;
        file1->Close();
        file2->Close();
        return 1;
    }

    // TH2IをTH2Dに変換
// TH2Iから新しいTH2Dオブジェクトを作成
TH2D* histI_asD = new TH2D("histI_asD", histI->GetTitle(),
                           histI->GetNbinsX(), histI->GetXaxis()->GetBinLowEdge(1), histI->GetXaxis()->GetBinUpEdge(histI->GetNbinsX()),
                           histI->GetNbinsY(), histI->GetYaxis()->GetBinLowEdge(1), histI->GetYaxis()->GetBinUpEdge(histI->GetNbinsY()));

// ビンの内容をTH2IからTH2Dにコピー
for (int x = 1; x <= histI->GetNbinsX(); ++x) {
    for (int y = 1; y <= histI->GetNbinsY(); ++y) {
        histI_asD->SetBinContent(x+1, y+1, histI->GetBinContent(x, y));
    }
}

    if (histI_asD) {
    histI_asD->SetDirectory(0);
} else {
    std::cerr << "Error: histI_asD is nullptr" << std::endl;
} 

    // TH2DとTH2Dを引き算
    histD->Add(histI_asD, -1);  // histD = histD - histI_asD

    // 結果を保存するために新しいファイルを作成
    TFile* outputFile = TFile::Open("sample/output.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error creating output.root" << std::endl;
        file1->Close();
        file2->Close();
        return 1;
    }
    histD->Write("result");

    // クリーンアップ
    outputFile->Close();
    file1->Close();
    file2->Close();

    delete histI_asD;  // クローンのメモリ解放

    std::cout << "Subtraction complete and saved to output.root" << std::endl;
    return 0;
}
