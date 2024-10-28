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
void original_genLUT(){
  //std::vector<double> r={33.25,50.5,88.5,122.5,299,371,443,514};
  std::vector<double> r={291,400,562,762,1000};
  //std::vector<double> dr={6,15,15,15,15};//full cover
  //std::vector<double> dr={2.7,6.9,6.5,6.5,6.5};//std div
  std::vector<double> dr={0,0,0,0,0};//ignore
  //output
  double phi_min=0.3;
  double phi_max=0.5;
  int phi_bin=96;//64??
  double qA_pT_min=-3e-4;
  double qA_pT_max=3e-4;
  int qA_pT_bin=216;
  TH2D* hout=new TH2D("out","out;phi_t;qA/p_{T}",phi_bin,phi_min,phi_max,qA_pT_bin,qA_pT_min,qA_pT_max);
  int nLayer=r.size();
  //resource
  int max_LUT=300e3;
  int max_FF=3.4e6;

  //input
  double in_phi_min=-1.*TMath::Pi();
  double in_phi_max=TMath::Pi();
  int in_phi_bin=pow(2,16);
  TH1D* hin=new TH1D("in","in",in_phi_bin,in_phi_min,in_phi_max);
  std::vector<TH1D*> v_hin;

  std::vector<LUT> v_LUT;
  std::vector<int> v_count;
  int lcount[5]={0,0,0,0,0};
  int in_min;
  int in_max;
  int xi;
  int yi;
  int ri;
  for(ri=0;ri<r.size();ri++){
    double phi_L_min=phi_min-qA_pT_max*(r.at(ri)+dr.at(ri));//FIXME OK only if phi_min,phi_max>0
    double phi_L_max=phi_max-qA_pT_min*(r.at(ri)-dr.at(ri));
    int phi_L_min_bin=hin->FindBin(phi_L_min);
    int phi_L_max_bin=hin->FindBin(phi_L_max);
    double dPhi=hin->GetXaxis()->GetBinUpEdge(phi_L_max_bin)-hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin);
    int dPhi_bits=phi_L_max_bin-phi_L_min_bin+1;
    double dPhi_bits_length=TMath::Log2(dPhi_bits);
    printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dPhi=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",phi_L_min,phi_L_max,phi_L_min_bin,phi_L_max_bin,dPhi,dPhi_bits,dPhi_bits,dPhi_bits_length);
    TH1D* hin_l=new TH1D(Form("in%d",ri),Form("in%d",ri),(int)pow(2,ceil(dPhi_bits_length)),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length))));//origin
    //printf("%d %e\n",hin_l->GetNbinsX(),hin_l->GetBinWidth(1)-hin->GetBinWidth(1));
    //TH1D* hin_l=new TH1D(Form("in%d",ri),Form("in%d",ri),(int)pow(2,10),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length))));//10 bits-keep same bin edge but merge some bins
    v_hin.push_back(hin_l);

  }
  std::cout<<std::endl;
  //return;
  TFile* fout= TFile::Open(Form("requirement-%d-%d.root",qA_pT_bin,phi_bin),"recreate");
  TTree* requirement=new TTree("requirement","requirement");
  requirement->Branch("input_begin",&in_min);
  requirement->Branch("input_end",&in_max);
  requirement->Branch("output_x",&xi);
  requirement->Branch("output_y",&yi);
  requirement->Branch("output_l",&ri);
  int N=hout->GetNbinsX()*hout->GetNbinsY()*(r.size());

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
      for(ri=0;ri<r.size();ri++){
	//double phi_min=xmin-max(ymax*(r.at(ri)+dr.at(ri)),ymax*(r.at(ri)-dr.at(ri)));
	//double phi_max=xmax-min(ymin*(r.at(ri)-dr.at(ri)),ymin*(r.at(ri)+dr.at(ri)));
	double phi_min=xmean-ymean*r.at(ri)-sqrt(pow(dx,2)+pow(dy*r.at(ri),2)+pow(ymean*dr.at(ri),2));
	double phi_max=xmean-ymean*r.at(ri)+sqrt(pow(dx,2)+pow(dy*r.at(ri),2)+pow(ymean*dr.at(ri),2));
	//in_min=hin->FindBin(phi_min);
	//in_max=hin->FindBin(phi_max);
	in_min=v_hin.at(ri)->FindBin(phi_min);
	in_max=v_hin.at(ri)->FindBin(phi_max);
	//printf("[%.6e %.6e]-[%.6e %.6e]*[%.6e %.6e] \n",xmin,xmax,ymin,ymax,r.at(ri)-dr.at(ri),r.at(ri)+dr.at(ri));
	//printf("%f %f %d %d \n",phi_min,phi_max,in_min,in_max);
	//getchar();
	requirement->Fill();
	if(in_min>in_phi_bin||in_max<=0){
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
	}
	printf("\033[1A\033[1KLUT %5zu (%5.2f%% of original #LUTs) input pattern %5lld/%5d=%5.2f\n",v_LUT.size()*2-lcount[0]-lcount[1],(double)(v_LUT.size()*2-lcount[0]-lcount[1])/max_LUT*100.,requirement->GetEntries(),N,v_LUT.size(),(double)requirement->GetEntries()/N*100.);
      }
    }
  }
  requirement->Write();
  for(ri=0;ri<r.size();ri++){
    v_hin.at(ri)->Write();
  }
  fout->Close();
  std::cout<<std::endl;
  for(ri=0;ri<r.size();ri++){
    printf("physics layer %d : %d LUTs",ri,lcount[ri]);
    /*for(auto pos :v_LUT.at(ri).output){
      printf("[%d %d] ",xi,yi);
    }*/
    printf("\n");
  }
}
