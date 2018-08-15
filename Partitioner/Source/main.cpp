#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <iomanip> 
#include <stdlib.h>     /* atoi */


using namespace std;

class Cell
{
public:
	Cell(unsigned _iname):_name(_iname){
		_net_id.clear();
		_side="NULL";	
		_gain=0;
		_locked=false;
	}
	//~Cell();
	//data member
	vector<unsigned> 			_net_id;	
	string						_side;
	int  						_gain;
	unsigned 					_name;
	bool 						_locked;
};
class Partition
{
public:
	Partition(){}
	void Input_file(string &);
	void Algorithm();
		void Initial_partition();
		void Initial_bucket();
		void Check_cell(int &);
			bool Check_balance(int &,int &);
		void Update_cell_gain();
			void Move_update(Cell &base_cell);
	void Output_file(string&);
private:
	//data member
	int 					_current_left_num=0;
	int 					_current_right_num=0;
	int 					_iteration=0;
	int 					_best_solution=0;
	int    					_best_iteration=0;
	//收集最初的數據
	vector<Cell> 			_pre_cells; //收集最初的_cell
	int 					_pre_left_num=0;
	int 					_pre_right_num=0;
	int          			_wide=0;
	float					_balance_factor;
	unsigned 	 			_num_cut;
	//記錄變化值
	int 					_net_num=0;
	vector<int>				_change_cell; //放每個步驟的change_cell
	vector<int>				_change_sum_gain;//放每個步驟的累績的_gain
	//讀檔及輸出
	fstream 				fp;
};
class Net
{
public:
	Net(unsigned _iname,vector<unsigned> &_ics_id):_name(_iname),_cell_id(_ics_id)
	{ _l_num=0; _r_num=0; }
	//~Net();
	//Data member
 	vector<unsigned>  				_cell_id;
	unsigned 						_name;
	unsigned						_l_num;
	unsigned 						_r_num;
};
vector<Net> 				_nets; //收集所有的net
vector<Cell> 				_cells; //收集所有的cell
vector< list<unsigned> >	_bucket; //建一個bucket list 
vector<Net>::iterator		nt;
vector<Cell>::iterator		ct;
vector<unsigned>::iterator  cit;
vector<unsigned>::iterator  nit;

