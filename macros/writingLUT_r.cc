//writingLUT.cc
#include"/Users/griffon/efttrackingWS/genLUTs/include/LUTclass.h"
LUTClass lut;

void writingLUT_r(){
	
	//input
	double input_phi=0.35;//FIXME
	int noflayer=2;//FIXME
	double input_r=500.0;//FIXME
	int resolution=2048*2;
	lut.RebinRInputAllPlane(resolution); 
	cout<<"input phi is "<<input_phi<<" inpur r is "<<input_r<<" layer number is "<<noflayer<<" using resolution="<<resolution<<" LUT"<<endl;
	if(input_phi>lut.GetMaxInputPhi()||input_phi<lut.GetMinInputPhi()){
		cerr<<"out range of input phi"<<endl;
		return;
	}
	
	vector<TH2D*>v_hin=lut.GetInputLPlane();

	
	int phi_bin=v_hin.at(noflayer)->GetXaxis()->FindBin(input_phi);
	int r_bin=v_hin.at(noflayer)->GetYaxis()->FindBin(input_r);

	//cout<<phi_bin<<endl;
	
	TFile *LUT=new TFile(Form("../requirement/requirement-%d-%d_ver_r_%d.root",lut.GetNbitsqA_pT(),lut.GetNbitsOutputPhi(),resolution),"read");
	TTree * luttree=(TTree*)LUT->Get("requirement");
	int NLUT=luttree->GetEntries();
	
	int in_min;
	int in_max;
	int xi;
	int yi;
	int layer;
	int r_bit;
	int r_bit_start;
	int r_bit_end;	
	luttree->SetBranchAddress("input_begin",&in_min);
	luttree->SetBranchAddress("input_end",&in_max);
	luttree->SetBranchAddress("input_r_bit",&r_bit);
	luttree->SetBranchAddress("output_x",&xi);
	luttree->SetBranchAddress("output_y",&yi);
	luttree->SetBranchAddress("output_l",&layer);

	for(int i=0;i<NLUT;i++){
		luttree->GetEntry(i);
		//if(layer==0){cout<<in_min<<" "<<in_max<<endl;}	
		if(layer==noflayer&&phi_bin>in_min&&phi_bin<=in_max&&r_bit==r_bin){lut.AddCountHoughPlane(xi,yi,noflayer);}



	}//for loop in tree 

	vector<TH2D*>v_hout=lut.GetEachHoughPlane();
	TCanvas *c1=new TCanvas("c1","c1");	
	v_hout.at(noflayer)->Draw("colz");

	c1->SaveAs(Form("../png/res_%d_layer_%d_phi_%f_r_%f.png",resolution,noflayer,input_phi,input_r));
	
	LUT->Close();
	










}
