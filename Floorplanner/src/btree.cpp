#include "btree.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
using namespace std;

//BStree Function
//operation設定function------------------------
void Btree::adjust_tree(){
	int _op=rand()%3;
	int _a=rand()%_block_num;
	int _b=rand()%_block_num;
	while(_a==_b){ _b=rand()%_block_num; }
	switch(_op){
    	case 0: 
    		rotate_node(_a); 
    		break;
    	case 1: 
    		swap_node(_a,_b); 
    		break;
        case 2: 
        	delete_node(_a); 
        	insert_node(_a,_b);
        	break;
    }   
}
//--------------------------------------------

void Btree::initial_tree(){
	int _divide;
	int _mod;
	_root_id=0;
	for(int i=0;i<blocks.size();i++){
		if(i==0){ blocks[i].set_id_plr(-1,-1,-1); }
		else{
			_divide=(i-1)/2;
			_mod=(i-1)%2;
			blocks[i].set_id_plr(_divide,-1,-1);
			if(_mod==0){ blocks[_divide].set_id_l(i); }
			else{ blocks[_divide].set_id_r(i); }
		}
	}
}
void Btree::print_tree(){
	cout<<setw(8)<<right<<"Name";
	cout<<setw(7)<<"ID"<<setw(7)<<"ID_L"<<setw(7)<<"ID_R"<<setw(7)<<"ID_P";
	cout<<setw(7)<<"X"<<setw(7)<<"Y";
	cout<<setw(8)<<"Width"<<setw(8)<<"Height";
	cout<<endl;
	for(int i=0;i<blocks.size();i++){
		cout<<setw(8)<<blocks[i].get_name();
		cout<<setw(7)<<blocks[i].get_id();
		cout<<setw(7)<<blocks[i].get_id_l();
		cout<<setw(7)<<blocks[i].get_id_r();
		cout<<setw(7)<<blocks[i].get_id_p();
		cout<<setw(7)<<blocks[i].get_x();
		cout<<setw(7)<<blocks[i].get_y();
		cout<<setw(8)<<blocks[i].get_width();
		cout<<setw(8)<<blocks[i].get_height();

		cout<<endl;
	}
	cout<<"Root ID = "<<_root_id<<endl;
	//pair<int, int> cost=this->contour();
	//cout<<"Best= "<<"("<<cost.first<<","<<cost.second<<")"<<endl;
}

