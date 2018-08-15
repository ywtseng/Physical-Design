#include "btree.h"
#include <iostream>
#include <iomanip>
#include <stack>
using namespace std;

void Btree::input_file(const string& inputfile_block,const string& inputfile_net){
	//Input file:block------------
	fp.open(inputfile_block.c_str(),ios::in);
	string  _temp,_terminal_name;
	int 	_width,_height;//_width=x,_height=y
	fp>>_temp>>_outline_w>>_outline_h;
	fp>>_temp>>_block_num;
	fp>>_temp>>_terminal_num;
	for(int i=0;i<_block_num;i++){
		fp>>_temp>>_width>>_height;
		blocks.push_back(Block(_temp,_width,_height,i));
		terminals.push_back(Terminal(_temp,&blocks[i],i,true));
	}
	for(int i=0;i<_terminal_num;i++){
		fp>>_terminal_name>>_temp>>_width>>_height;
		//cout<<"T= "<<_width<<","<<_height<<endl;
		terminals.push_back(Terminal(_terminal_name,_width,_height,_block_num+i,false));
	}
	fp.close();
	//Input file:nets-------------
	fp.open(inputfile_net.c_str(),ios::in);
	int _degree=0;
	fp>>_temp>>_net_num;
	for(int i=0;i<_net_num;i++){
		fp>>_temp>>_degree;
		Net net(_degree);
		for(int i=0;i<_degree;i++){
			fp>>_temp;
			Terminal *t =find_terminal(_temp);
			net.add_terminal(t);
		}
		nets.push_back(net);
		//cout<<endl;
	}
	fp.close();
}
Terminal* Btree::find_terminal(string &_name){

	for(int i=0;i<terminals.size();i++){
		if(terminals[i].get_name()==_name){ return &(terminals[i]); }
	}
	return NULL;
}