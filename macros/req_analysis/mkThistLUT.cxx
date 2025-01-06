//genLUT.cxx
#include"../../include/LUTclass.h"
#include "../../include/LUTclass.cpp"

struct pos
{
  int x;
  int y;
  int input_rbit;
  int layer;
};
struct LUT
{
  int input_begin;
  int input_end;
  int input_rbit;
  int layer;
  std::vector<pos> output;
};
LUTClass lut;

void mkThistLUT(){
  //resource
  int max_LUT=300e3;
  int max_FF=3.4e6;
  
  const int nlayer=lut.GetNLayer();	
  TFile *savefile=new TFile("houghbin.root","recreate");
  TTree *tree=new TTree("tree","tree");
  TH2D *hout=lut.GetHoughPlane();
  TH2D *hin=lut.GetInputPlane();
  TH2D *test=nullptr;
  tree->Branch("hist","TH2D",&test); 
  vector<TH2D*>v_hin=lut.GetInputLPlane();
  std::vector<int> v_count;
  vector<double>r_min=lut.GetminR();
  vector<double>r_max=lut.GetmaxR();
 
  int lcount[6]={0,0,0,0,0,0};
  int in_min;
  int in_max;
  int xi;
  int yi;
  int ri;
  int r_bit_start;
  int r_bit_end;
  int r_bit;
  tree->Branch("xbin",&xi);
  tree->Branch("ybin",&yi);
  vector<double>phi_l_max=lut.GetMaxInputLPhi();
  vector<double>phi_l_min=lut.GetMinInputLPhi();
  int *v_dphi=lut.GetdPhi();//use this as dphi[6] watch hedder file!!
	
  int sum_bin_r=0;
  for(int ri=0;ri<nlayer;ri++){sum_bin_r+=v_hin.at(ri)->GetNbinsY();}
  int N=(hout->GetNbinsX()*hout->GetNbinsY())*sum_bin_r;

for(xi=1;xi<=hout->GetNbinsX();xi++){
    double xmin=hout->GetXaxis()->GetBinLowEdge(xi);
   // double xmean=hout->GetXaxis()->GetBinCenter(xi);
    double xmax=hout->GetXaxis()->GetBinUpEdge(xi);
    //double dx=xmax-xmean;//symmetric bin assume
    for(yi=1;yi<=hout->GetNbinsY();yi++){
      double ymin=hout->GetYaxis()->GetBinLowEdge(yi);
     // double ymean=hout->GetYaxis()->GetBinCenter(yi);
      double ymax=hout->GetYaxis()->GetBinUpEdge(yi);
      //double dy=ymax-ymean;//symmetric bin assume
	test=new TH2D(Form("x_%dy_%d",xi,yi),"test;input_phi;input_r (mm)",1000,0.3,0.5,500,400,900);
	for(int i=1;i<=test->GetNbinsY();i++){
		double r_max_local=v_hin.at(ri)->GetYaxis()->GetBinUpEdge(i);
		double r_min_local=v_hin.at(ri)->GetYaxis()->GetBinLowEdge(i);
		double value1=xmin-ymax*r_max_local;
		double value2=xmin-ymax*r_min_local;
		double value3=xmax-ymin*r_max_local;
		double value4=xmax-ymin*r_min_local;
		double phi_value[4]={value1,value2,value3,value4};
	//double phi_min=xmean-ymean*r_max.at(ri)-sqrt(pow(dx,2)+pow(dy*r_max.at(ri),2)+pow(ymean*dr.at(ri),2));
	//double phi_max=xmean-ymean*r_min.at(ri)+sqrt(pow(dx,2)+pow(dy*r_min.at(ri),2)+pow(ymean*dr.at(ri),2));
		double phi_min=*min_element(begin(phi_value),end(phi_value));
		double phi_max=*max_element(begin(phi_value),end(phi_value));
		int min_bin=test->FindBin(phi_min);
		int max_bin=test->FindBin(phi_max);
		for(int j=min_bin;j<=max_bin;j++)test->SetBinContent(j,i,1);
		
			
	
	
	}	
	tree->Fill();
	delete test;
	test=nullptr;
	//double phi_min=xmin-max(ymax*(r.at(ri)+dr.at(ri)),ymax*(r.at(ri)-dr.at(ri)));
	//double phi_max=xmax-min(ymin*(r.at(ri)-dr.at(ri)),ymin*(r.at(ri)+dr.at(ri)));
    }//yi loop
  }//xi loop
	//v_hist_phiminmax.at(0)->Draw("colz");

	tree->Write();

	savefile->Close();
	
}
