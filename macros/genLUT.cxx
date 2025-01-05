//genLUT.cxx
#include"/Users/griffon/efttrackingWS/genLUTs/include/LUTclass.h"
struct pos
{
  int x;
  int y;
  int layer;
};
struct LUT
{
  int input_begin;
  int input_end;
  int layer;
  std::vector<pos> output;
};
LUTClass lut;

void genLUT(){
  //resource
  int max_LUT=300e3;
  int max_FF=3.4e6;

return; 

  TH2D *hout=lut.GetHoughPlane();
  TH2D *hin=lut.GetInputPlane();
 
  vector<TH2D*>v_hin=lut.GetInputLPlane();
  std::vector<LUT> v_LUT;
  std::vector<int> v_count;
  vector<double>r_min=lut.GetminR();
  vector<double>r_max=lut.GetmaxR();
 
  int lcount[6]={0,0,0,0,0,0};
  int in_min;
  int in_max;
  int xi;
  int yi;
  int ri;
  vector<double>phi_l_max=lut.GetMaxInputLPhi();
  vector<double>phi_l_min=lut.GetMinInputLPhi();
  int *v_dphi=lut.GetdPhi();//use this as dphi[6] watch hedder file!!
 const int nlayer=lut.GetNLayer();	 
  std::cout<<std::endl;
	
  TFile* fout= TFile::Open(Form("../requirement/requirement-%d-%d_re.root",lut.GetNbitsqA_pT(),lut.GetNbitsOutputPhi()),"recreate");
  TTree* requirement=new TTree("requirement","requirement");
  requirement->Branch("input_begin",&in_min);
  requirement->Branch("input_end",&in_max);
  requirement->Branch("output_x",&xi);
  requirement->Branch("output_y",&yi);
  requirement->Branch("output_l",&ri);
  int N=hout->GetNbinsX()*hout->GetNbinsY()*(nlayer);


for(xi=1;xi<=hout->GetNbinsX();xi++){
    double xmin=hout->GetXaxis()->GetBinLowEdge(xi);
    double xmean=hout->GetXaxis()->GetBinCenter(xi);
    double xmax=hout->GetXaxis()->GetBinUpEdge(xi);
    double dx=xmax-xmean;//symmetric bin assume
    for(yi=1;yi<=hout->GetNbinsY();yi++){
      double ymin=hout->GetYaxis()->GetBinLowEdge(yi);
      double ymean=hout->GetYaxis()->GetBinCenter(yi);
      double ymax=hout->GetYaxis()->GetBinUpEdge(yi);
      double dy=ymax-ymean;//symmetric bin assume
      for(ri=0;ri<nlayer;ri++){
	//double phi_min=xmin-max(ymax*(r.at(ri)+dr.at(ri)),ymax*(r.at(ri)-dr.at(ri)));
	//double phi_max=xmax-min(ymin*(r.at(ri)-dr.at(ri)),ymin*(r.at(ri)+dr.at(ri)));
	double value1=xmin-ymax*r_max.at(ri);
	double value2=xmin-ymax*r_min.at(ri);
	double value3=xmax-ymin*r_max.at(ri);
	double value4=xmax-ymin*r_min.at(ri);
	double phi_value[4]={value1,value2,value3,value4};
	//double phi_min=xmean-ymean*r_max.at(ri)-sqrt(pow(dx,2)+pow(dy*r_max.at(ri),2)+pow(ymean*dr.at(ri),2));
	//double phi_max=xmean-ymean*r_min.at(ri)+sqrt(pow(dx,2)+pow(dy*r_min.at(ri),2)+pow(ymean*dr.at(ri),2));
	double phi_min=*min_element(begin(phi_value),end(phi_value));
	double phi_max=*max_element(begin(phi_value),end(phi_value));
	//in_min=hin->FindBin(phi_min);
	//in_max=hin->FindBin(phi_max);
	in_min=v_hin.at(ri)->GetXaxis()->FindBin(phi_min);
	in_max=v_hin.at(ri)->GetXaxis()->FindBin(phi_max);
	//printf("[%.6e %.6e]-[%.6e %.6e]*[%.6e %.6e] \n",xmin,xmax,ymin,ymax,r.at(ri)-dr.at(ri),r.at(ri)+dr.at(ri));
	//printf("%f %f %d %d \n",phi_min,phi_max,in_min,in_max);
	//getchar();
	requirement->Fill();
	if(in_min>lut.GetNbitsInputPhi()||in_max<0){
	  printf("\033[1A\033[1Kout of range [%d,%d]\n\n",xi,yi);
		
	}else{
	
	  bool toBeFilled=true;
	  if(v_LUT.size()!=0){
	    for(auto LUT_i:v_LUT){
	      if(LUT_i.input_begin==in_min&&LUT_i.input_end==in_max&&LUT_i.layer==ri){//OR
		LUT_i.output.push_back({xi,yi,ri});
		toBeFilled=false;
		break;
	      }
	    }
	  }
	  if(toBeFilled){
	    LUT LUT_i;
	    LUT_i.input_begin=in_min;
	    LUT_i.input_end=in_max;
	    LUT_i.layer=ri;
	    LUT_i.output.push_back({xi,yi,ri});
	    v_LUT.push_back(LUT_i);
	    lcount[ri]++;
	  }
	
	printf("\033[1A\033[1KLUT %5zu (%5.2f%% of original #LUTs) input pattern %5lld/%5d=%5.2f\n"
		,v_LUT.size(),(double)v_LUT.size()/max_LUT*100.,requirement->GetEntries(),N,(double)requirement->GetEntries()/N*100.);
      	}

	}}}
    
  
	//v_hist_phiminmax.at(0)->Draw("colz");


  requirement->Write();
  /*for(ri=0;ri<z.size();ri++){
    v_hin.at(ri)->Write();
  }*/


  fout->Close();
  std::cout<<std::endl;
  for(ri=0;ri<nlayer;ri++){
    printf("physics layer %d : %d LUTs",ri,lcount[ri]);
    /*for(auto pos :v_LUT.at(ri).output){
      printf("[%d %d] ",xi,yi);
    }*/
    printf("\n");
  }
	
}
