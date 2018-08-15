#ifndef router
#define router
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include <math.h>
#include <climits>

using namespace std;
class Net;
class Pin;
class F_net;

class Router
{
public:
	Router(){}
	void parser(const string& inputfile);
	void initial_tree();
	void kruskal();
	void rectilinear(Pin *p1,Pin *p2);
	void output_file(const string& outputfile);
private:
	fstream    	  fp;
	int 	      _boundary_x;
	int 	      _boundary_y;
	int           _number_pin;
	int 		  total_distance;
	vector<Pin>   _pin;
	vector<Net>   _net;
	vector<F_net> Final;
};

class Pin
{
public:
	Pin(string _iname,int _ix,int _iy,int _iid):_name(_iname),_x(_ix),_y(_iy),_id(_iid){}
	Pin(int _ix,int _iy,int _iid):_x(_ix),_y(_iy),_id(_iid){ _name="steiner"; }
	//get
	void get_pin_info(){ cout<<_name<<"  ("<<_x<<","<<_y<<")"; }
	string get_name(){ return _name; }
	int get_x(){ return _x; }
	int get_y(){ return _y; }
	int get_id(){ return _id; }
	int get_cluster_num(){ return cluster_num; }
	//set
	void set_cluster_num(int i){ cluster_num=i; }
	int 	     _x;
	int 		 _y;
private:
	string		 _name;
	int          _id;
	int          cluster_num;
};

class Net
{
public:
	Net(Pin &ipin_l,Pin &ipin_r):pin_l(&ipin_l),pin_r(&ipin_r){ manhattan_distance(); }
	void manhattan_distance(){ _m_distance=abs( pin_l->get_x()-pin_r->get_x() )+abs( pin_l->get_y()-pin_r->get_y() ); }
	Pin *get_pin_l(){ return pin_l; }
	Pin *get_pin_r(){ return pin_r; }
	int  _m_distance;
private:
	Pin *pin_l;
	Pin *pin_r;
};

class F_net
{
public:
	F_net(int l_x,int l_y,int r_x,int r_y,bool v):
	pin_l_x(l_x),pin_l_y(l_y),pin_r_x(r_x),pin_r_y(r_y),v_line(v){
		if(pin_l_x==pin_r_x){
			if(pin_l_y>pin_r_y){ int swap_y=pin_l_y; pin_l_y=pin_r_y; pin_r_y=swap_y; } 
		}
		else{
			if(pin_l_x>pin_r_x){ int swap_x=pin_l_x; pin_l_x=pin_r_x; pin_r_x=swap_x; } 
		}
	}
	int 	pin_l_x;
	int 	pin_l_y;	
	int 	pin_r_x;	
	int 	pin_r_y;
	bool 	v_line;
};

#endif