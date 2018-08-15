1. 學號：R05943092
2. 姓名：曾育為
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: <tar cvfz  R05943092_PA3.tar.gz R05943092_PA3>
6. 各檔案說明：
	R05943092_PA3/src    		             :程式原始碼
	R05943092_PA3/src/Placement              :自己寫的部分
	R05943092_PA3/src/Legalizer    			 :自己寫的部分
	R05943092_PA3/R05943092_PA3_report.doc   :程式報告
	R05943092_PA3/readme.txt
7. 編譯方式說明：
   R05943092_PA3/Src/cmake .		
   R05943092_PA3/Src/make
8. 執行、使用說明：
   執行檔的命令格式為：./legalizer -aux <inputFile.aux> 
   EX: ./legalizer -aux ibm01-cu85.aux
9. 特殊情況解釋:
在死線之前，實驗室的Server突然無法把檔案載下來，所以我把原本Source code改的二個資料夾
./Placement ->此資料夾的.h檔
./Legalizer ->此資料夾的 legal.cpp 和 legal.h檔
這幾個檔案加入原本Source Code即可以執行
很抱歉，造成助教的不便

10. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
執行 time ./legalizer -aux ibm01-cu85.aux 後，console 會輸出


Benchmark: ibm01-cu85
Memory usage: 22.4 MB
Core region: (-33330,-33208)-(33396,33320)
Top    :33320
Bottom :-33208
Left   :-33330
Right  :33396

////// Legalization ///////
start check

  # row error: 0
  # site error: 0
  # overlap error: 0
Check success!!
total displacement: 4.99503e+08
legalization success!


////////////////////
Benchmark: ibm01-cu85

real    0m0.465s
user    0m0.370s
sys     0m0.073s

	 