1. 學號：R05943092
2. 姓名：曾育為
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: <tar cvfz  R05943092_PA2.tar.gz R05943092_PA2>
6. 各檔案說明：
	R05943092_PA2/Source/main.cpp    : 主程式
	R05943092_PA2/Source/Makefile    : Makefile
	R05943092_PA2/Source/btree.h     : 資料結構
	R05943092_PA2/Source/parser.cpp  : 讀檔
	R05943092_PA2/Source/btree.cpp   : btree
	R05943092_PA2/Source/contour.cpp : 進行floorplan
	R05943092_PA2/Source/sa.cpp      : sa流程
	R05943092_PA2/data/  		 : 測試檔
	R05943092_PA2/R05943092_PA2_report.doc ：程式報告
7. 編譯方式說明：        	
   R05943092_PA2/Source/make
8. 執行、使用說明：
   執行檔的命令格式為：./btree <a value> <input_block file name> <input_nets file name> <output file name>
   EX: ./btree 0.01 ami33.block ami33.nets ami33.rpt
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 執行 ./btree 0.01 ami33.block ami33.nets ami33.rpt 後，console 會輸出
	 best solution
               ~
	 Round = 529275			       —-Btree進行的operation次數
	 Fit in outline = 1588.                —-放入Outline次數
	 (Outline_w,Outline_h)=(5336,7673).    —-題目所給的Outline
	 (  Max_X  ,  Max_Y  )=(5152,7476).    —-最佳解的Max_X、Max_Y範圍
         HPWL: 543046			       —-HPWL的結果
	 Cost: 2.00376e+07		       —-cost function的結果
	 Program Runtime: 17.0871 sec.         —-程式總執行時間