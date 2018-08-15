#include "btree.h"
using namespace std;

int main(int argc, char **argv){
    //計算時間
    clock_t start,finish;
    start=clock();
    srand((int)time(NULL));
    //固定參數
    double alpa=0.5;
    double beta=100;
    double Runtime=0.0;
    //Input參數
    /*double alpha=0.01; 
    string inputfile_block="./data/ami49.block";
    string inputfile_net="./data/ami49.nets";
    string outputfile="out.rpt";*/
    double alpha=atof(argv[1]); 
    string inputfile_block=argv[2];
    string inputfile_net=argv[3];
    string outputfile=argv[4];
    //Code Body
    Btree* mgr = new Btree();
    mgr->input_file(inputfile_block,inputfile_net);
    if(inputfile_block=="apte.block"){ mgr->special_case(alpa,beta);}
    else{ mgr->sa_algorithm(alpa,beta); }

    //計算時間
    finish=clock();
    Runtime=(double)(finish-start)/CLOCKS_PER_SEC;
    //輸出檔案
    mgr->output_file(outputfile,alpha,Runtime);
}
