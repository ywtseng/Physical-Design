#include "router.h"
#include <sstream>
#include <stdlib.h>
using namespace std;

void Router::parser(const string& inputfile){
	fp.open(inputfile.c_str(),ios::in);
	string _temp,_temp_b,token,_position;
	int x;
	fp>>_temp>>_temp>>_temp>>_temp;
	//deal with first line
	stringstream se(_temp);
	for(int i=0;i<2;i++){
		getline(se,token, ',');
		if(token[0]=='('){ token.erase(0,1);  }
		else if(token[token.size()-1]==')'){ 
			token.erase(token.size()-1,token.size()); 
		}
		//save data
		if(i==0){ _boundary_x=atoi(token.c_str() ); }
		if(i==1){ _boundary_y=atoi(token.c_str() ); }
	}
	//deal with second line
	fp>>_temp>>_temp>>_number_pin;
	//deal with Pin line
	for(int i=0;i<_number_pin;i++){
		int _x_position;
		int _y_position;
		fp>>_temp>>_temp>>_position;
		stringstream su(_position);
		while(getline(su,token, ',')){
			if(token[0]=='('){ 
				token.erase(0,1);
				_x_position=atoi(token.c_str() );  
			}
			else if(token[token.size()-1]==')'){ 
				token.erase(token.size()-1,token.size()); 
				_y_position=atoi(token.c_str() );
			}
		}
		_pin.push_back(Pin(_temp,_x_position,_y_position,i+1));
	}
	cout<<"Boundary=("<<_boundary_x<<","<<_boundary_y<<")"<<endl;
	cout<<"Pin size= "<<_pin.size()<<endl;
	fp.close();
}

