//Wrapperclass_part1.cpp
#ifndef Wrapperclasscpp1
#define Wrapperclasscpp1

#include "Wrapperclass.h"
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include"Hit.h"

//Clustered part


void SpilitHits(){
	hitsPerModule.clear();
	if(input_hits.size()==0){cerr<<"input hits is null !! Do GetHit"<<endl;return;}
	for(auto hit:Input_hits){
		if(hitsPerModule.size()==0){
			vector<Hit>hits;
			hits.push_bask(hit);
			hitsPerModule.push_back(hits);		
		}else{
			bool addedToModule = false;
			for(int i=0;i<hitsPerModule.size();i++){
				if(hitsPerModule.at(i).at(0).identiferharsh==hit.identiferharsh){
					hitsPerModule.at(i).push_back(hit);	
					addedToModule=true;	
					break;
				}
			}//for hitspermodule size
			if(addedToModule){continue;
			}else{
				vector<Hit>hits;
				hits.push_bask(hit);
				hitsPerModule.push_back(hits);					

				}		
	
		}	

	}//for hit loop


}


void GlowCluster(){
	if(hitsPerModule.size()==0){cerr<<"hitsPerModule is null!! Do SpilitHits!!"<<endl;return;}
	for(int mo=0;mo<hitsPerModule.size();mo++){
		if(hitsPerModule.at(mo).size()==1){
			//hitsPerModule.at(mo).at(0).hitType=2;
			Clustered.push_back(hitsPerModule.at(mo).at(0));	
		}else{
			vector<Hit>ClusterEquiv;
			for(auto hit:hitsPerModule.at(mo)){
				if(hit.detType==0){PixelCluster(hit,ClusterEquiv);}
				


			
				


			}
			
		}

	}//mo for loop	



}






 

#endif
