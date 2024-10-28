//writingLUT.cc
#include"/Users/griffon/efttrackingWS/genLUTs/include/LUTclass.h"
LUTClass lut;

void writingLUT(){
/*
	TH2D*hout=lut.GetHoughPlane();
	hout->Draw("colz");
	return;	*/
	//input
	double input_phi=0.5;//FIXME
	int noflayer=0;//FIXME

	cout<<"input phi is "<<input_phi<<" layer number is "<<noflayer<<endl;
	if(input_phi>lut.GetMaxInputPhi()||input_phi<lut.GetMinInputPhi()){
		cerr<<"out range of input phi"<<endl;
		return;
	}
	
	vector<TH2D*>v_hin=lut.GetInputLPlane();

	
	int phi_bin=v_hin.at(noflayer)->GetXaxis()->FindBin(input_phi);
	//cout<<phi_bin<<endl;
	
	TFile *LUT=new TFile(Form("../requirement/requirement-%d-%d.root",lut.GetNbitsqA_pT(),lut.GetNbitsOutputPhi()),"read");
	TTree * luttree=(TTree*)LUT->Get("requirement");
	int NLUT=luttree->GetEntries();
	
	int in_min;
	int in_max;
	int xi;
	int yi;
	int layer;
	
	luttree->SetBranchAddress("input_begin",&in_min);
	luttree->SetBranchAddress("input_end",&in_max);
	luttree->SetBranchAddress("output_x",&xi);
	luttree->SetBranchAddress("output_y",&yi);
	luttree->SetBranchAddress("output_l",&layer);

	for(int i=0;i<NLUT;i++){
		luttree->GetEntry(i);
		//if(layer==0){cout<<in_min<<" "<<in_max<<endl;}	
		if(layer==noflayer&&phi_bin>in_min&&phi_bin<=in_max){lut.AddCountHoughPlane(xi,yi,noflayer);}



	}//for loop in tree 

	vector<TH2D*>v_hout=lut.GetEachHoughPlane();
	
	v_hout.at(noflayer)->Draw("colz");

	LUT->Close();
	










}
