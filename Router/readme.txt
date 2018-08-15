1. 學號：R05943092
2. 姓名：曾育為
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: <tar cvfz  R05943092_PA4.tar.gz R05943092_PA4>
6. 各檔案說明：
	R05943092_PA4/src                       : 主程式檔
	R05943092_PA4/arearouting/  	        : 測資檔
	R05943092_PA4/outputfile/  	            : 輸出檔
	R05943092_PA4/outputfile/case8  	    : case8 比較檔
	R05943092_PA4/R05943092_PA4_report.doc  : 程式報告
7. 編譯方式說明：        	
   R05943092_PA4/src/make
8. 執行、使用說明：
   執行檔的命令格式為：./router <inputfile name> <outputfile name> 
   EX: ./router ../arearouting/testbench/case8 ../outputfile/case8.rpt
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 執行 ./router ../arearouting/testbench/case8 ../outputfile/case8.rpt 後，console 會輸出
		Boundary=(100000000,100000000)    --boundary的大小
		Pin size= 10000                   --pin 的數量
		Final net= 19998                  --最後net的數量
		Runtime= 0.208342                 --程式總執行時間