void Btree::rotate_node(int &_a){
	int _pri_width=blocks[_a].get_width();
	blocks[_a].set_width(blocks[_a].get_height());
	blocks[_a].set_height(_pri_width);
}
void Btree::swap_node(int &_a,int &_b){
	string _pri_name=blocks[_a].get_name();
	int    _pri_width=blocks[_a].get_width();
	int    _pri_height=blocks[_a].get_height();
	blocks[_a].set_name(blocks[_b].get_name());
	blocks[_a].set_width(blocks[_b].get_width());
	blocks[_a].set_height(blocks[_b].get_height());
	blocks[_b].set_name(_pri_name);
	blocks[_b].set_width(_pri_width);
	blocks[_b].set_height(_pri_height);	
}
void Btree::delete_node(int &_a){
	//判斷為左分支還是右分支
	int _c=blocks[_a].get_id_p();
	int _c_l=blocks[_c].get_id_l();
	int _c_r=blocks[_c].get_id_r();
	//紀錄左子點及右子點
	int _d=blocks[_a].get_id_l();
	int _e=blocks[_a].get_id_r();
	//----開始計算----
	//刪除點為root
	if(_c==(-1)){ delete_root(_a,_d,_e); }
	//刪除點為左分支
	else if(_a==_c_l){
		if(_d==(-1)&&_e==(-1)){ blocks[_c].set_id_l(-1); }
		else if(_d==(-1)){
			blocks[_c].set_id_l(_e);
			blocks[_e].set_id_p(_c);
		}
		else if(_e==(-1)){
			blocks[_c].set_id_l(_d);
			blocks[_d].set_id_p(_c);
		}
		else{
			int _op=rand()%2;
			switch(_op){
				case 0:
					blocks[_c].set_id_l(_d);
					blocks[_e].set_id_p(_d);
					adjust_node_mpl(_d,_c,_e);
					break;
				case 1:
					blocks[_c].set_id_l(_e);
					blocks[_d].set_id_p(_e);
					adjust_node_mpr(_e,_c,_d);
					break;
			}
		}
	}
	//刪除點為右分支
	else if(_a==_c_r){
		if(_d==(-1)&&_e==(-1)){ blocks[_c].set_id_r(-1); }
		else if(_d==(-1)){
			blocks[_c].set_id_r(_e);
			blocks[_e].set_id_p(_c);
		}
		else if(_e==(-1)){
			blocks[_c].set_id_r(_d);
			blocks[_d].set_id_p(_c);
		}
		else{
			int _op=rand()%2;
			switch(_op){
				case 0:
					blocks[_c].set_id_r(_d);
					blocks[_e].set_id_p(_d);
					adjust_node_mpl(_d,_c,_e);
					break;
				case 1:
					blocks[_c].set_id_r(_e);
					blocks[_d].set_id_p(_e);
					adjust_node_mpr(_e,_c,_d);
					break;
			}
		}
	}
}
void  Btree::delete_root(int &_a,int &_d,int &_e){
	//分成四種情況考慮
	if(_d==(-1)&&_e==(-1)){
		blocks[_a].set_id_plr(-1,-1,-1);
		_root_id=-1;
	}
	else if(_e==(-1)){
		blocks[_a].set_id_plr(-1,-1,-1);
		blocks[_d].set_id_p(-1);
		_root_id=_d;
	}
	else if(_d==(-1)){
		blocks[_a].set_id_plr(-1,-1,-1);
		blocks[_e].set_id_p(-1);
		_root_id=_e;
	}
	else{
		int _op=rand()%2;
		int _root_p=(-1);
		switch(_op){
			case 0:
				blocks[_e].set_id_p(_d);
				_root_id=_d;
				adjust_node_mpl(_d,_root_p,_e);
				break;
			case 1:
				blocks[_d].set_id_p(_e);
				_root_id=_e;
				adjust_node_mpr(_e,_root_p,_d);
				break;
		}
	}
}
void Btree::adjust_node_mpl(int &_d,int &_c,int &_e){
	int _f=blocks[_d].get_id_l();
	int _g=blocks[_d].get_id_r();
	if(_f==(-1)&&_g==(-1)){ blocks[_d].set_id_plr(_c,-1,_e); }
	else if(_g==(-1)){ blocks[_d].set_id_plr(_c,_f,_e); }
	else if(_f==(-1)){ blocks[_d].set_id_plr(_c,_g,_e); }
	else{
		int _op=rand()%2;
		switch(0){
			case 0:
				blocks[_d].set_id_plr(_c,_f,_e);
				blocks[_g].set_id_p(_f);
				adjust_node_mpl(_f,_d,_g);
				break;
			case 1:
				blocks[_d].set_id_plr(_c,_g,_e);
				blocks[_f].set_id_p(_g);
				adjust_node_mpr(_g,_d,_f);
				break;
		}
	}
}
void Btree::adjust_node_mpr(int &_e,int &_c,int &_d){
	int _f=blocks[_e].get_id_l();
	int _g=blocks[_e].get_id_r();
	if(_f==(-1)&&_g==(-1)){ blocks[_e].set_id_plr(_c,_d,-1); }
	else if(_g==(-1)){ blocks[_e].set_id_plr(_c,_d,_f); }
	else if(_f==(-1)){ blocks[_e].set_id_plr(_c,_d,_g); }
	else{
		int _op=rand()%2;
		switch(0){
			case 0:
				blocks[_e].set_id_plr(_c,_d,_g);
				blocks[_f].set_id_p(_g);
				adjust_node_mpr(_g,_e,_f);
				break;
			case 1:
				blocks[_e].set_id_plr(_c,_d,_f);
				blocks[_g].set_id_p(_f);
				adjust_node_mpl(_f,_e,_g);
				break;
		}

	}
}
void Btree::insert_node(int &_a,int &_b){
	if(blocks[_b].get_id_p()==_a){
		int _a_p=blocks[_a].get_id_p();
		if(blocks[_a_p].get_id_l()==_a){ blocks[_a_p].set_id_l(_b); }
		else{ blocks[_a_p].set_id_r(_b); }
		blocks[_b].set_id_p(_a_p);
	}
	int _ins_id_l=blocks[_b].get_id_l();//_c
	int _ins_id_r=blocks[_b].get_id_r();//_d
	if(_ins_id_l==(-1)&&_ins_id_r==(-1)){
		int _op=rand()%2;
		switch(_op){
			case 0:
				blocks[_b].set_id_l(_a);
				blocks[_a].set_id_plr(_b,-1,-1);
				break;
			case 1:
				blocks[_b].set_id_r(_a);
				blocks[_a].set_id_plr(_b,-1,-1);
				break;
		}
	}
	else if(_ins_id_r==(-1)){
		int _op=rand()%3;
		switch(_op){
			case 0:
				blocks[_b].set_id_l(_a);
				blocks[_a].set_id_plr(_b,_ins_id_l,-1);
				blocks[_ins_id_l].set_id_p(_a);
				break;
			case 1:
				blocks[_b].set_id_l(_a);
				blocks[_a].set_id_plr(_b,-1,_ins_id_l);
				blocks[_ins_id_l].set_id_p(_a);
				break;
			case 2:
				blocks[_b].set_id_r(_a);
				blocks[_a].set_id_plr(_b,-1,-1);
				break;
		}
	}
	else if(_ins_id_l==(-1)){
		int _op=rand()%3;
		switch(_op){
			case 0:
				blocks[_b].set_id_l(_a);
				blocks[_a].set_id_plr(_b,-1,-1);
				break;
			case 1:
				blocks[_b].set_id_r(_a);
				blocks[_a].set_id_plr(_b,_ins_id_r,-1);
				blocks[_ins_id_r].set_id_p(_a);
				break;
			case 2:
				blocks[_b].set_id_r(_a);
				blocks[_a].set_id_plr(_b,-1,_ins_id_r);
				blocks[_ins_id_r].set_id_p(_a);
				break;
		}
	}
	else{
		int _op=rand()%2;
		switch(_op){
			case 0:
				blocks[_b].set_id_l(_a);
				blocks[_a].set_id_plr(_b,_ins_id_l,-1);
				blocks[_ins_id_l].set_id_p(_a);
				break;
			case 1:
				blocks[_b].set_id_r(_a);
				blocks[_a].set_id_plr(_b,-1,_ins_id_r);
				blocks[_ins_id_r].set_id_p(_a);
				break;
		}
	}
}
