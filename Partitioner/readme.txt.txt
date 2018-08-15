1. 學號：R05943092
2. 姓名：曾育為
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: <tar cvfz  R05943092_PA1.tar.gz R05943092_PA1>
6. 各檔案說明：
	 R05943092_PA1/Source/main.cpp : 主程式
	R05943092_PA1/Source/makefile : Makefile
	 R05943092_PA1/test_file/input_ .dat: 測試檔
	R05943092_PA1/test_file/output_.dat: 測試檔結果
	 R05943092_PA1/R05943092_PA1_report.doc ： 程式報告
7. 編譯方式說明：        	
   R05943092_PA1/Source/make
8. 執行、使用說明：
   執行檔的命令格式為：./fm <input file name> <output file name>
   EX: ./fm input_1.dat output_1.dat
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 執行 ./fm input_5.dat output_5.dat 後，console 會輸出
 	 balance factor= 0.01	--平衡係數
		Net number= 483599     	--net數量
		Cell number= 382489		--cell數量
	 Pin number= 4			--cell連結net最大數量
		Number of cut = 343031    	--初始化的cut數量
		====FM Result====
	Best_iteration = 169976	--在第幾個iteration得到最佳解
	Number of cut = 170000          --最後的cut數量
	Program Runtime: 3143.55 S      -- 程式總執行時間
