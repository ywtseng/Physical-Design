#include "Legal.h"
#include "arghandler.h"
#include "GnuplotLivePlotter.h"
#include "GnuplotMatrixPlotter.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <cassert>
#include <cfloat>
using namespace std;
//Abcus function
class Cluster
{
public:
    Cluster(){
        _cluster_cell.clear();
        first=last;
        cur_xc=0.0;
        total_weight=0.0;
        total_width=0.0;
        total_qc=0.0;
    }
    //set
    void set_first(Module* _first){ first=_first; }
	void add_cell(Module* _module){
        _cluster_cell.push_back(_module);
        last=_module;
		//numPin change to module area
        total_weight=(double)_module->numPins()+total_weight;
        total_qc=(double)_module->numPins()*(_module->x()-total_width)+total_qc;
        total_width=_module->width()+total_width;
    }
	void merge_cluster( vector<Module*> &add_cluster ){
		for(unsigned i=0;i<add_cluster.size();i++){
			//add_cell(add_cluster[i]);
			_cluster_cell.push_back(add_cluster[i]);
		}
		last=_cluster_cell.back();
	}
	void set_last(Module* _last){ last=_last; }
    void set_cur_xc(double xc){ cur_xc=xc; }
	void set_total_weight(double _weight){ total_weight=_weight; }
    void set_total_width(double _width){ total_width=_width; }
	void set_total_qc(double _qc){ total_qc=_qc; }
	
    //set_xc
    void initial_xc(double xc){ cur_xc=xc; }
    void calculate_cur_xc(){
		//if( fmod(total_qc, total_weight)==0 ){ cur_xc = total_qc/total_weight; }
		//if( (total_qc%total_weight)==0 ){ cur_xc = total_qc/total_weight; }
		//else{ cur_xc=(int)(total_qc/total_weight)+1; }
		cur_xc = total_qc/total_weight;
	}
    //set_we,wi,qc
    void set_value(double we,double wi,double qc){
        total_weight=we;
        total_width=wi;
        total_qc=qc;
    }
    //get
    double get_cur_xc(){ return cur_xc; }
    double get_total_weight(){ return total_weight; }
    double get_total_width(){ return total_width; }
    double get_total_qc(){ return total_qc; }
	int get_cell_number(){ return _cluster_cell.size(); }
    Module* get_last_module(){ return last; }
    vector<Module*> _cluster_cell;
   
private:
    Module* first;
    Module* last;
    double cur_xc;//xc(c)
    double total_weight;//ec(c)
    double total_width;//wc(c)
    double total_qc;//qc(c)
    
};

