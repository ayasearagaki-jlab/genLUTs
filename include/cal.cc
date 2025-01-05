//cal.cc
std::vector<double> z={1500,1700,1940,2230,2525,2840};
double eta_max=2.16893 ;
double eta_min=2.16893 ;
double Z_max=88.5566;
double Z_min=88.5566;
double D0=1.03414;
void cal(){

std::vector<double> r_max;
std::vector<double> r_min;


for(int i=0;i<z.size();i++){
			double ri_max=(z.at(i)-Z_min)*tan(2.0*atan(exp(-eta_max)))+D0;
			double ri_min=(z.at(i)-Z_max)*tan(2.0*atan(exp(-eta_min)))+D0;

			r_max.push_back(ri_max);
			r_min.push_back(ri_min);
		}


for(int i=0;i<z.size();i++){

	printf("[%8.5f,%8.5f] dr=%8.5f\n",r_min.at(i),r_max.at(i),r_max.at(i)-r_min.at(i));


}




}

