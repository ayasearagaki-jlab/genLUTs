//readevent.cc
#include "../include/LUTclass.h"
#include "../include/LUTclass.cpp"
#include "../include/FPGAenum.h"
LUTClass lut(false);


void barrelreadevent(){
	int resolution = 512;//FIXME
	int event_num=0;//FIXME


	lut.SetLUT(resolution);
	
	std::vector<float>*v_x=nullptr;
    std::vector<float>*v_y=nullptr;
    std::vector<float>*v_z=nullptr;
    std::vector<float>*v_R=nullptr;
    std::vector<float>*v_GPhi=nullptr;
    std::vector<int>*v_detType=nullptr;
    std::vector<int>*v_detZone=nullptr;
    std::vector<unsigned>*v_physlayer=nullptr;	
	std::vector<unsigned>*v_side=nullptr;	
	std::vector<unsigned>*v_layer_disk=nullptr;	
	std::vector<int>*v_etamo=nullptr;
    std::vector<unsigned>*v_phimo=nullptr;
    std::vector<int>*v_phiindex=nullptr;
    std::vector<int>*v_etaindex=nullptr;
	std::vector<int>*v_hittype=nullptr;	


	TFile *wrapper=new TFile("../wrapper/reg0_result.root","read");

	TTree *tree=(TTree*)wrapper->Get("eventhit");
	
                 tree->SetBranchAddress("x",&v_x);
                 tree->SetBranchAddress("y",&v_y);
                 tree->SetBranchAddress("z",&v_z);
                 tree->SetBranchAddress("R",&v_R);
                 tree->SetBranchAddress("GPhi",&v_GPhi);
                 tree->SetBranchAddress("detType",&v_detType);
                 tree->SetBranchAddress("detZone",&v_detZone);
				 tree->SetBranchAddress("side",&v_side);
				 tree->SetBranchAddress("physLayer",&v_physlayer);
                 tree->SetBranchAddress("layer_disk",&v_layer_disk);
				 tree->SetBranchAddress("etamod",&v_etamo);
                 tree->SetBranchAddress("phimod",&v_phimo);
                 tree->SetBranchAddress("phiindex",&v_phiindex);
                 tree->SetBranchAddress("etaindex",&v_etaindex);
				 tree->SetBranchAddress("hittype",&v_hittype);



	//TH2D *hist=new TH2D("hist","hist",900,200,3000,100,0,800);
		tree->GetEntry(event_num);
		cout<<"all of # of hits is "<<v_z->size()<<endl;
		int count=0;
		for(int arg=0;arg<v_z->size();arg++){
			//cout<<"z="<<v_z->at(arg)<<endl;
			//cout<<"r="<<v_R->at(arg)<<endl;
			//cout<<"Phi="<<v_GPhi->at(arg)<<endl;
			//cout<<"detType="<<v_detType->at(arg)<<endl;
			//cout<<"detZone="<<v_detZone->at(arg)<<endl;
			//cout<<"layer_num="<<v_layer_disk->at(arg)<<endl;

			//hist->Fill(v_z->at(arg),v_R->at(arg));
			if(v_detType->at(arg)==0&&v_detZone->at(arg)==0){lut.WritingLUT(v_GPhi->at(arg),(v_layer_disk->at(arg))+1,v_physlayer->at(arg)+1);count+=1;
				cout<<"phi index= "<<v_phiindex->at(arg);
				cout<<" eta index= "<<v_etaindex->at(arg);
				cout<<" phi Module= "<<v_phimo->at(arg);
				cout<<" eta Module= "<<v_etamo->at(arg);
				cout<<" hittype= "<<v_hittype->at(arg)<<endl;



			}
			if(v_detType->at(arg)==1&&v_detZone->at(arg)==0&&v_layer_disk->at(arg)==4){
				lut.WritingLUT(v_GPhi->at(arg),0,0);count+=1;
			    cout<<"phi index="<<v_phiindex->at(arg);
				cout<<"eta index="<<v_etaindex->at(arg);
				cout<<"phi Module="<<v_phimo->at(arg);
				cout<<"eta Module="<<v_etamo->at(arg);
				cout<<" hittype= "<<v_hittype->at(arg)<<endl;

			
			}
	
			


		}
		cout<<"strip hit is "<<count<<endl;
		lut.FlatEachHoughPlane();

		TH2D* hist_merge;
		hist_merge=lut.GetAndMergeHoughPlane();
		hist_merge->SetStats(0);
		hist_merge->Draw("colz");
		cout<<"Max count is "<<hist_merge->GetMaximum()<<endl;

		lut.CloseLUT();
	


	
	
	//hist->Draw("colz");
	hist_merge->SetDirectory(0);//closeした後にもhistが残る処理	
	wrapper->Close();
	
	TFile* file=new TFile("../checkresult/sample/localresult_new.root","recreate");
	hist_merge->Write("localresult_event0");
	file->Close();











}
