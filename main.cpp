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

//����r�������ܽs�X �~���|�ýX 
string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//��o�{���ܼƪ��j�p
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//��o�{���ܼƪ��j�p
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

//CSV���C��r��z�L�r�I���� 
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
	//���} 
	const char* url = "https://data.nhi.gov.tw/resource/Nhi_Fst/Fstdata.csv";
  //��m�M�ɦW 
	const char* dst = "C:/Users/User/Documents/csvsearch/data.csv";
	//�U���ɮ� 
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
		//Ū�� 
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
		
		//�g�� 
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
			search_last="�L"; 
		cout<<"�j�M�ϰ�d��:(XX��XX��)"<<endl;
		cout<<"��J1�i�ϥΤW���j�M�ϰ�:"<<search_last<<endl<<"�п�J�ϰ�:";
		cin>>input_search;
		//��J�n�d�䪺��� 
	
		
		if(input_search=="1")
		{
			input_search=search_last;
		}
		else
		{
			setSearchLast(input_search);
		}
		// �M���C����Ƭd��䤤��map 
		
		bool find=false;
		
		for(int k=0;k < csv_alldata.size();k++)
		{
	
			string address;
			//����r�@�Ӧr��Ӧr�� 0~12 ��e���Ӥ���r 
			address=address.assign(csv_alldata[k]["��ƾ��c�a�}"], 0, 12);
		
			//�T�w�ֿz�ƶq����0 �B�j�M�ϰ�ۦP 
			if(csv_alldata[k]["�ֿz�վ��I�ܥثe���l�s�f�ƶq"]!="0"&&address==input_search)
			{
				find=true;
				cout<<"��|�W��: "<<left<< setw(25) <<csv_alldata[k]["��ƾ��c�W��"];
				cout<<"�a�}: "<<csv_alldata[k]["��ƾ��c�a�}"]<<endl;
			}
		

		}
		if(!find)
		{
			cout<<"�å��j�M����|"<<endl<<endl;
		}
	}
};

int main(int argc, char** argv)
{

	
	dowCsv();

	ifstream inFile("C:/Users/User/Documents/csvsearch/data.csv", ios::in);
	if (!inFile)
    {
        cout << "�}���ɮץ��ѡI" << endl;
        exit(1);
    }   
    
    int count=0;
    //map �r�嫬�A key value 
    csvdata csvalldata;

    string line;
    while (getline(inFile, line))//getline(inFile, line)��ܫ���Ū��CSV�ɮפ������   
    {
    	string str = UTF8ToGB(line.c_str()).c_str();
    	//row�C����ƪ�map 
    	map<string,string> csv_map;
    	//row�C����Ƴz�L�r�����j 
    	vector<string> a = _csv(str);
		//���Csv�C�����ܦ��r�嫬�� 
        for (int ii = 0; ii < a.size(); ii++)
        {
        	//�Ĥ@��O���Y ��ƾ��c�W�� �a�}���� �ҥH�u����Ĥ@���x�stitle 
        	if(count==0)
        	{
        		csvalldata.csv_title.push_back(a[ii]);
        		
			}
			//���D ��� 
			else{
				csv_map[csvalldata.csv_title[ii]]=a[ii];
			
			}

        }
        //�N�C����Ʃ�J csv_alldata �]��0����ƥu��title �ҥH���L >0�A�Ӱ� 
        if(count>0)
        	csvalldata.csv_alldata.push_back(csv_map);

       count++;
    }
   
	csvalldata.searchAddress();
	
	system("pause");
	return 0;

}
