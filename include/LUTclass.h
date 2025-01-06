//LUTclass.h

#ifndef LUTclass
#define LUTclass


class LUTClass {

public:
	LUTClass(bool flag=true):flagendcap(flag){
		v_phi_L=vector<vector<double>>(2);

		hin=new TH2D("in","in",in_phi_bin,in_phi_min,in_phi_max,in_phi_bin,in_r_min,in_r_max);
		hout=new TH2D("out","out;phi_t;qA/p_{T}",phi_bin,phi_min,phi_max,qA_pT_bin,qA_pT_min,qA_pT_max);
	    hin_barrel=new TH1D("in_barrel","in_barrel",in_phi_bin,in_phi_min,in_phi_max);
		hout_merge=new TH2D("out_merge","out_merge;phi_t;qA/p_{T}",phi_bin,phi_min,phi_max,qA_pT_bin,qA_pT_min,qA_pT_max);


//high eta mode calculation		
		if(flag){
		for(int i=0;i<z.size();i++){
			double ri_max=(z.at(i)-Z)*tan(2.0*atan(exp(-eta_max)));
			double ri_min=(z.at(i)-Z)*tan(2.0*atan(exp(-eta_min)));

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



		}//endcap construct
		}else{
			for(int ri=0;ri<barrel_r.size();ri++){
    			double phi_L_min=phi_min-qA_pT_max*(barrel_r.at(ri)+barrel_dr.at(ri));//FIXME OK only if phi_min,phi_max>0
    			double phi_L_max=phi_max-qA_pT_min*(barrel_r.at(ri)-barrel_dr.at(ri));
    			int phi_L_min_bin=hin->FindBin(phi_L_min);
    			int phi_L_max_bin=hin->FindBin(phi_L_max);
    			double dPhi=hin->GetXaxis()->GetBinUpEdge(phi_L_max_bin)-hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin);
    			int dPhi_bits=phi_L_max_bin-phi_L_min_bin+1;
    			double dPhi_bits_length=TMath::Log2(dPhi_bits);
    			printf("[%8.5f,%8.5f] [0x%04x,0x%04x] dPhi=%8.5f (=%d [0x%04x] pattern=%.1f bins)\n",phi_L_min,phi_L_max,phi_L_min_bin,phi_L_max_bin,dPhi,dPhi_bits,dPhi_bits,dPhi_bits_length);
    			TH1D* hin_l=new TH1D(Form("in%d",ri),Form("in%d",ri),(int)pow(2,ceil(dPhi_bits_length)),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length))));//origin
    		//printf("%d %e\n",hin_l->GetNbinsX(),hin_l->GetBinWidth(1)-hin->GetBinWidth(1));
    		//TH1D* hin_l=new TH1D(Form("in%d",ri),Form("in%d",ri),(int)pow(2,10),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin),hin->GetXaxis()->GetBinLowEdge(phi_L_min_bin+(int)pow(2,ceil(dPhi_bits_length))));//10 bits-keep same bin edge but merge some bins
    			v_hin_barrel.push_back(hin_l);
  			}
			for(int ri=0;ri<barrellayer;ri++){
				TH2D* hout_l=(TH2D*)hout->Clone(Form("hout_%d",ri));
				v_hout.push_back(hout_l);
				v_hout.at(ri)->SetTitle(Form("Hough Plane in layer%d;phi_t;qA/p_{T}",ri));
			}


		}//barrel construct
		

	}
	
	~LUTClass(){
		delete hout;
		if(flagendcap){if(!flagsetLUT||(LUT&&LUT->IsOpen())){for(int i=0;i<v_hin.size();i++){delete v_hin.at(i);}}
		}else{if(!flagsetLUT||(LUT&&LUT->IsOpen())){for(int i=0;i<v_hin_barrel.size();i++){delete v_hin_barrel.at(i);}}}
		for(int i=0;i<v_hout.size();i++){delete v_hout.at(i);}
		if(LUT&&LUT->IsOpen()){delete luttree;}
		delete LUT;

		delete hin;
		delete hin_barrel;
		delete hout_merge;
	}

	int GetNLayer()const{
		if(flagendcap){return z.size();}
		else{return barrel_r.size();}
	}

	vector<double> GetMaxInputLPhi(){return v_phi_L.at(1);}
	vector<double> GetMinInputLPhi(){return v_phi_L.at(0);}
	int* GetdPhi(){return v_dphi;}
	
	
	TH2D* GetHoughPlane(){return hout;}
	
	vector<TH2D*> GetInputLPlane(){if(flagendcap){return v_hin;}else{cerr<<"GetInputLPlane is for endcap"<<endl;exit(1);}}//L means for each layer the maximum and minimum of region of input about geometry using eta.
	TH2D* GetInputPlane(){if(flagendcap){return hin;}else{cerr<<"GetInputPlane is for endcap"<<endl;exit(1);}}//Only Input is  requirement from setting of eft project or ROD.
	vector<double> GetRMax(){return r_max;}
	vector<double> GetRMin(){return r_min;}

	double GetMinInputPhi(){return in_phi_min;}
	double GetMaxInputPhi(){return in_phi_max;}

	vector<double> GetmaxR(){return r_max;}
	vector<double> GetminR(){return r_min;}

	int GetNbitsInputPhi(){return in_phi_bin;}
	int GetNbitsOutputPhi(){return phi_bin;}
	int GetNbitsqA_pT(){return qA_pT_bin;}
	int Getnofbit(){return nofbit;}

	void AddCountHoughPlane(int xbin,int ybin, int layer){
		if(xbin<1||ybin<1||xbin>hout->GetNbinsX()||ybin>hout->GetNbinsY()||layer<0){
			cerr<<":AddCountHoughPlane out range of bin number"<<endl;
			return ;
		}
		
		int content=v_hout.at(layer)->GetBinContent(xbin,ybin);
		v_hout.at(layer)->SetBinContent(xbin,ybin,content+1);
	


	}

	vector<TH2D*> GetEachHoughPlane(){return v_hout;}

	void RebinRInputAllPlane(int rebin_r_num){
		if(!flagendcap){cerr<<"RebinRInputAllPlane is valid in barrel operation"<<endl;return;}

		if(rebin_r_num==1){return;}
		cout<<endl<<"RebinRInputAllPlane make all Input Plane reset, so use this function before fill"<<endl;
		v_hin.clear();
		
		
			delete hin;
			hin=nullptr;
		
			

		in_r_bin=ceil(in_phi_bin/rebin_r_num);//hinのrのbin数を設定
		
		hin=new TH2D("in_rebin","in_rebin",in_phi_bin,in_phi_min,in_phi_max,in_r_bin,in_r_min,in_r_max);//hinを再定義
		
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
		std::cout<<"set LUT rebinnum="<<rebin_r_num<<endl; 

	}


	void FlatEachHoughPlane(){
		for(int ri=0;ri<v_hout.size();ri++){
			TH2D* hout_copy=(TH2D*)v_hout.at(ri)->Clone(Form("hout%d_copy",ri));
			v_hout.at(ri)->Divide(hout_copy);
		}
	}
	
	void ResetEachHoughPlane(){
		for(int ri=0;ri<v_hout.size();ri++){v_hout.at(ri)->Reset();}
		hout_merge->Reset();
	}


	TH2D* GetAndMergeHoughPlane(){

		for(int ri=0;ri<v_hout.size();ri++){
			if(v_hout.at(ri)->Integral()==0){cout<<"layer"<<ri<<"Hough Plane is empty!!"<<endl;}
			hout_merge->Add(v_hout.at(ri));
		}
		
		return hout_merge;
	}
	
	void WritingLUT(double input_phi, int noflayer, double input_r,int physlayer);

	void WritingLUT(double input_phi, int noflayer,int physlayer);

	
	void SetLUT(int resolution){
		if(flagendcap){
			cout<<"Set LUT in endcap and R_rebinnum="<<resolution<<endl;

			LUT = new TFile(Form("../requirement/requirement-%d-%d_ver_r_%d.root",GetNbitsqA_pT(), GetNbitsOutputPhi(), resolution), "read");
	
			if (!LUT || LUT->IsZombie()) {
    			std::cerr << "Error: TFile about LUT could not be opened or does not exist. Operation aborted." << std::endl;
    			delete LUT; // 念のためのクリーンアップ
    			LUT=nullptr;
				exit(1);     // 即座にプログラムを終了
			}	
		
			luttree = (TTree*)LUT->Get("requirement");
			
			luttree->SetBranchAddress("input_begin", &in_min);
    		luttree->SetBranchAddress("input_end", &in_max);
    		luttree->SetBranchAddress("input_r_bit", &r_bit);
    		luttree->SetBranchAddress("output_x", &xi);
    		luttree->SetBranchAddress("output_y", &yi);
    		luttree->SetBranchAddress("output_l", &layer);
			for(int i=0;i<v_hin.size();i++){delete v_hin.at(i);}
			v_hin.clear();
			for(int ri=0;ri<z.size();ri++){
				TH2D *endcap_l=(TH2D*)LUT->Get(Form("in%d",ri));
				v_hin.push_back(endcap_l);
			}



			flagsetLUT=true;
			return;
		}else{
			cout<<"In Barrel operation resolution value is valid"<<endl;
			LUT = new TFile(Form("../requirement/requirement-%d-%d.root",GetNbitsqA_pT(), GetNbitsOutputPhi()), "read");
			if (!LUT || LUT->IsZombie()) {
    			std::cerr << "Error: TFile about LUT could not be opened or does not exist. Operation aborted." << std::endl;
    			delete LUT; // 念のためのクリーンアップ
    			LUT=nullptr;
				exit(1);     // 即座にプログラムを終了
			}	
			luttree = (TTree*)LUT->Get("requirement");
			luttree->SetBranchAddress("input_begin", &in_min);
    		luttree->SetBranchAddress("input_end", &in_max);
    		luttree->SetBranchAddress("output_x", &xi);
    		luttree->SetBranchAddress("output_y", &yi);
    		luttree->SetBranchAddress("output_l", &layer);
			for(int i=0;i<v_hin_barrel.size();i++){delete v_hin_barrel.at(i);}
			v_hin_barrel.clear();
			for(int ri=0;ri<barrel_r.size();ri++){
				TH1D *barrel_l=(TH1D*)LUT->Get(Form("in%d",ri));
				v_hin_barrel.push_back(barrel_l);
			}
			
			flagsetLUT=true;
			return;}
		
	
	}
	

	int GetNLUT(){
		if(!luttree){cerr<<"luttree is null do SetLUT!! in GetNLUT"<<endl;return-1;}
		
		return luttree->GetEntries();
	}

	void CloseLUT(){
		if(luttree){delete luttree;luttree=nullptr;}
		
		if(!LUT){cerr<<"LUT Tfile is null in close"<<endl;return;}
		if(LUT&&LUT->IsOpen()){LUT->Close();cout<<"closed Tfile LUT"<<endl;return;}
		else{cerr<<"LUT Tfile is not open? What's up ?"<<endl;return;}

	}



