#include <iostream>
#include<Windows.h>
#include<string>
#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <vector>
#include <sstream>
#include <map>
#include <locale> 
#include <iomanip>
using namespace std;

//中文字必須轉變編碼 才不會亂碼 
string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//獲得臨時變數的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//獲得臨時變數的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

//CSV的每行字串透過逗點分割 
vector<string> _csv(string s)
{
    vector<string> arr;
    istringstream delim(s);
    string token;
    int c = 0;
    while (getline(delim, token, ','))           
    {
        arr.push_back(token);                 
        c++;                  
		if(c==9)
			break;                        
    }
    return  arr;
}
string dowCsv(){
	//網址 
	const char* url = "https://data.nhi.gov.tw/resource/Nhi_Fst/Fstdata.csv";
  //位置和檔名 
	const char* dst = "C:/Users/User/Documents/csvsearch/data.csv";
	//下載檔案 
	if (S_OK == URLDownloadToFile(NULL, url, dst, 0, NULL))
	{
		cout<<"Saved."<<endl;
		return "Saved";
	}
	else
	{
		cout<<"Failed."<<endl;
		return "Failed";
	}
	return "Failed";
}

struct csvdata{
	vector<map<string,string>> csv_alldata;
	vector<string> csv_title;
	string search_last;
	ifstream ifs;
	ofstream ofs;
	csvdata()
	{
		//讀黨 
		search_last="";
		ifs.open("searchlast.txt", ios::in);
    	if (!ifs.is_open()) {
        	cout << "Failed to open searchlast file.\n";
    	} else {
       	
			string line;
			while (getline(ifs, line))
			{
						
				search_last=line;
				
			}
        ifs.close();
    }
	}
	void setSearchLast(string search)
	{
		
		//寫黨 
		ofs.open("searchlast.txt",ios::ate);
    	if (!ofs.is_open()) {
        	cout << "Failed to open file.\n";
    	} else {

        ofs<<search;
		search_last=search;
        ofs.close();
    }
	}
	void searchAddress()
	{
		string input_search;
		
		if(search_last=="")
			search_last="無"; 
		cout<<"搜尋區域範例:(XX縣XX區)"<<endl;
		cout<<"輸入1可使用上次搜尋區域:"<<search_last<<endl<<"請輸入區域:";
		cin>>input_search;
		//輸入要查找的資料 
	
		
		if(input_search=="1")
		{
			input_search=search_last;
		}
		else
		{
			setSearchLast(input_search);
		}
		// 遍歷每筆資料查找其中的map 
		
		bool find=false;
		
		for(int k=0;k < csv_alldata.size();k++)
		{
	
			string address;
			//中文字一個字兩個字元 0~12 抓前六個中文字 
			address=address.assign(csv_alldata[k]["醫事機構地址"], 0, 12);
		
			//確定快篩數量不為0 且搜尋區域相同 
			if(csv_alldata[k]["快篩試劑截至目前結餘存貨數量"]!="0"&&address==input_search)
			{
				find=true;
				cout<<"醫院名稱: "<<left<< setw(25) <<csv_alldata[k]["醫事機構名稱"];
				cout<<"地址: "<<csv_alldata[k]["醫事機構地址"]<<endl;
			}
		

		}
		if(!find)
		{
			cout<<"並未搜尋到醫院"<<endl<<endl;
		}
	}
};

int main(int argc, char** argv)
{

	
	dowCsv();

	ifstream inFile("C:/Users/User/Documents/csvsearch/data.csv", ios::in);
	if (!inFile)
    {
        cout << "開啟檔案失敗！" << endl;
        exit(1);
    }   
    
    int count=0;
    //map 字典型態 key value 
    csvdata csvalldata;

    string line;
    while (getline(inFile, line))//getline(inFile, line)表示按行讀取CSV檔案中的資料   
    {
    	string str = UTF8ToGB(line.c_str()).c_str();
    	//row每筆資料的map 
    	map<string,string> csv_map;
    	//row每筆資料透過逗號分隔 
    	vector<string> a = _csv(str);
		//抓取Csv每行資料變成字典型式 
        for (int ii = 0; ii < a.size(); ii++)
        {
        	//第一行是標頭 醫事機構名稱 地址等等 所以只執行第一次儲存title 
        	if(count==0)
        	{
        		csvalldata.csv_title.push_back(a[ii]);
        		
			}
			//標題 資料 
			else{
				csv_map[csvalldata.csv_title[ii]]=a[ii];
			
			}

        }
        //將每筆資料放入 csv_alldata 因為0的資料只有title 所以跳過 >0再來做 
        if(count>0)
        	csvalldata.csv_alldata.push_back(csv_map);

       count++;
    }
   
	csvalldata.searchAddress();
	
	system("pause");
	return 0;

}
