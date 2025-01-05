//Wrapperclass.h

#ifndef Wrapperclass
#define Wrapperclass
#include"Hit.h"

class WrapperClass{
public:
	WrapperClass(int reg=3){
		TString filename="../wrapper/reg"+(TString)to_string(reg)+"_result.root";
		file=new TFile(filename,"read");
		if(!file || file->IsZombie()){
			cerr<<"Error: wrapper is not exit!!"<<endl;
			delete file;
			file=nullptr;
			exit(1);
		}
		cout<<"open region "<<reg<<"Wrapper!!"<<endl;
		
		Tree=(TTree*)file->Get("eventhit");
		if(Tree){cout<<"Get Tree!!"<<endl;}
		Tree->SetBranchAddress("x",&v_x);
        Tree->SetBranchAddress("y",&v_y);
        Tree->SetBranchAddress("z",&v_z);
        Tree->SetBranchAddress("R",&v_R);
        Tree->SetBranchAddress("GPhi",&v_GPhi);
        Tree->SetBranchAddress("detType",&v_detType);
        Tree->SetBranchAddress("detZone",&v_detZone);
        Tree->SetBranchAddress("side",&v_side);           
        Tree->SetBranchAddress("layer_disk",&v_layer_disk);
        Tree->SetBranchAddress("physLayer",&v_physlayer);
        Tree->SetBranchAddress("etamod",&v_etamo);
        Tree->SetBranchAddress("phimod",&v_phimo);
        Tree->SetBranchAddress("phiindex",&v_phiindex);
        Tree->SetBranchAddress("etaindex",&v_etaindex);
        Tree->SetBranchAddress("hittype",&v_hittype);
        Tree->SetBranchAddress("etawidth",&v_etawidth);
        Tree->SetBranchAddress("phiwidth",&v_phiwidth);
        Tree->SetBranchAddress("identiferharsh",&v_identiferharsh);

		
			
	
	}

	~WrapperClass(){
		if(file&&file->IsOpen()){delete Tree;}
		delete file;
		delete v_x;
        delete v_y;
        delete v_z;
        delete v_R;
    	delete v_GPhi;
        delete v_side;
        delete v_detType;
        delete v_physlayer;
	    delete v_detZone;
        delete v_layer_disk;
        delete v_etamo;
        delete v_phimo;
        delete v_phiindex;
        delete v_etaindex;
        delete v_hittype;
        delete v_etawidth;
        delete v_phiwidth;
        delete v_identiferharsh;
		
		
	}

	void GetHit(int id){
		event_num=id;
		if(input_hits.size()>0){input_hits.clear();cout<<"input hits clear!!"<<endl;}
		if(final_hits.size()>0){final_hits.clear();cout<<"final hits clear!!"<<endl;}

		if(!Tree){cerr<<"Wrapper Tree is null !!"<<endl;return;}
		Tree->GetEntry(id);
		if(!v_z){cerr<<"v_* is nullptr!!"<<endl;return;}
		for(int arg=0;arg<v_z->size();arg++){
		if((v_detType->at(arg)==1&&v_layer_disk->at(arg)==4)||v_detType->at(arg)==0){
			Hit hit;
			hit.x=v_x->at(arg);
			hit.y=v_y->at(arg);
			hit.z=v_z->at(arg);
			hit.R=v_R->at(arg);
			hit.GPhi=v_GPhi->at(arg);
			hit.side=v_side->at(arg);
			hit.detType=v_detType->at(arg);
			hit.detZone=v_detZone->at(arg);
			hit.hitType=v_hittype->at(arg);
			hit.physlayer=v_physlayer->at(arg);
			hit.layer_disk=v_layer_disk->at(arg);
			hit.etamo=v_etamo->at(arg);
			hit.phimo=v_phimo->at(arg);
			hit.phiindex=v_phiindex->at(arg);
			hit.etaindex=v_etaindex->at(arg);
			hit.etawidth=v_etawidth->at(arg);
			hit.phiwidth=v_phiwidth->at(arg);
			hit.identiferharsh=v_identiferharsh->at(arg);
			input_hits.push_back(hit);
		}//if
		}//for
	}

	void CloseWrapper(){
		if(file){delete Tree;Tree=nullptr;}
		if(file&&file->IsOpen()){file->Close();cout<<"closed Tfile Wrapper"<<endl;return;}
		else{cerr<<"Wrapper Tfile is not open? What's up ?"<<endl;return;}
	}

	int GetNevent(){
		if(!Tree){cerr<<"Wrapper Tree is null !!"<<endl;return-1;}

		return Tree->GetEntries();
	}

	void CatEvent(){
		if(input_hits.size()==0){cerr<<"input hits is null !! Do GetHit"<<endl;return;}
		for(int arg=0;arg<input_hits.size();arg++){
			cout<<input_hits.at(arg)<<endl;
		}
		return;
	}
	
//Clustering
	void DoCluster();
	void SpilitHits();	
	void GlowCluster();
	
	



	

private:
	TTree *Tree=nullptr;
	TFile *file=nullptr;
	std::vector<Hit>input_hits;
	std::vector<Hit>final_hits;
	
	
	std::vector<float>*v_x=nullptr;
    std::vector<float>*v_y=nullptr;
    std::vector<float>*v_z=nullptr;
    std::vector<float>*v_R=nullptr;
    std::vector<float>*v_GPhi=nullptr;
    std::vector<int>*v_detType=nullptr;
    std::vector<int>*v_detZone=nullptr;
    std::vector<unsigned>*v_layer_disk=nullptr;	
	std::vector<unsigned>*v_physlayer=nullptr;	
	std::vector<unsigned>*v_side=nullptr;	
	std::vector<int>*v_etamo=nullptr;
    std::vector<unsigned>*v_phimo=nullptr;
    std::vector<int>*v_phiindex=nullptr;
    std::vector<int>*v_etaindex=nullptr;
	std::vector<int>*v_hittype=nullptr;	
	std::vector<unsigned>*v_etawidth=nullptr;
    std::vector<unsigned>*v_phiwidth=nullptr;
    std::vector<unsigned>*v_identiferharsh=nullptr;
	int event_num=0;





	//clustering
	std::vector<std::vector<Hit>>hitsPerModule;
	std::vector<Hit>Clustered;
	
	


};


#endif
