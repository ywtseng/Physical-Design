#include "router.h"
#include <algorithm> 
using namespace std;

class Cluster{
public:
	Cluster(int _iid):_id(_iid){ _pin_set.clear(); }
	void add_pin(Pin* pin){ _pin_set.push_back(pin); }
	void union_cluster(Cluster &pre_cluster){
		for(int i=0;i<pre_cluster._pin_set.size();i++){
			pre_cluster._pin_set[i]->set_cluster_num(_id);
			_pin_set.push_back(pre_cluster._pin_set[i]);
		}
		pre_cluster._pin_set.clear();
	}
	int get_id(){ return _id; }
	vector<Pin*> _pin_set; 
private:
	int  _id;
};

inline bool operator<(const Pin& r,const Pin& l){
    return r._x < l._x;
}
inline bool operator<(const Net& r,const Net& l){
    return r._m_distance < l._m_distance;
}

//function--------------------
void Router::initial_tree(){
	sort(_pin.begin(),_pin.end());
	for(int i=0;i<_pin.size();i++){
		int a=i+20;
		if(_pin.size()>3000){ a=a+30; }
		for(int j=(i+1);j<a;j++){
			if(j>=_pin.size()){ break; }
			if(_pin[i].get_y()>_pin[j].get_y()){ _net.push_back(Net(_pin[i],_pin[j]));}
			else{ _net.push_back(Net(_pin[j],_pin[i])); }
		}
	}
	//according to m_distance sort
    sort(_net.begin(),_net.end());
}

void Router::kruskal(){
	total_distance=0;
	vector<Cluster> _cluster;
	//initial make-set
	for(int i=0;i<_pin.size();i++){
		Cluster initial_set(i);
		initial_set.add_pin(&_pin[i]);
		_pin[i].set_cluster_num(i);
		_cluster.push_back(initial_set);
	}
	//according to sorting result
	for(int i=0;i<_net.size();i++){
		Pin *p1=_net[i].get_pin_l();
		Pin *p2=_net[i].get_pin_r();
		if(p1->get_cluster_num()!=p2->get_cluster_num()){
			Cluster &p2_cluster=_cluster[p2->get_cluster_num()];
			_cluster[p1->get_cluster_num()].union_cluster(p2_cluster);
			rectilinear(p1,p2);
		}
	}
	cout<<"Final net= "<<Final.size()<<endl;
}
void Router::rectilinear(Pin *p1,Pin *p2){
	if(p1->_x==p2->_x){ 
		Final.push_back(F_net(p1->_x,p1->_y,p2->_x,p2->_y,true));
		total_distance=total_distance+abs(p1->_y-p2->_y);
	}
	else if(p1->_y==p2->_y){
		Final.push_back(F_net(p1->_x,p1->_y,p2->_x,p2->_y,false));
		total_distance=total_distance+abs(p1->_x-p2->_x);
	}
	else{
		Final.push_back(F_net(p1->_x,p1->_y,p1->_x,p2->_y,true));
		Final.push_back(F_net(p1->_x,p2->_y,p2->_x,p2->_y,false));
		total_distance=total_distance+abs(p1->_y-p2->_y)+abs(p1->_x-p2->_x);
	}
}

