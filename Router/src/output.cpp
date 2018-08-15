#include "router.h"
#include <stack>
#include <stdlib.h> 
using namespace std;
void Router::output_file(const string &outputfile){
	fp.open(outputfile.c_str(),ios::out);
	fp<<"NumRoutedPins = "<<_pin.size()<<endl;
	fp<<endl;
	fp<<"WireLength = "<<total_distance<<endl;
    fp<<endl;
	for(int i=0;i<Final.size();i++){
		if(Final[i].v_line==true){ fp<<"V-line";}
		else{ fp<<"H-line"; }
		fp<<"  ";
		fp<<"("<<Final[i].pin_l_x<<","<<Final[i].pin_l_y<<")";
		fp<<" ("<<Final[i].pin_r_x<<","<<Final[i].pin_r_y<<")";
		fp<<endl;
	}
	fp.close();

}