void Partition::Input_file(string &inputfile){
	string				_temp;
	string				_temp_a;
	unsigned		    _cell_id;
	int 				_max_cell_id=0;
	vector<unsigned>	temp_cells_id;
	fp.open(inputfile.c_str(),ios::in);
	//balance factor-------
	fp>>_temp;
	_balance_factor=atof(_temp.c_str());
	cout<<"balance factor= "<<_balance_factor<<endl;
	//---------------------
	while(fp.eof()!=true){
		fp>>_temp;//讀入Net
		if(_temp=="NET"){
			fp>>_temp;//讀入n1
			temp_cells_id.clear();
			_net_num++;
			//-----------------------
			fp>>_temp;//讀入c1
			_temp.erase(0,1);
			_cell_id=atoi(_temp.c_str());
			temp_cells_id.push_back(_cell_id);
			if(_max_cell_id<_cell_id) _max_cell_id=_cell_id;
			//-----------------------
			while(_temp!=";"){
				fp>>_temp;//讀入c2 c5 ;
				if(_temp[0]=='c'){
					_temp.erase(0,1);
					_cell_id=atoi(_temp.c_str());
					temp_cells_id.push_back(_cell_id);
					if(_max_cell_id<_cell_id) _max_cell_id=_cell_id;
				}
			}
			//放入Net
			Net temp_net( _net_num, temp_cells_id );
			_nets.push_back(temp_net);
		}
	}
	for(int i=0;i<_max_cell_id;i++){
		Cell _temp_cell(i+1);
		//全部的cell name皆需要＋1  放在cell[0]的是name 1
		_cells.push_back(_temp_cell);
	}
	//將所有的cell的netlist 皆填上
	for(int i=0;i<_nets.size();i++){
		for(cit=_nets[i]._cell_id.begin();cit!=_nets[i]._cell_id.end();cit++){
			_cells[*cit-1]._net_id.push_back(i);
			if(_wide<_cells[*cit-1]._net_id.size()){ 
				_wide=_cells[*cit-1]._net_id.size(); 
			}
		}
	}
	fp.close();
	cout<<"Net number= "<<_nets.size()<<endl;
	cout<<"Cell number= "<<_cells.size()<<endl;
	cout<<"Pin number= "<<_wide<<endl;
	/*for(int i=0;i<_nets.size();i++){
		cout<<"N"<<(i+1)<<"  "<<_nets[i]._cell_id_number()<<endl;
	}
	for(int i=0;i<_cells.size();i++){
		cout<<"C"<<i+1<<"  "<<_cells[i]._ne_id_number()<<endl;
	}*/
}
void Partition::Algorithm(){
	Initial_partition();
	Initial_bucket();
	_iteration=0;
	//Step 1--------------
	Check_cell(_iteration);
	/*for(int i=0;i<_cells.size();i++){
		cout<<_cells[i]._name<<"  Side="<<_cells[i]._side<<endl;
	}*/
	//bool _best_solution=false;
	for(int i=1;i<_cells.size();i++){
		Update_cell_gain();
		_iteration++;
		Check_cell(_iteration);
	}
	Update_cell_gain();
	cout<<"====FM Result===="<<endl;
	cout<<"Best_iteration = "<<_best_iteration<<endl;
	cout<<"Number of cut = "<<(_num_cut-_best_solution)<<endl;
	
	/*for(int i=0;i<_change_sum_gain.size();i++){
		cout<<(i+1)<<" =  "<<_change_sum_gain[i]<<endl;
	}*/
	//輸出結果
	/*for(int i=0;i<_pre_cells.size();i++){
		cout<<_pre_cells[i]._name<<"  = "<<_pre_cells[i]._side<<endl;
	}*/
	/*for(int i=0;i<_bucket.size();i++){
			cout<<(4-i)<<"  "<<_bucket[i].size()<<endl;
			if(_bucket[i].size()!=0){
				list<unsigned>::iterator gt;
				for(gt=_bucket[i].begin();gt!=_bucket[i].end();gt++){
					cout<<"G"<<*gt+1<<"  ";
				}
				cout<<endl;
			}
	}*/
}
void Partition::Initial_partition(){
	//將cell的_side和_gain值初始化
    for(int i=0; i<_cells.size();i++){
    	if(i<(_cells.size())/2){
	        _cells[i]._side="left";
	    	_current_left_num++;
	    }
	    else{
	    	_cells[i]._side="right";
    		_current_right_num++;
	    }
	    _cells[i]._gain=0;
    }
    _pre_cells=_cells;
    _pre_left_num=_current_left_num;
    _pre_right_num=_current_right_num;
    //先將_net初始化
	for(nt=_nets.begin();nt!=_nets.end();nt++){
		for(cit=nt->_cell_id.begin();cit!=nt->_cell_id.end();cit++){
			if(_cells[*cit-1]._side=="left") {  ++nt->_l_num; }
			if(_cells[*cit-1]._side=="right"){  ++nt->_r_num; }
		}
	}
	/*----Test-------
	for(nt=_nets.begin();nt!=_nets.end();nt++){
		cout<<"N"<<nt->_name<<": "<<nt->_l_num<<" , "<<nt->_r_num<<"  ||";
		for(cit=nt->_cell_id.begin();cit!=nt->_cell_id.end();cit++){
			cout<<"C"<<*cit<<"="<<_cells[*cit-1]._side<<"   ";
		}
		cout<<endl;
	}*/
	//計算cell的gain值
	for(nt=_nets.begin();nt!=_nets.end();nt++){
		for(cit=nt->_cell_id.begin();cit!=nt->_cell_id.end();cit++){
			if(_cells[*cit-1]._side=="left"){
				if(nt->_l_num==1){	++_cells[*cit-1]._gain; }
				else if(nt->_r_num==0){	--_cells[*cit-1]._gain; }
			}
			if(_cells[*cit-1]._side=="right"){
				if(nt->_r_num==1){	++_cells[*cit-1]._gain; }
				else if(nt->_l_num==0){	--_cells[*cit-1]._gain; }
			}
		}
	}
	//初始化cut---------
	_num_cut=0;
	for(nt=_nets.begin();nt!=_nets.end();++nt){
		if(nt->_l_num>0 && nt->_r_num>0){ _num_cut++; }
	}
	cout<<"Number of cut = "<<_num_cut<<endl;
}

