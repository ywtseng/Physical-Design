#include "btree.h"
#include <iomanip>
#include <math.h>  
using namespace std;
void Btree::sa_algorithm(double &alpa,double &beta){
	//調整參數
	double 			T0=1000000;
	double 			Tth=1000;
	double 			k=1000;
	double 			r=0.98;
	//固定參數
	double          T=T0;
	double 			_reject=0;
	int    			_round=0;
	bool            _change=false;
	double          _cost1;
	double          _cost2;
	bool 			_out=false;
	bool			_in=false;

	//儲存最佳解 與 前一個解
	vector<Block> 	best_blocks;
	vector<Block> 	prev_blocks;
	int 			best_root_id;
	int 			prev_root_id;
	//開始進行SA
	//初始化 tree
	initial_tree();
	_best_solution=0;
	//儲存 最佳的解 和 root ID
	best_blocks=blocks;
	best_root_id=get_root_id();
	double _best_cost=count_cost(alpa,beta);
	_cost1=_best_cost;
	//儲存 最佳的解 和 root ID
	prev_blocks=blocks;
	prev_root_id=get_root_id();
	//進行SA迴圈
	do{
		_reject=0;
		_round++;
		for(int i=0;i<k;i++){
			_round++;
			if(_change==true){
				_change=false;
				prev_blocks=blocks;
				prev_root_id=get_root_id();
			}
			adjust_tree();
			_cost2=count_cost(alpa,beta);
			double _cost_diff=_cost2-_cost1;
			if(get_fit_outline()==true){  _out=true; }
			_in=_out;
			//判斷公式
			if(((_out && _in)||(!_out && !_in))&& (_cost_diff<=0||(double)rand()/RAND_MAX<pow(2.8,-_cost_diff/T)))
			{
                if(_cost1<_best_cost){ best_blocks=blocks; best_root_id=get_root_id(); }   
                _cost1=_cost2;
                _change=true;
            }
            else{ 
            	blocks=prev_blocks; set_root_id(prev_root_id);
               	_reject++;
                _change=false;
            }
            if(_change) prev_blocks.clear();
		}
		T = r * T;
		//調整公式
		if((T<Tth||_reject/k>0.95)&&!_out){
			k*=2; 
			T*=100; 
			beta*=10;
			_reject=0; 
		}
    }while(!(_reject/k>0.97 || T<Tth));
    cout<<"Round = "<<_round<<endl;
    cout<<"Fit in outline = "<<_best_solution<<endl;

   
    blocks=best_blocks;
    set_root_id(best_root_id);
    int max_X=0,max_Y=0;
    contour(max_X,max_Y);
    cout<<"(Outline_w,Outline_h)=("<<_outline_w<<","<<_outline_h<<")"<<endl;
    cout<<"(  Max_X  ,  Max_Y  )=("<<max_X<<","<<max_Y<<")"<<endl;
    //print_tree();
}

void Btree::output_file(const string& outputfile,double &alpha,double &runtime){
	fp.open(outputfile.c_str(),ios::out);
    int _max_X=0,_max_Y=0;
    contour(_max_X,_max_Y);
    double _hpwl=HPWL();
	double _cost=(alpha*_max_X*_max_Y+(1-alpha)*_hpwl);

    fp<<_cost<<endl;//cost=aA+(1-a)W
    fp<<_hpwl<<endl;//HPWL
    cout<<"HPWL: "<<_hpwl<<endl;
    fp<<_max_X*_max_Y<<endl;//chip area
    cout<<"Area: "<<_cost<<endl;

    fp<<_max_X<<"  "<<_max_Y<<endl;//chip width chip height
    fp<<runtime<<" s"<<endl;//Runtime

    fp<<endl;//<macro_name> <x1> <y1> <x2> <y2>
    for(int i=0;i<_block_num;i++){
    	fp<<setw(7)<<left<<blocks[i].get_name();
    	fp<<setw(6)<<blocks[i].get_x();
		fp<<setw(6)<<blocks[i].get_y();
		fp<<setw(6)<<blocks[i].get_x()+blocks[i].get_width();
		fp<<setw(6)<<blocks[i].get_y()+blocks[i].get_height();
    }
    fp.close();

    cout<<"Program Runtime: "<<runtime<< " sec."<<endl;

}
void Btree::special_case(double &alpa,double &beta){
	initial_tree();
	int _round=0;
	for(int i=0;i<10000000;i++){
		_round++;
		adjust_tree();
		count_cost(alpa,beta);
		if(get_fit_outline()==true){ break; }
	}
	int max_X=0,max_Y=0;
    contour(max_X,max_Y);
    cout<<"Round = "<<_round<<endl;
    cout<<"(Outline_w,Outline_h)=("<<_outline_w<<","<<_outline_h<<")"<<endl;
    cout<<"(  Max_X  ,  Max_Y  )=("<<max_X<<","<<max_Y<<")"<<endl;

}