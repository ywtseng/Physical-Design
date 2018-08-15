#include "btree.h"
#include <stack>
using namespace std;

class Node{
public:
    Node(int _ix,int _iy):_x(_ix),_y(_iy){}
    int get_x(){ return _x; }
    int get_y(){ return _y; }
private:
    int _x;
    int _y;
};

double Btree::HPWL(){
    double count=0.0;
    for(int i=0;i<_net_num;i++){
        int _min_x=10000;
        int _max_x=0;
        int _min_y=10000;
        int _max_y=0;
        for(int j=0;j<nets[i].get_degree();j++){
            Terminal *temp=nets[i].get_terminal(j);
            int _temp_x=0;
            int _temp_y=0;
            if(temp->get_type()==true){
                _temp_x=blocks[temp->get_id()].get_x()+(blocks[temp->get_id()].get_width()/2);
                _temp_y=blocks[temp->get_id()].get_y()+(blocks[temp->get_id()].get_height()/2);
            }
            else{
                _temp_x=temp->get_x();
                _temp_y=temp->get_y();
            }
            if(_temp_x < _min_x){ _min_x=_temp_x; }
            if(_temp_x > _max_x){ _max_x=_temp_x; }
            if(_temp_y < _min_y){ _min_y=_temp_y; }
            if(_temp_y > _max_y){ _max_y=_temp_y; }
        }
        count=count+(_max_x-_min_x+_max_y-_min_y);
    }
    return count;
}
double Btree::count_cost(double &alpa, double &beta){
    int max_X=0,max_Y=0;
    contour(max_X,max_Y);
    if(max_X<=_outline_w &&max_Y<=_outline_h){ 
        _best_solution++;
        _fit_outline=true;
    }
    else{ _fit_outline=false; }
    double _penalty=0;
    if(max_X>_outline_w){ _penalty += (max_X-_outline_w)*(max_X-_outline_w); }
    if(max_Y>_outline_h){ _penalty += (max_Y-_outline_h)*(max_Y-_outline_h); }
    //return (alpa*max_X*max_Y)+(1-alpa)*HPWL()*100+beta*_penalty;
    return (alpa*max_X*max_Y)+beta*_penalty;
}
void Btree::contour(int &max_X,int& max_Y){
    //建一個stack
    list<Node>             contours;
    list<Node>::iterator   at;
    stack<Block*>          mystack;
    mystack.push(&blocks[_root_id]);
    contours.push_back(Node(100000,0));
    int count=0;
    while(!mystack.empty()){
        //cout<<"Round  "<<count<<"--------------"<<endl;
        Block* thisBlock=mystack.top();
        mystack.pop();
        if(thisBlock->get_id_r()!=(-1)){ mystack.push(&blocks[thisBlock->get_id_r()]); }
        if(thisBlock->get_id_l()!=(-1)){ mystack.push(&blocks[thisBlock->get_id_l()]); }
        //cout<<"ThisBlock name = "<<thisBlock->get_name()<<endl;
        //判斷thisBlock是否為root
        if(thisBlock->get_id_p()==(-1)){
            //設定此block的位置
            thisBlock->set_x(0);
            thisBlock->set_y(0);
            //設定contour
            contours.push_front(Node(0,thisBlock->get_height()));
            thisBlock->bt=contours.begin();
        }
        else{
            //thisBlock為左子點or右子點-------------
            Block *this_parent=&blocks[thisBlock->get_id_p()];
            list<Node>::iterator ct=this_parent->bt;
            list<Node>::iterator dt=this_parent->bt;
            int x1,x2,x3,y1,y2,y3;
           
            if(this_parent->get_id_l()==thisBlock->get_id()){
                //設定Ｘ值的範圍
                x1=this_parent->get_x()+this_parent->get_width();
                x2=x1+thisBlock->get_width();
                //cout<<"Range = ("<<x1<<","<<x2<<")"<<endl;
                //設定Ｙ值
                y1=this_parent->get_y();
                for(ct++;ct->get_x()<=x2;ct++){
                    if(y1<ct->get_y()){ y1=ct->get_y(); }
                }
                y2=y1+thisBlock->get_height();

                //設定此block的位置
                //cout<<"Block position =("<<x1<<","<<y1<<")"<<endl;
                thisBlock->set_x(x1);
                thisBlock->set_y(y1);
                if(x2>max_X){ max_X=x2; }
                if(y2>max_Y){ max_Y=y2; }
                //設定contour
                x3=x1;
                y3=y1+thisBlock->get_height();
                dt++;
                contours.insert(dt,Node(x3,y3));
                dt--;
                thisBlock->bt=dt;
                //cout<<"ThisBlock->bt=("<<thisBlock->bt->get_x()<<","<<thisBlock->bt->get_y()<<")"<<endl;
            }
            else{
                //設定Ｘ值的範圍
                x1=this_parent->get_x();
                x2=x1+thisBlock->get_width();
                //cout<<"Range = ("<<x1<<","<<x2<<")"<<endl;
                //設定Ｙ值
                y1=this_parent->get_y()+this_parent->get_height();
                for(ct++;ct->get_x()<x2;ct++){
                    if(y1<ct->get_y()){ y1=ct->get_y(); }
                }
                y2=y1+thisBlock->get_height();
                //設定此block的位置
                //cout<<"Block position =("<<x1<<","<<y1<<")"<<endl;
                thisBlock->set_x(x1);
                thisBlock->set_y(y1);
                if(x2>max_X){ max_X=x2; }
                if(y2>max_Y){ max_Y=y2; }
                //設定contour
                x3=x1;
                y3=y1+thisBlock->get_height();
                dt++;
                contours.insert(dt,Node(x3,y3));
                contours.erase(this_parent->bt);
                dt--;
                thisBlock->bt=dt;
                //cout<<"ThisBlock->bt=("<<thisBlock->bt->get_x()<<","<<thisBlock->bt->get_y()<<")"<<endl;
            }
        }
        count++;
    }
}