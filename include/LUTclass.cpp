//LUTclass.cpp

#ifndef LUTclasscpp
#define LUTclasscpp

#include "LUTclass.h"
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>

void LUTClass::WritingLUT(double input_phi, int noflayer, double input_r) {
    // Set the resolution

    std::cout << "input phi is " << input_phi 
              << ", input r is " << input_r 
              << ", layer number is " << noflayer<<std::endl; 
              

    if (input_phi > GetMaxInputPhi() || input_phi < GetMinInputPhi()) {
        std::cerr << "out of range of input phi" << std::endl;
        return;
    }

    //std::vector<TH2D*> v_hin = GetInputLPlane();
    int phi_bin = v_hin.at(noflayer)->GetXaxis()->FindBin(input_phi);
    int r_bin = v_hin.at(noflayer)->GetYaxis()->FindBin(input_r);

    if(!luttree){cerr<<"luttree is null do SetLUT in writingLUT"<<endl;return;}

    for (int i = 0; i < GetNLUT(); i++) {
        luttree->GetEntry(i);
        if (layer == noflayer && phi_bin > in_min && phi_bin <= in_max && r_bit == r_bin) {
            AddCountHoughPlane(xi, yi, noflayer);
        }
    }

    //std::vector<TH2D*> v_hout = GetEachHoughPlane();
    //TCanvas *c1 = new TCanvas("c1", "c1");    
    //v_hout.at(noflayer)->Draw("colz");
    //c1->SaveAs(Form("../png/res_%d_layer_%d_phi_%f_r_%f.png", resolution, noflayer, input_phi, input_r));
	
    
}
#endif
