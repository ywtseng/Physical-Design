#ifndef btree
#define btree
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
using namespace std;

class Block;
class Terminal;
class Net;
class Node; 

class Btree{
public:
	Btree(){  _fit_outline=false; }
	//parser.cpp
	void  input_file(const string& inputfile_block,const string& inputfile_net);
		Terminal* find_terminal(string &_name);
	//Btree.cpp
	void  initial_tree();
	void  print_tree();
	void  adjust_tree();
		void  rotate_node(int &_a);
		void  swap_node(int &_a,int &_b);
		void  delete_node(int &_a);
			void  delete_root(int &_a,int &_d,int &_e);
			void  adjust_node_mpl(int &_d,int &_c,int &_e);
			void  adjust_node_mpr(int &_e,int &_c,int &_d);
		void  insert_node(int &_a,int &_b);
	//contour.cpp
	double  HPWL();
	double  count_cost(double &alpa, double &beta);
	void contour(int &max_X,int &max_Y);
	//sa.cpp
	void sa_algorithm(double &alpa,double &beta);
		bool get_fit_outline(){ return _fit_outline; }
		void set_root_id(int i){ _root_id=i; }
		int  get_root_id(){ return _root_id; }	
	void special_case(double &alpa, double &beta);
	void output_file(const string& outputfile,double &alpha,double &runtime);
private:
	//parser.cpp
	fstream 			fp;
	int   				_outline_w;
	int   				_outline_h;
	int   				_block_num;
	int   				_terminal_num;
	int   				_net_num;
	bool  				_fit_outline;//可以放入格子
	int   				_root_id;
	vector<Block>	   	blocks;
	vector<Terminal>   	terminals;
	vector<Net>			nets;
	//sa.cpp
	int 				_best_solution;
};

class Block
{
public:
	Block(string _iname,int _iwidth,int _iheight):_name(_iname),_width(_iwidth),_height(_iheight){}
	Block(string _iname,int _iwidth,int _iheight,int _i_id):
	_name(_iname),_width(_iwidth),_height(_iheight),_id(_i_id){}
	//parser.cpp
	void set_name(string i){ _name=i;    }
	void set_width(int i) { _width=i;   }
	void set_height(int i){ _height=i;  }
	string get_name(){ return _name;    }
	int get_width()  { return _width;   }
	int get_height() { return _height;  }
	//Btree.cpp. ~Tree位置資訊
	void set_id(int i)    { _id=i;      }
	void set_id_p(int i)  { _id_p=i;    }
	void set_id_l(int i)  { _id_l=i;    }
	void set_id_r(int i)  { _id_r=i;    }
	void set_id_plr(int p,int l,int r){ _id_p=p; _id_l=l; _id_r=r;  }
	int get_id()  { return _id;    }
	int get_id_p(){ return _id_p;  }
	int get_id_l(){ return _id_l;  }
	int get_id_r(){ return _id_r;  }
	//contour.cpp  ~Floorplan位置資訊
	void set_x(int i) { _x=i; }
	void set_y(int i) { _y=i; }
	int get_x() { return _x; }
	int get_y() { return _y; }
	list<Node>::iterator  	bt;
private:
	//parser.cpp
	string  				_name;
	int 					_width;
	int						_height;
	//Btree.cpp. ~Tree位置資訊
	int 					_id;
	int 					_id_p;
	int						_id_l;
	int						_id_r;
	//contour.cpp ~Floorplan位置資訊
	int 					_x;
	int						_y;
};

class Terminal
{
public:
	Terminal(string _iname,Block* b,int _i,bool t): _name(_iname),block(b),_id(_i),_type(t){}
    Terminal(string _iname,int _ix,int _iy,int _i,bool t): _name(_iname), _x(_ix), _y(_iy),_id(_i),_type(t){ block=NULL; }
	string get_name(){ return _name; }
	int get_x(){ return _x; }
	int get_y(){ return _y; }
	int get_type(){ return _type; }
	int get_id(){ return _id; }

	Block   *block;
private:
	string  _name;
	int		_x;
	int		_y;
	int     _id;
	bool    _type;//如果為true代表為block
};

class Net
{
public:
	Net(int _idegree):_degree(_idegree){}
	void add_terminal(Terminal* t){ this->net_of_terminals.push_back(t); }
	Terminal* get_terminal(int i){
		if(i>=this->_degree){ return NULL; }
    	else{ return net_of_terminals[i]; } 
	}
	int get_degree(){ return _degree; }
private:
	int    				_degree;
	vector<Terminal*>   net_of_terminals;
};

#endif