void Partition::Initial_bucket(){
	//先初始化 _bucket---------------
	int _bucket_size=(2*_wide+1);
	_bucket.reserve(_bucket_size);
	for(int i=0;i<_bucket_size;i++){
		list<unsigned>    _temp_list;
		_bucket.push_back(_temp_list);
	}
	for(int i=0;i<_cells.size();i++){
		_bucket[ _wide-_cells[i]._gain ].insert(_bucket[ _wide-_cells[i]._gain ].begin(),i);
	}
}
void Partition::Check_cell(int &_iteration){
	//cout<<"Step"<<(_iteration+1)<<endl;
	bool _feasible=false;
	for(int i=0;i<_bucket.size();i++){
		_feasible=Check_balance(i,_iteration);
		if(_feasible==true) break;
		else	continue;
	}
	//是否符合平衡
	if(_feasible==true){
		//cout<<"F: max_gain="<<_change_sum_gain[_iteration]<<endl;
		if(_change_sum_gain.back()>_best_solution){
			_best_solution=_change_sum_gain.back();
			_best_iteration=(_iteration+1);
		}
		//cout<<"l_num="<<_current_left_num<<", r_num="<<_current_right_num<<endl;
	}
}
bool Partition::Check_balance(int &_id,int &_iteration){
	float _lower=(1-_balance_factor)/2*(_cells.size());
	float _upper=(1+_balance_factor)/2*(_cells.size());
	list<unsigned>::iterator 	bt;
	for(bt=_bucket[_id].begin();bt!=_bucket[_id].end();bt++){
		int _temp_current_left_num=_current_left_num;
		int _temp_current_right_num=_current_right_num;
		if(_cells[*bt]._side=="left"){
			_temp_current_left_num--;
			_temp_current_right_num++;	
		}
		if(_cells[*bt]._side=="right"){
			_temp_current_left_num++;
			_temp_current_right_num--;
		}
		if( _lower<_temp_current_left_num && _lower<_temp_current_right_num
			&& _temp_current_left_num<_upper && _temp_current_right_num<_upper){
			_current_left_num=_temp_current_left_num;
			_current_right_num=_temp_current_right_num;
			//cout<<"Balance!!!!!"<<endl;
			/*for(cit=_cells[*bt+1]._net_id.begin();cit!=_cells[*bt+1]._net_id.end();cit++){
				cout<<"N"<<*cit<<"  ";
				if(_nets[*cit]._l_num>0 && _nets[*cit]._r_num>0){
					_num_cut--; 
				}
			}*/
			if(_change_sum_gain.empty()){ _change_sum_gain.push_back(_wide-_id); }
			else{ _change_sum_gain.push_back( _change_sum_gain[_iteration-1]+(_wide-_id) );}
			_change_cell.push_back(_cells[*bt]._name);
			_cells[*bt]._locked=true;
			return true;
		}
	}
	return false;
}
void Partition::Update_cell_gain(){
	Cell &base_cell=_cells[_change_cell.back()-1];
	//刪除base_cell 從_bucket
	_bucket[_wide-base_cell._gain].remove(base_cell._name-1);
	Move_update(base_cell);
    if(base_cell._side=="left")
		base_cell._side="right"; 
    else{ base_cell._side="left"; }
}
void Partition::Move_update(Cell &base_cell){
	//cout<<"Before  move"<<endl;
	for(cit=base_cell._net_id.begin();cit!=base_cell._net_id.end();cit++){
		//base_cell==LEFT
		if(base_cell._side=="left"){
			//當T(n)=0  Free cell +1
			if(_nets[*cit]._r_num==0){
				//cout<<"Case 1"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain++;//Free cell +1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			//當T(n)=1.  只有[T cell]-1
			else if(_nets[*cit]._r_num==1){
				//cout<<"Case 2"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._side=="right" && _cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain--;//只有[T cell]-1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			else{}
		}
		//base_cell==Right
		else{
			//當T(n)=0 Free cell +1
			if(_nets[*cit]._l_num==0){
				//cout<<"Case 3"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain++;//Free cell +1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			//當T(n)=1.  只有[T cell]-1
			else if(_nets[*cit]._l_num==1){
				//cout<<"Case 4"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._side=="left" && _cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain--;//只有[T cell]-1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
						//list<unsigned> ::iterator dt=_bucket[_temp_gain].begin();
						//cout<<_temp_gain<<":"<<_bucket[_temp_gain].size()<<endl;
					}
				}
			}
			else{}
		}
		//Change Cell
		if(base_cell._side=="left") { 
			++_nets[ *cit ]._r_num; 
			--_nets[ *cit ]._l_num;
		}
    	else if(base_cell._side=="right"){ 
    		++_nets[ *cit ]._l_num; 
    		--_nets[ *cit ]._r_num; 
    	}
    	//cout<<"After  move"<<endl;
		//base_cell==LEFT
		if(base_cell._side=="left"){
			//當F(n)=0 Free cell -1
			if(_nets[*cit]._l_num==0){
				//cout<<"Case 5"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain--; //Free cell -1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			//當F(n)=1.  只有[F cell]+1
			else if(_nets[*cit]._l_num==1){
				//cout<<"Case 6"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._side=="left" && _cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain++; //只有[F cell]+1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			else{}
		}
		//base_cell==Right
		else{
			//當F(n)=0 Free cell -1
			if(_nets[*cit]._r_num==0){
				//cout<<"Case 7"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain--;//Free cell -1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			//當F(n)=1.  只有[F cell]+1
			else if(_nets[*cit]._r_num==1){
				//cout<<"Case 8"<<endl;
				for(nit=_nets[*cit]._cell_id.begin();nit!=_nets[*cit]._cell_id.end();nit++){
					if(_cells[*nit-1]._side=="right" && _cells[*nit-1]._locked==false){
						int _temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].remove(*nit-1);
						_cells[*nit-1]._gain++;//只有[F cell]+1
						_temp_gain=_wide-_cells[*nit-1]._gain;
						_bucket[_temp_gain].push_back(*nit-1);
					}
				}
			}
			else{}
		}
	} 
}
void Partition::Output_file(string &outputfile){
	for(int i=0;i<_best_iteration;i++){
		int _change_id=_change_cell[i];
		if(_pre_cells[_change_id-1]._side=="right"){  
			_pre_cells[_change_id-1]._side="left"; 
			_pre_left_num++;
			_pre_right_num--;
		}
		else{ 
			_pre_cells[_change_id-1]._side="right";
			_pre_left_num--;
			_pre_right_num++; 
		}
		_pre_cells[_change_id-1]._locked=true;
	}
	vector<unsigned>  _temp_right;
	fp.open(outputfile.c_str() ,ios::out);
	fp<<"Cutsize = "<<(_num_cut-_best_solution)<<endl;
	fp<<"G1 "<<setw(4)<<_pre_left_num<<endl;
	for(int i=0;i<_pre_cells.size();i++){
		if(_pre_cells[i]._side=="left"){ fp<<"c"<<_pre_cells[i]._name<<" "; }
		else{ _temp_right.push_back(_pre_cells[i]._name); }
	}
	fp<<";"<<endl;
	fp<<"G2	"<<setw(4)<<_pre_right_num<<endl;
	for(int i=0;i<_temp_right.size();i++){
		fp<<"c"<<_temp_right[i]<<" ";
	}
	fp<<";"<<endl;
    fp.close(); 
}

int main(int argc, char **argv){
	//測試時間
	clock_t start,finish;
    start=clock();
	srand((int)time(NULL));
	//Code Body--------
	string inputfile=argv[1];
	string outputfile=argv[2];
    Partition p;
    p.Input_file(inputfile);
    p.Algorithm();
    p.Output_file(outputfile);
	//-----------------
    finish=clock();
    cout<<"Program Runtime: "<<(double)(finish-start)/CLOCKS_PER_SEC << " S"<<endl;
}