vector<Cluster> _cluster;
void addcluster(Cluster &pre_cluster,Cluster &cur_cluster){
	pre_cluster.merge_cluster( cur_cluster._cluster_cell );
	pre_cluster.set_total_weight( pre_cluster.get_total_weight()+cur_cluster.get_total_weight() );
	pre_cluster.set_total_qc( pre_cluster.get_total_qc() + cur_cluster.get_total_qc() - 
		cur_cluster.get_total_weight()*pre_cluster.get_total_width() );
	pre_cluster.set_total_width( pre_cluster.get_total_width()+cur_cluster.get_total_width() );
    
}
void collapse(Cluster &cur_cluster,double &max_x,double &min_x){
	cur_cluster.calculate_cur_xc();
	double xc=cur_cluster.get_cur_xc();
	double check_2 = max_x - cur_cluster.get_total_width();
    if( xc < min_x ){ xc=min_x; }
    else if( xc > check_2 ){
		xc = check_2; 
	}
    cur_cluster.set_cur_xc(xc);
	if(_cluster.size()>1){
		Cluster &pre_cluster=_cluster[_cluster.size()-2];
		if( pre_cluster.get_cur_xc()+pre_cluster.get_total_width()>xc){
			addcluster(pre_cluster,cur_cluster);
			_cluster.pop_back();
			collapse(pre_cluster,max_x,min_x);		
		}
	}
}
void CLegal::legalization()
{
    //Row的y軸 從-33208、-32704、-32200.......32312、32816 （一行504)
    for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++)
    {
        Module &curModule = _placement.module(moduleId);
        double x = curModule.x();
        double y = curModule.y();
        double best_cost=DBL_MAX;
        int best_row_id=100000;
        int rowId=(y-_placement.boundaryBottom())/(_placement.getRowHeight());
		for(int i=0;i<_placement.numRows();i++){
            if(i<0 || i>(_placement.numRows()-1) ){ continue; }
            Row &curRow = _placement.row(i);
            //-------------------------------------
			if(curRow.right_x(curModule.width()) ){
				double cost=0.0;
				curRow.add_row_module(&curModule);
				place_row(curModule,curRow,false);
				cost=fabs(x-curModule.get_temp_x())+fabs(y-curModule.get_temp_y());
				//cout<<"i= "<<i<<"->"<<rowId<<"->"<<curRow.y()<<"="<<cost<<endl;
				
				if(cost<best_cost){
					best_cost=cost;
					best_row_id=rowId;
				}
				curRow.delete_row_module();
			}
			else continue;
	    }
      
        Row &bestRow=_placement.row(best_row_id);
        bestRow.add_row_module(&curModule);
        place_row(curModule,bestRow,true);

        m_bestLocations[moduleId] = CPoint( curModule.x(), curModule.y() );
    }
}
void CLegal::place_row(Module &curModule,Row &curRow,bool final){
    
    //cout<<"Row Module number=  "<<curRow.get_module_num()<<endl;
    _cluster.clear();
	double row_x_max=curRow.max_x();
	double row_x_min=curRow.x();
    //-----------------
    
    Cluster new_cluster;
    new_cluster.set_value(0,0,0);
    Module* module_left=curRow.get_module(0);

    new_cluster.initial_xc(module_left->x());
    new_cluster.set_first(module_left);
    new_cluster.add_cell(module_left);

    _cluster.push_back(new_cluster);
    //---------------------------
    for(int i=1 ; i<curRow.get_module_num() ; i++){
        Module* module_cur=curRow.get_module(i);
        Cluster& _last_cluster=_cluster.back();
        if( _last_cluster.get_cur_xc()+_last_cluster.get_total_width() > module_cur->x() ){
  
            _last_cluster.add_cell(module_cur);
            collapse(_last_cluster,row_x_max,row_x_min);
        }
        else{
            Cluster renew_cluster;
            renew_cluster.set_value(0,0,0);
            renew_cluster.initial_xc(module_cur->x());
            renew_cluster.set_first(module_cur);
            renew_cluster.add_cell(module_cur);
            
            _cluster.push_back(renew_cluster);
        }
    }
   	//正式放入
	if(final==true){
		for(int i=0;i<_cluster.size();i++){
			Cluster& cur_cluster=_cluster[i];
			cur_cluster._cluster_cell[0]->setPosition(( cur_cluster.get_cur_xc() ),curRow.y());
            //cout<<"("<<cur_cluster._cluster_cell[0]->x()<<","<<cur_cluster._cluster_cell[0]->y()<<")"<<endl;

			for(int i=1;i<cur_cluster._cluster_cell.size();i++){
				cur_cluster._cluster_cell[i]->setPosition(
					( cur_cluster._cluster_cell[i-1]->x()+cur_cluster._cluster_cell[i-1]->width() ),
                    curRow.y());
                //cout<<"("<<cur_cluster._cluster_cell[i]->x()<<","<<cur_cluster._cluster_cell[i]->y()<<")"<<endl;
			}
		}
	}
	//假設放入
	else{
		for(int i=0;i<_cluster.size();i++){
			Cluster& cur_cluster=_cluster[i];
			cur_cluster._cluster_cell[0]->set_temp_Position(cur_cluster.get_cur_xc(),curRow.y());
			for(int i=1;i<cur_cluster._cluster_cell.size();i++){
				cur_cluster._cluster_cell[i]->set_temp_Position(
					cur_cluster._cluster_cell[i-1]->get_temp_x()+cur_cluster._cluster_cell[i-1]->width(),curRow.y() );
			}
		}
	}
}
//global function------------------------------------------------------
inline bool operator<(const Module& a,const Module& b)
{
    return a._x < b._x;
}

bool sortModule( Module* a, Module* b)
{
    return a->x() < b->x();
}
//CLegal function---------------------------------
bool CLegal::solve()
{
    // TODO: edit your code HERE
    // Note:
    //      1. You should save your legal solution into m_bestLocations, and call setLegalResult() tp upload it into Placement.
    //      2. Run check() to make sure that the solution is legal.
    //      3. Feel free to add any class, function, and/or data member.
    // Good luck!
    sort(_placement._modules.begin(),_placement._modules.end());
    saveGlobalResult();
	Greedy();
    setLegalResult();
    if( check() ){
        cout<< "total displacement: " << totalDisplacement() << endl;
    }
    else{ return false; }
    
    return true;
}
CLegal::CLegal( Placement& placement  ) :
    _placement( placement )
{

    //Compute average cell width
    int cell_count = 0;
    double total_width = 0;
    //double max_height = 0.0;
    m_max_module_height = 0.0;
    m_max_module_width = 0.0;
    for( unsigned  moduleId = 0 ; moduleId < placement.numModules() ; moduleId++ )
    {
        Module& curModule = placement.module(moduleId);

        m_max_module_height = max( m_max_module_height, curModule.height() );
        m_max_module_width = max( m_max_module_width, curModule.width() );
    //Do not include fixed cells and macros
        if( curModule.isFixed() || curModule.height() > placement.getRowHeight() )
        continue;

        cell_count++;
        total_width += curModule.width();
    }

    m_average_cell_width = total_width / cell_count;

    m_free_sites = placement.m_sites;
    m_site_bottom = m_free_sites.front().y();
    m_site_height = m_free_sites.front().height();

    //initalize m_origLocations and m_bestLocations
    m_bestLocations.resize( placement.numModules() );
    m_globalLocations.resize( placement.numModules() );
    m_chip_left_bound = placement.rectangleChip().left();
}

