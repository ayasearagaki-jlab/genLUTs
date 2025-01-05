#ifndef Hit_H
#define Hit_H
#include <string>
#include <iostream>
struct Hit
{
	float x;
	float y;
	float z;
	float R;
	float GPhi;
	int detType;
	int detZone;
	int hitType;
	unsigned layer_disk;
	unsigned physlayer;
	unsigned side;
	int etamo;
	unsigned phimo;
	int phiindex;
	int etaindex;
	unsigned phiwidth;
	unsigned etawidth;
	unsigned identiferharsh;
	
};

std::string ar_dettype[4]={"strip", "pixel", "nTechs", "undefined"};
std::string ar_detzone[5]={ "barrel", "posEndcap", "negEndcap", "nZones", "undefined" };
std::string ar_hittype[8]={"unmapped", "mapped", "clustered", "wildcard", "guessed", "undefined", "extrapolated", "spacepoint"};

std::ostream& operator<<(ostream& os, const Hit& hit) {
    
	os	<<"r: " << hit.R
       	<<" ,phi: " << hit.GPhi
		<<" ,z: " <<hit.z
       	<< " ,detType: " <<ar_dettype[hit.detType] 
		<<" ,detZone: "<<ar_detzone[hit.detZone]
		<<" ,physlayer: "<<hit.physlayer
		<<" ,layerdisk: "<<hit.layer_disk
		<<" ,side: "<<hit.side
		<<" ,identiferharsh: "<<hit.identiferharsh
		<<" ,hitType: "<<ar_hittype[hit.hitType]
		<<" ,etamodule: "<<hit.etamo
		<<" ,phimodule: "<<hit.phimo
		<<" ,etaindex: "<<hit.etaindex
		<<" ,phiindex: "<<hit.phiindex;
    return os;
}

#endif

