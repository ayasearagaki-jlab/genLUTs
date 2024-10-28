//LUTclass.h

#ifndef LUTclass
#define LUTclass


class LUTClass {

public:
	LUTClass(){
		v_phi_L=vector<vector<double>>(2);

		hin=new TH2D("in","in",in_phi_bin,in_phi_min,in_phi_max,in_phi_bin,in_r_min,in_r_max);
		hout=new TH2D("out","out;phi_t;qA/p_{T}",phi_bin,phi_min,phi_max,qA_pT_bin,qA_pT_min,qA_pT_max);
//high eta mode calculation		
		for(int i=0;i<z.size();i++){
			double ri_max=2.0*z.at(i)*atan(exp(-eta_max));
			double ri_min=2.0*z.at(i)*atan(exp(-eta_min));

			r_max.push_back(ri_max);
			r_min.push_back(ri_min);
		}
		 for(int ri=0;ri<z.size();ri++){
    		double phi_L_min=phi_min-qA_pT_max*(r_max.at(ri));//FIXME OK only if phi_min,phi_max>0
    		double phi_L_max=phi_max-qA_pT_min*(r_min.at(ri));
    		int phi_L_min_bin=hin->GetXaxis()->FindBin(phi_L_min);
    		int phi_L_max_bin=hin->GetXaxis()->FindBin(phi_L_max);
    		int r_min_bin=hin->GetYaxis()->FindBin(r_min.at(ri));
			int r_max_bin=hin->GetYaxis()->FindBin(r_max.at(ri));


			double dPhi=hin->GetXaxis()->GetBinUpEdge(phi_L_max_bin)-hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin);
			double dri=hin->GetYaxis()->GetBinUpEdge(r_max_bin)-hin->GetYaxis()->GetBinLowEdge(r_min_bin);

    		int dPhi_bits=phi_L_max_bin-phi_L_min_bin+1;
			int dri_bits=r_max_bin-r_min_bin+1;
			double dPhi_bits_length=TMath::Log2(dPhi_bits);
			double dri_bits_length=TMath::Log2(dri_bits);

    		printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dPhi=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",phi_L_min,phi_L_max,phi_L_min_bin,phi_L_max_bin,dPhi,dPhi_bits,dPhi_bits,dPhi_bits_length);
    		printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dr=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",r_min.at(ri),r_max.at(ri),r_min_bin,r_max_bin,dri,dri_bits,dri_bits,dri_bits_length);

			TH2D* hin_l=new TH2D(Form("in%d",ri),Form("in%d",ri),(int)pow(2,ceil(dPhi_bits_length)),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length)))
					,(int)pow(2,ceil(dri_bits_length)),hin->GetYaxis()->GetBinLowEdge(r_min_bin),hin->GetYaxis()->GetBinLowEdge(r_min_bin+(int)pow(2,ceil(dri_bits_length))));//origin
			v_hin.push_back(hin_l);//r_iが変数なので使わない?
			v_phi_L.at(0).push_back(phi_L_min);
			v_phi_L.at(1).push_back(phi_L_max);
			v_dphi[ri]=static_cast<int>(std::round(dPhi_bits));
			TH2D* hout_l=(TH2D*)hout->Clone(Form("hout_%d",ri));
			v_hout.push_back(hout_l);
			v_hout.at(ri)->SetTitle(Form("Hough Plane in layer%d;phi_t;qA/p_{T}",ri));



		}
		

	}
	
	~LUTClass(){
		delete hout;
		for(int i=0;i<v_hin.size();i++){delete v_hin.at(i);}
		for(int i=0;i<v_hout.size();i++){delete v_hout.at(i);}

		delete hin;
	}

	int GetNLayer()const{return z.size();}

	vector<double> GetMaxInputLPhi(){return v_phi_L.at(1);}
	vector<double> GetMinInputLPhi(){return v_phi_L.at(0);}
	int* GetdPhi(){return v_dphi;}
	
	
	TH2D* GetHoughPlane(){return hout;}
	
	vector<TH2D*> GetInputLPlane(){return v_hin;}//L means for each layer the maximum and minimum of region of input about geometry using eta.
	TH2D* GetInputPlane(){return hin;}//Only Input is  requirement from setting of eft project or ROD.
	vector<double> GetRMax(){return r_max;}
	vector<double> GetRMin(){return r_min;}

	double GetMinInputPhi(){return in_phi_min;}
	double GetMaxInputPhi(){return in_phi_max;}

	vector<double> GetmaxR(){return r_max;}
	vector<double> GetminR(){return r_min;}

	int GetNbitsInputPhi(){return in_phi_bin;}
	int GetNbitsOutputPhi(){return phi_bin;}
	int GetNbitsqA_pT(){return qA_pT_bin;}

	void AddCountHoughPlane(int xbin,int ybin, int layer){
		if(xbin<1||ybin<1||xbin>hout->GetNbinsX()||ybin>hout->GetNbinsY()||layer<0||layer>=6){
			cerr<<":AddCountHoughPlane out range of bin number"<<endl;
			return ;
		}
		
		int content=v_hout.at(layer)->GetBinContent(xbin,ybin);
		v_hout.at(layer)->SetBinContent(xbin,ybin,content+1);
	


	}

	vector<TH2D*> GetEachHoughPlane(){return v_hout;}

	void RebinRInputAllPlane(int rebin_r_num){
		if(rebin_r_num==1){return;}
		cout<<endl<<"RebinRInputAllPlane make all Input Plane reset, so use this function before fill"<<endl;
		
		v_hin.clear();
		delete hin;
		int in_r_bin=ceil(in_phi_bin/rebin_r_num);//hinのrのbin数を設定
		
		hin=new TH2D("in","in",in_phi_bin,in_phi_min,in_phi_max,in_r_bin,in_r_min,in_r_max);//hinを再定義

		for(int ri=0;ri<z.size();ri++){
    		double phi_L_min=phi_min-qA_pT_max*(r_max.at(ri));//FIXME OK only if phi_min,phi_max>0
    		double phi_L_max=phi_max-qA_pT_min*(r_min.at(ri));
    		int phi_L_min_bin=hin->GetXaxis()->FindBin(phi_L_min);
    		int phi_L_max_bin=hin->GetXaxis()->FindBin(phi_L_max);
    		int r_min_bin=hin->GetYaxis()->FindBin(r_min.at(ri));
			int r_max_bin=hin->GetYaxis()->FindBin(r_max.at(ri));


			double dPhi=hin->GetXaxis()->GetBinUpEdge(phi_L_max_bin)-hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin);
			double dri=hin->GetYaxis()->GetBinUpEdge(r_max_bin)-hin->GetYaxis()->GetBinLowEdge(r_min_bin);

    		int dPhi_bits=phi_L_max_bin-phi_L_min_bin+1;
			int dri_bits=r_max_bin-r_min_bin+1;
			double dPhi_bits_length=TMath::Log2(dPhi_bits);
			double dri_bits_length=TMath::Log2(dri_bits);

    		printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dPhi=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",phi_L_min,phi_L_max,phi_L_min_bin,phi_L_max_bin,dPhi,dPhi_bits,dPhi_bits,dPhi_bits_length);
    		printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dr=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",r_min.at(ri),r_max.at(ri),r_min_bin,r_max_bin,dri,dri_bits,dri_bits,dri_bits_length);

			TH2D* hin_l=new TH2D(Form("in%d_rebin",ri),Form("in%d_rebin",ri),(int)pow(2,ceil(dPhi_bits_length)),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length)))
					,(int)pow(2,ceil(dri_bits_length)),hin->GetYaxis()->GetBinLowEdge(r_min_bin),hin->GetYaxis()->GetBinLowEdge(r_min_bin+(int)pow(2,ceil(dri_bits_length))));//origin
			v_hin.push_back(hin_l);//r_iが変数なので使わない?
			v_dphi[ri]=static_cast<int>(std::round(dPhi_bits));



		}//for loop
	}








private:
	
//Input
	std::vector<double> dr={0,0,0,0,0,0};	
	std::vector<double> z={1500,1700,2000,2250,2600,3000};
  	std::vector<double> r_max;
  	std::vector<double> r_min;
	double eta_max=2.0;
  	double eta_min=2.2;
	
	double in_phi_min=-4.0;/*-1.*TMath::Pi();*/
  	double in_phi_max=4.0;/*TMath::Pi();*/
  	double in_r_min=0.0;//ITkのいつもの図から取ってきた
  	double in_r_max=1024;  
	int in_phi_bin=pow(2,16);
	TH2D* hin;
	std::vector<TH2D*> v_hin;
	vector<vector<double>> v_phi_L;
	int v_dphi[6];


	
//Output
	double phi_min=0.3;
  	double phi_max=0.5;
  	int phi_bin=64;//96?
  	double qA_pT_min=-3e-4;
  	double qA_pT_max=3e-4;
  	int qA_pT_bin=216;
	TH2D* hout;
	vector<TH2D*>v_hout;
};
#endif
