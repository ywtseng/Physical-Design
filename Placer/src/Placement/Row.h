#ifndef ROW_H
#define ROW_H

#include <vector>
#include "Module.h"

using namespace std;


class Row
{
public:
    enum Orient {OR_N, OR_W, OR_S, OR_E, OR_FN, OR_FW, OR_FS, OR_FE};

    Row(double x = -1, double y = -1, double height = -1, double siteSpacing = -1,
        unsigned numSites = -1, Orient orient = OR_N, bool isSymmetric = true)
        : _x(x), _y(y), _height(height), _siteSpacing(siteSpacing),
          _numSites(numSites), _orient(orient), _isSymmetric(isSymmetric)
    {}


    Row( double& set_bottom,  double& set_height,  double& set_step ) :
        _y( set_bottom ), _height( set_height ),
        _siteSpacing( set_step ), _orient( OR_N )
    {}

    ////////////////////////////////////////////
    static bool Lesser( const Row& r1, const Row& r2  )
    {
        return ( r1._y < r2._y );
    }
    static bool Greater( const Row& r1, const Row& r2  )
    {
        return ( r1._y > r2._y );
    }
    vector<double> m_interval;
    
    //增減module
    void add_row_module(Module *add_module){
        _row_module.push_back(add_module);
		total_row_width=(total_row_width+add_module->width());
    }
    void delete_row_module(){
		total_row_width=(total_row_width-_row_module.back()->width());
        _row_module.pop_back();
    }
    vector<Module*> get_module(){ return _row_module; }
    Module* get_module(int id){ return _row_module[id]; }
    int get_module_num(){ return _row_module.size(); }
	bool right_x(double width){
		double sum=0.0;
		if(_row_module.size()>0){
			if(total_row_width+width+_x > max_x() ){ return false; }
			else{ return true; }
		}
		return true;
		
	}
	
    
    /////////////////////////////////////////////
    // get
    /////////////////////////////////////////////
    double x() const {return _x;}
	double max_x() const {return (_x+_numSites*_siteSpacing);}
    double y() const {return _y;}
    double height() const {return _height;}
    double width() const {return _numSites*_siteSpacing;}
    double siteSpacing() const {return _siteSpacing;}
    double numSites() const {return _numSites;}
    void row_module_number() const { cout<<_row_module.size()<<endl; }
	

    /////////////////////////////////////////////
    // set
    /////////////////////////////////////////////
    void setPosition(double x, double y) {
        _x = x;
        _y = y;
    }
    void setHeight(double height) {
        _height = height;
    }
    void setSiteSpacing(double siteSpacing) {
        _siteSpacing = siteSpacing;
    }
    void setNumSites(unsigned numSites) {
        _numSites = numSites;
    }
    void setOrient(Orient orient) {
        _orient = orient;
    }
    void setIsSymmetric(bool isSymmetric) {
        _isSymmetric = isSymmetric;
    }
	//儲存module的地方
	vector<Module*>  _row_module;
	double total_row_width;
private:
    // variables from benchmark input
    double _x, _y; // low x and low y
    double _height; // height of row
    double _siteSpacing; // distance between the beginings of neighboring sites
    unsigned _numSites; // number of sites
    Orient _orient; // orient
    bool _isSymmetric; // symmetry
    
    
   
};

#endif // ROW_H