void CLegal::saveGlobalResult()
{
    for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++)
    {
        Module &curModule = _placement.module(moduleId);
        double x = curModule.x();
        double y = curModule.y();
        m_globalLocations[moduleId] = CPoint( x, y );
        m_bestLocations[moduleId] = CPoint( x, y );
    }
}
void CLegal::Greedy(){
	for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++){
		Module &curModule = _placement.module(moduleId);		
		for(int i=0;i<_placement.numRows();i++){
			Row &curRow = _placement.row(i);
			if(curRow.right_x(curModule.width())){
				curRow.add_row_module(&curModule);
				if(curRow._row_module.size()>1){
					Module *pre=curRow._row_module[curRow._row_module.size()-2];
					curModule.setPosition(pre->x()+pre->width(),curRow.y());
					m_bestLocations[moduleId] =CPoint(pre->x()+pre->width(),curRow.y());
				}
				else{
					curModule.setPosition(curRow.x(),curRow.y());
					m_bestLocations[moduleId] =CPoint(curRow.x(),curRow.y());
				}
				break;
			}
		}
	}
}



void CLegal::setLegalResult()
{
    for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++)
    {
        Module &curModule = _placement.module(moduleId);
        curModule.setPosition(m_bestLocations[moduleId].x,m_bestLocations[moduleId].y);
    }
}


bool CLegal::check()
{
    cout << "start check" << endl;
    int notInSite=0;
    int notInRow=0;
    int overLap=0;
    
    ///////////////////////////////////////////////////////
    //1.check all standard cell are on row and in the core region
    //////////////////////////////////////////////////////////
    for(unsigned int i=0; i<_placement.numModules(); ++i)
    {
        Module& module = _placement.module(i);
        if(module.isFixed()) continue;
        double curX = module.x();
        double curY = module.y();
        
        double res = ( curY - m_site_bottom ) / _placement.getRowHeight();
        //cout << curY << " " << res << endl;
        int ires = (int) res;
        if( (m_site_bottom + _placement.getRowHeight() * ires) != curY )
        {
            cerr<<"\nWarning: cell:"<<i<<" is not on row!!";
            ++notInRow;
        }
        if( (curY<_placement.boundaryBottom()) || (curX<_placement.boundaryLeft())||
           ( (curX+module.width())>_placement.boundaryRight()) ||
           ( (curY+module.height())>_placement.boundaryTop()) )
        {
            cerr<<"\nWarning: cell:"<<i<<" is not in the core!!";
            ++notInSite;
        }
    }
    
    ///////////////////////////////////////////
    //2. row-based overlapping checking
    ///////////////////////////////////////////
    
    Rectangle chip = _placement.rectangleChip();
    const double &rowHeight = _placement.getRowHeight();
    unsigned numRows = _placement.numRows();
    vector< vector<Module*> > rowModules( numRows, vector<Module*>( 0 ) );
    for(unsigned int i=0; i<_placement.numModules(); ++i)
    {
        Module& module = _placement.module(i);
        double curY = m_bestLocations[i].y;
        
        if( module.area() == 0 ) continue;
        if( module.isFixed() ) continue;
        
        double yLow = curY - chip.bottom(), yHigh= curY + module.height() - chip.bottom();
        size_t low = floor( yLow / rowHeight ), high = floor(yHigh / rowHeight);
        if( fabs( yHigh - rowHeight * floor(yHigh / rowHeight) ) < 0.01 ){ --high; }
        
        for( size_t i = low; i <= high; ++i ){ rowModules[ i ].push_back( &module ); }
    }
    for( size_t i = 0; i < numRows; ++i )
    {
        vector<Module*> &modules = rowModules[i];
        sort(modules.begin(), modules.end(), sortModule);
        if( modules.size() < 1 ) { continue; }
        for( size_t j = 0; j < modules.size() - 1; ++j ){
            Module &mod = *modules[ j ];
            if(mod.isFixed()) continue;
            size_t nextId = j+1;
            while( mod.x() + mod.width() - modules[ nextId ]->x() > 0.01 ){
                Module &modNext = *modules[ nextId ];
                if( mod.x() + mod.width() - modules[ nextId ]->x() > 0.01 ){
                    ++overLap;
                    cout << mod.name() << " overlap with " << modNext.name() << endl;
                }
                ++nextId; if( nextId == modules.size() ) { break; }
            }
        }
    }
    
    cout << endl <<
    "  # row error: "<<notInRow<<
    "\n  # site error: "<<notInSite<<
    "\n  # overlap error: "<<overLap<< endl;
    //cout << "end of check" << endl;
    
    if( notInRow!=0 || notInSite!=0 || overLap!=0 )
    {
        cout <<"Check failed!!" << endl;
        return false;
    }
    else
    {
        cout <<"Check success!!" << endl;
        return true;
    }
}
double CLegal::totalDisplacement()
{
    double totaldis = 0;
    for( unsigned  moduleId = 0 ; moduleId < _placement.numModules() ; moduleId++ )
    {
        Module& curModule = _placement.module(moduleId);
        double x = curModule.x();
        double y = curModule.y();

        totaldis += CPoint::Distance(m_globalLocations[moduleId] , CPoint( x, y ));
    }
    return totaldis;
}