private:
	bool flagendcap=true;
	bool flagsetLUT=false;	
//Input
	std::vector<double> dr={0,0,0,0,0,0};	
	//std::vector<double> z={1500,1700,1940,2230,2525,2840};
	std::vector<double> z={1940,2230,2525,2840};
	std::vector<double> barrel_r={291,400,562,762,1000};
	std::vector<double> barrel_dr={0,0,0,0,0};
	int barrellayer=9;
  	std::vector<double> r_max;
  	std::vector<double> r_min;
	double eta_max=2.0;
  	double eta_min=2.2;
	
	double in_phi_min=-4.0;/*-1.*TMath::Pi();*/
  	double in_phi_max=4.0;/*TMath::Pi();*/
  	double in_r_min=0.0;//ITkのいつもの図から取ってきた
  	double in_r_max=1023.984375;  
	int nofbit=16;
	int in_phi_bin=pow(2,nofbit);
	int in_r_bin=pow(2,nofbit);
	TH2D* hin=nullptr;
	TH1D* hin_barrel=nullptr;
	std::vector<TH2D*> v_hin;
	std::vector<TH1D*> v_hin_barrel;

	vector<vector<double>> v_phi_L;
	int v_dphi[6];
	
	double Z=-150;



	
//Output
	double phi_min=0.3;
  	double phi_max=0.5;
  	int phi_bin=64;//96?
  	double qA_pT_min=-3e-4;
  	double qA_pT_max=3e-4;
  	int qA_pT_bin=216;
	TH2D* hout;
	vector<TH2D*>v_hout;
	TH2D *hout_merge=nullptr;


//LUT

	TFile *LUT=nullptr;
	TTree *luttree=nullptr;
	int in_min, in_max, xi, yi, layer, r_bit;







};
#endif
