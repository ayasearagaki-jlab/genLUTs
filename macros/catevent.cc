//catevent.cc
#include"../include/Wrapperclass.h"

WrapperClass Wrap(0); 
void catevent(){
	int event_num=0;//FIXME
	Wrap.GetHit(0);
	Wrap.CatEvent();
	Wrap.CloseWrapper();


	/*std::vector<float>*v_x=nullptr;
    std::vector<float>*v_y=nullptr;
    std::vector<float>*v_z=nullptr;
    std::vector<float>*v_R=nullptr;
    std::vector<float>*v_GPhi=nullptr;
    std::vector<int>*v_detType=nullptr;
    std::vector<int>*v_detZone=nullptr;
    std::vector<unsigned>*v_physlayer=nullptr;	
	std::vector<unsigned>*v_side=nullptr;	
	std::vector<unsigned>*v_layer_disk=nullptr;	
	


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

	TH2D *hist=new TH2D("hist","hist",900,200,3000,100,0,800);
		tree->GetEntry(event_num);
		cout<<"all of # of hits is "<<v_z->size()<<endl;
		for(int arg=0;arg<v_z->size();arg++){
			cout<<"z="<<v_z->at(arg)<<endl;
			cout<<"r="<<v_R->at(arg)<<endl;
			cout<<"Phi="<<v_GPhi->at(arg)<<endl;
			cout<<"detType="<<v_detType->at(arg)<<endl;
			cout<<"detZone="<<v_detZone->at(arg)<<endl;
			cout<<"layer_num="<<v_layer_disk->at(arg)<<endl;
			cout<<"side="<<v_side->at(arg)<<endl;
			cout<<"physLayer="<<v_physlayer->at(arg)<<endl;

	

			hist->Fill(v_z->at(arg),v_R->at(arg));
			//if(v_detType->at(arg)==1&&v_detZone->at(arg)==2){lut.WritingLUT(v_GPhi->at(arg),v_layer_disk->at(arg),v_R->at(arg));count+=1;}
			


		}

	


	
	
	hist->Draw("colz");
	hist->SetDirectory(0);//closeした後にもhistが残る処理	
	wrapper->Close();


*/









}
