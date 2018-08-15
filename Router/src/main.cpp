#include "router.h"

using namespace std;
int main(int argc,char **argv){
	//計算時間
    clock_t start,finish;
    start=clock();
    srand((int)time(NULL));
    //string inputfile="./arearouting/testbench/case2";
    string inputfile=argv[1];
    string outputfile=argv[2];
    Router* mgr = new Router();
    //parser.cpp
    mgr->parser(inputfile);
    //kruskal.cpp
    mgr->initial_tree();
    mgr->kruskal();
    //output.cpp
    mgr->output_file(outputfile);
    finish=clock();
    cout<<"Runtime= "<<(double)(finish-start)/CLOCKS_PER_SEC<<endl;

}
