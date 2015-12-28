#ifndef _TABLE_H
#define _TABLE_H
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Date.h"
#include "CNode.h"
using namespace std;
vector<Date> vec_date;
class Key{
	string name, kind;
	public:
		Key(string,string);
		Key();
		string get_name() {return name;};
		string get_kind() {return kind;};
	};
Key::Key(string a, string b) {
	name=a;
	kind=b;
	}
Key::Key(){
	name="";
	kind="";
	}


/***************************************************************/
/*class Evaluate{
	public: 
		Table table;
		vector<char> condition;
		Table evaluateselection();
		}
	Table Evaluate::evaluateselection(){
		for(int i=0; i<condition.size();i++){
			table=table.select(
			

*/
enum datatype{INT,STR,DAT};

class Data{
public:
	string title;
	union datavalue{
		int i;
		char s[64];
		Date *d;
		} data;
	
	datatype type;
	
	Data(datatype inType=INT):type(inType)
	{
		if(type == INT)
			data.i = 0;
		else if(type==STR)
			memset(data.s, 0, sizeof(char)*64);
		else {
			data.d;
			}
	}
	
	inline bool operator == (const Data &d)
    {
       if(d.type==INT)
		{
			return (data.i==d.data.i && title==d.title && type==d.type);
		}
		else if(d.type==STR)
		{
			return (data.s==d.data.s && title==d.title && type==d.type);
		}
		else
		{
			return(data.d==d.data.d && title==d.title && type==d.type);
		}
    }
    
    inline bool operator != (const Data &d)
    {
       if(d.type==INT)
		{
			return (data.i!=d.data.i || title!=d.title || type!=d.type);
		}
		else
		{
			return (data.s!=d.data.s || title!=d.title || type!=d.type);
		}
    }
	Data &operator= (const Data & other)
	{
		if(other.type==INT)
		{
			data.i=other.data.i;
			title=other.title;
			type=other.type;
		}
		else if(other.type ==STR)
		{
			title=other.title;
			type=other.type;
			memset(data.s, 0, sizeof(char)*64);
			memcpy(data.s, other.data.s, strlen(other.data.s)+1);
		}
		else
		{
			title=other.title;
			type=other.type;
			data.d=other.data.d;
	}
}
};

ostream &operator<<(ostream& os, const Data& d)
{
	if(d.type == STR)
	{
		return os << d.data.s;
	}
	else if(d.type ==INT)
	{
		return os << d.data.i;
	}
	else
	{
		return os<<*d.data.d;
	}
}



/*************************************************************/

class Table {
	public:
		Key primary_key;
		string table_name;
		vector<vector<Data> > matrix;
		
		//Get and set functions
		string get_tablename() {return table_name;};
		void set_tablename(string s) {table_name=s;};
		Key get_primary_key() { return primary_key;};
		void set_primary_key();
		Table select(string, Data);
		Table select(string, string);
		Table select(string, int);
		Table select(string, char, int);
		Table select(CNode*);
    	Table project(string); 
    	Table project(vector<string>);
		void rename(string, string);
		Table rename_all(vector<string>);
		//Table constructor
		Table(string a,vector<Data> b,Key e);
		Table();
		void show();
		void clear();
		void insert_data(vector<Data>);
    	void insert_column(vector<Data>);
		Table combination(Table);
		Table combination_norepeat(Table);
    	Table difference(Table);
    	Data find(Data);
    	//Update all integer attributes to new integer
    	void update(string,int);
    	//Update all string attributes to new string
    	void update(string,string);
    	//Update integer attributes to new integer where that integer is the second int
    	void update(string,int,int);
    	//Update integer attributes to new integer where that integer is CONDITION in relation to the second int
		void update(string,int,int,char);
		//Update string attributes to string if the first string matches the third string
		void update(string,string,string);
		void update(string, int, string);
		void update(string, Date, string);
		void write();
		Table load(string);
		
		void sort(string);
		
		
		bool isempty();
		    
	Table operator= (Table other)
	{
		if(other.isempty()==true){
			cout<<"Error, assignment of empty table"<<endl;
			}
		clear();
		matrix.resize(other.matrix.size());
		for(int i=0;i<other.matrix.size();i++){
			matrix[i].resize(other.matrix[i].size());
			}
			
		for(int i=0; i<other.matrix.size();i++){
			for(int j=0;j<other.matrix[0].size();j++){
				matrix[i][j]=other.matrix[i][j];
				}
		}
		return *this;
	}
		};
		
Table::Table(string a, vector<Data> b, Key e)
{
	primary_key=e;
	matrix.resize(b.size());
	for(int i=0; i<b.size(); i++){
		matrix[i].resize(1);
		matrix[i][0]=b[i];
		}
	table_name=a;
};

Table::Table()
{
	Key k;
	primary_key=k;
	matrix.resize(1);
	matrix[0].resize(1);
	matrix[0][0].data.i=0;
	show();
	table_name="NO TABLE NAME";
}

void Table::insert_data(vector<Data> a){	
	if(isempty()==true){
		for(int i=0; i<a.size(); i++){
			matrix[i][0]=a[i];
		}
		return;
	}
	for(int i=0; i<matrix.size(); i++){
		matrix[i].resize(matrix[0].size()+1);
		}
	int heigth=matrix[0].size()-1;
	for(int i=0; i<a.size(); i++){
		if(a[i].type==matrix[i][0].type){
			matrix[i][heigth]=a[i];
			}
		else{
			cout<<"INVALID TYPE ASSIGNED TO CELL "<<i<<","<<heigth<<endl;
			}
	}
}

bool Table::isempty(){
		for(int i=0; i<matrix[0].size(); i++){
			for(int j=0; j<matrix.size()-1;j++){
				if(matrix[j][0].type==INT){
					if(matrix[j][i].data.i!=0 ){
						return false;
					}
				}
				else if(matrix[j][0].type==STR){
					if(strlen(matrix[j][i].data.s)>0){
						return false;
					}
				}
			}
		}
	return true;
}

void Table::insert_column(vector<Data>d){
		if(isempty()==true){
			matrix.resize(1);
		 	matrix[0].resize(d.size());
         	for(int i=0; i<matrix[0].size(); i++){
				matrix[0][i]=d[i]; 
				matrix[0][i].type=d[i].type;
				matrix[0][i].title=d[i].title;
				cout<<"Continuing"<<endl;
			}
			cout<<"Width is: "<<matrix.size()<<"Hieghth is: "<<matrix[0].size()<<endl;
		}
		else{
         	matrix.resize(matrix.size()+1); 
         	cout<<"After inserting column, size is: "<<matrix.size()<<endl;
		 	int width=matrix.size();
		 	matrix[width-1].resize(matrix[0].size());
         	for(int i=0; i<matrix[0].size(); i++){
				matrix[width-1][i]=d[i]; 
				matrix[width-1][i].type=d[i].type;
			}
        } 
}


void Table::show() {
	if(isempty()==true){
		cout<<"Error displaying empty table"<<endl;
		return;
		}
	/*for(int k=0; k<matrix.size(); k++){
		cout<<matrix[k][0].title<<"\t";
	}
	cout<<"\n";*/
	for(int i=0; i<matrix[0].size(); i++){
		for(int j=0; j<matrix.size();j++){
			cout<<matrix[j][i]<<"\t";
			}
		cout<<"\n";
	}
}

Table Table::select(string s, Data d){
	vector<Data> rows(matrix.size());
	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
				if(matrix[i][0].data.s==d.data.s || matrix[i][0].data.i==d.data.i){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][0];
						}
				}
	}
	}
	
	Key k;
	Table t (" ", rows, k);	

	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
			for(int j=0; j<matrix[0].size();j++){
				if(matrix[i][j]==d){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][j];
						}
						/*
						for(int i=0;i<t.matrix[i].size();i++){
							t.matrix[i].resize(matrix[i].size()+1);
						}
						*/
					t.insert_data(rows);
				}
			}
		}
	}
	return t;
	}

Table Table::select(string s, string d){
	vector<Data> rows(matrix.size());
	int checkpoint=0;
	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
			for(int j=0; j<matrix[0].size();j++){
				checkpoint++;
				if(matrix[i][j].data.s==d){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][j];
						}
					break;
				}
			}
		}
	}
	Key k;
	Table t (" ", rows, k);	
	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
			for(int j=checkpoint; j<matrix[0].size();j++){
				if(matrix[i][j].data.s==d){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][j];
						}
					t.insert_data(rows);
				}
			}
		}
	}
	if(t.isempty()==false){
		return t;
	}
	else{		
			cout<<"Error, data not found, returning original table"<<endl;
			return *this;
	}
}


Table Table::select(string s, int d){
	vector<Data> rows(matrix.size());
	int checkpoint=0;
	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
			for(int j=0; j<matrix[0].size();j++){
				checkpoint++;
				if(matrix[i][j].data.i==d){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][j];
						}
					break;
				}
			}
		}
	}
	Key k;
	Table t (" ", rows, k);	
	for(int i=0; i<matrix.size(); i++){
		if(matrix[i][0].title==s){
			for(int j=checkpoint; j<matrix[0].size();j++){
				if(matrix[i][j].data.i==d){
					for(int k=0; k<matrix.size();k++){
						rows[k]=matrix[k][j];
						}
					t.insert_data(rows);
				}
			}
		}
	}
	if(t.isempty()==false){
		return t;
	}
	else{
			cout<<"Error, data not found, returning original table"<<endl;
			return *this;
	}
}

Table Table::select(string s, char c, int d){
	if(c=='>'){
		vector<Data> rows(matrix.size());
		int checkpoint=0;
		for(int i=0; i<matrix.size(); i++){
			if(matrix[i][0].title==s){
				for(int j=0; j<matrix[0].size();j++){
					checkpoint++;
					if(matrix[i][j].data.i>d){
						for(int k=0; k<matrix.size();k++){
							rows[k]=matrix[k][j];
							}
						break;
					}
				}
			}
		}
		Key k;
		Table t (" ", rows, k);	
		for(int i=0; i<matrix.size(); i++){
			if(matrix[i][0].title==s){
				for(int j=checkpoint; j<matrix[0].size();j++){
					if(matrix[i][j].data.i>d){
						for(int k=0; k<matrix.size();k++){
							rows[k]=matrix[k][j];
							}
						t.insert_data(rows);
					}
				}
			}
		}
		if(t.isempty()==false){
			return t;
			}
		else{
			cout<<"Error, data not found, returning original table"<<endl;
			return *this;
			}
	}
	else {
		vector<Data> rows(matrix.size());
		int checkpoint=0;
		for(int i=0; i<matrix.size(); i++){
			if(matrix[i][0].title==s){
				for(int j=0; j<matrix[0].size();j++){
					checkpoint++;
					if(matrix[i][j].data.i<d){
						for(int k=0; k<matrix.size();k++){
							rows[k]=matrix[k][j];
							}
						break;
					}
				}
			}
		}
		Key k;
		Table t (" ", rows, k);	
		for(int i=0; i<matrix.size(); i++){
			if(matrix[i][0].title==s){
				for(int j=checkpoint; j<matrix[0].size();j++){
					if(matrix[i][j].data.i<d){
						for(int k=0; k<matrix.size();k++){
							rows[k]=matrix[k][j];
							}
						t.insert_data(rows);
					}
				}
			}
		}
	if(t.isempty()==false){
			return t;
			}
		else{
			cout<<"Error, data not found, returning original table"<<endl;
			return *this;
			}
	}
}
 Table Table::project(string attribute){ 
		Table t; 
		t.matrix.resize(1);
		t.matrix[0].resize(matrix[0].size());
		vector<Data>column(matrix[0].size());
		for(int j=0; j<matrix.size();++j){   
			if(matrix[j][0].title==attribute){ 
				for(int i=0; i<matrix[0].size(); ++i){ 
					column[i]=matrix[j][i];
				}
			}
		}
		t.insert_column(column);
        return t; 
}

 Table Table::project(vector<string>  attribute_list){
 	vector<Data> first_row (attribute_list.size());
 	Key k ("name","kind");
 	for(int i=0; i<attribute_list.size();i++){
 		first_row[i].type=matrix[0][i].type;
 		first_row[i].title=matrix[0][i].title;
 		//cout<<"The first row is"<<first_row[i]<<endl;
 	}
	Table t ("ProjectedTable", first_row,k); 
 	for(int i=0; i<attribute_list.size();i++){
 		string attribute=attribute_list[i];
 		vector<Data>column(matrix[0].size());
		for(int j=0; j<matrix.size();++j){   
			if(matrix[j][0].title==attribute){ 
				for(int i=0; i<matrix[0].size(); ++i){ 
					column[i]=matrix[j][i];
				}
			}
		}
		t.insert_column(column); 
    }
    cout<<t.matrix.size()<<t.matrix[0].size()<<endl;
    return t;
}

void Table::clear() {
	for(int i=0; i<matrix[0].size(); i++){
		for(int j=0; j<matrix.size();j++){
			if(matrix[j][i].type=INT){
				matrix[j][i].data.i=0;
				}
			else if(matrix[j][i].type=STR){
				string hur="";
				memset(matrix[j][i].data.s, 0, sizeof(char)*64);
				memcpy(matrix[j][i].data.s, hur.c_str(), sizeof(char)*hur.length());
				}
			else {
				Date d (0,0,0);
				matrix[j][i].data.d=&d;
			}
		}
	}
}
	
Table Table::combination(Table t){
	if(t.isempty()==true){
		return *this;
	}
	Table d;
	int width=matrix.size();
	int heigth=matrix[0].size();
	if(t.matrix.size()>matrix.size()){
		d.matrix.resize(t.matrix.size());
		}
	else d.matrix.resize(matrix.size());
	for(int i=0; i<d.matrix.size(); i++){
		d.matrix[i].resize(t.matrix[0].size()+matrix[0].size());
		}
	int x_coordinate=0;
	int y_coordinate=0;
	for(int i=0; i<matrix[0].size(); i++){
	y_coordinate++;
		for(int j=0; j<matrix.size();j++){
			x_coordinate++;
			d.matrix[j][i]=matrix[j][i];
			}
	}
	for(int i=0; i<t.matrix[0].size(); i++){
		for(int j=0; j<t.matrix.size();j++){
			d.matrix[j][i+y_coordinate]=t.matrix[j][i];
			}
	}
		return d;
	}
/*	
Table Table::combination_norepeat(Table t){
	if(t.isempty()==true){
		return *this;
	}
	Table d;
	int same_cells=0;
	bool row_same=false;
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[0].size();j++){
			for(int k=0;k<t.matrix.size();k++){
				for(int l=0; l<t.matrix[0].size();l++){
					if(matrix[i][j]== t.matrix[i][j]) row_same=true;
				}
			}
			if(row_same) same_cells++;
			row_same=false;
		}
	}
	int same_rows=same_cells/matrix.size();
	//Sizing matrix
	int width=matrix.size();
	int heigth=matrix[0].size();
	if(t.matrix.size()>matrix.size()){
		d.matrix.resize(t.matrix.size());
		}
	else d.matrix.resize(matrix.size());
	for(int i=0; i<d.matrix.size(); i++){
		d.matrix[i].resize(t.matrix[0].size()+matrix[0].size()-same_rows);
		}
	int x_coordinate=0;
	int y_coordinate=0;
	
	//Filling d
	//
	for(int i=0; i<matrix[0].size(); i++){
		for(int j=0; j<matrix.size();j++){
		//Searches for cell's presence in Table
			for(int k=0;k<t.matrix.size();k++){
				for(int l=0; l<t.matrix[0].size();l++){
					if(matrix[i][j]== t.matrix[i][j]) row_same=true;
				}
			}
			if(row_same) same_cells++;
			row_same=false;
		}
	}
}
*/

Table Table::difference(Table t){
  if(t.isempty()==true){
    return *this;
  }
  Table d;
  int width=matrix.size();
  int heigth=matrix[0].size();

  if(t.matrix.size()>matrix.size()){
     d.matrix.resize(t.matrix.size());
    }
  else d.matrix.resize(matrix.size());
  
  //Good until here
  int rows_not_equal=0;
  int cells_not_equal=0;
  bool equal=true;
  for(int i=0; i<matrix.size(); i++){
  	for(int j=0;j<matrix[i].size();j++){
    	if(t.matrix[i][j]!=matrix[i][j]) equal=false;
    	}
    	if(!equal) {
    		rows_not_equal++;
    		equal=true;
    		}
    	
    }
    cout<<rows_not_equal<<"rows not equal"<<endl;
	for(int i=0;i<matrix[0].size();i++){
		d.matrix[i].resize(rows_not_equal);
	}
	
  int x_coordinate=0;
  int y_coordinate=0;
  for(int i=0; i<matrix[0].size(); i++){
  y_coordinate++;
    for(int j=0; j<matrix.size();j++){
      x_coordinate++;
      d.matrix[j][i]=matrix[j][i];
      }
  }
  Data h;
  h.type=INT;
  h.data.i=0;
  for(int i=0; i<t.matrix[0].size(); i++){
    for(int j=0; j<t.matrix.size();j++){
		if(matrix[j][i] ==t.matrix[j][i]){
			t.matrix[j][i]=h;
		}
		d.matrix[j][i]=t.matrix[j][i];
    }

 }
 
 
    return d;
 }  

void Table::rename(string old_title, string new_title)
{
 int i=0; 
 int j=0;
 for(; i < matrix.size(); i++)
  {
    if(matrix[i][0].title == old_title) break;
  }
 for(; j < matrix[i].size(); j++)
  {
    matrix[i][j].title = new_title;
  }
  return;
}

Table Table::rename_all(vector<string> attribute_names)
{
	for(int i=0; i<attribute_names.size(); i++){
		matrix[i][0].title = attribute_names[i];
		cout<<matrix[i][0].title<<endl;
	}
	return *(this);
}

Data Table::find(Data d){
	string key_name=primary_key.get_name();
	int column=0;
	for(int i=0; i<matrix.size();i++){
		if(matrix[i][0].title==key_name){
			column=i;
			}
	}
	for(int j=0; j<matrix[column].size();j++){
		if(matrix[column][j]==d){
			return matrix[column][j];
			}
	}
	cout<<"Error, data not found"<<endl;
	return d;
}


void Table::update(string title,int new_val){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				matrix[i][j].data.i=new_val;
			}
		}
	}
	
}

void Table::update(string title, string new_name){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				memcpy(matrix[i][j].data.s, new_name.c_str(), sizeof(char)*new_name.length());
			}
		}
	}
}

void Table::update(string title, int new_val, int old_val){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				if(matrix[i][j].data.i==old_val) matrix[i][j].data.i=new_val;
			}
		}
	}
	
}

void Table::update(string title, Date new_dat, string old_date_name){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				if(matrix[0][j].data.s==old_date_name) matrix[i][j].data.d=&new_dat;
			}
		}
	}
	
}

void Table::update(string title, int new_val, string old_val){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				if(matrix[i][j].data.s==old_val) matrix[i][j].data.i=new_val;
			}
		}
	}
	
}

void Table::update(string title, int new_val, int dependent_var, char cond){
	if(cond=='='){
		for(int i=0;i<matrix.size();i++){
			if(matrix[i][0].title==title){
				for(int j=0;j<matrix[0].size();j++){
					if(matrix[i][j].data.i==dependent_var) matrix[i][j].data.i=new_val;
				}
			}
		}
	}
	if(cond=='>'){
		for(int i=0;i<matrix.size();i++){
			if(matrix[i][0].title==title){
				for(int j=0;j<matrix[0].size();j++){
					if(matrix[i][j].data.i>dependent_var) matrix[i][j].data.i=new_val;
				}
			}
		}
	}
	if(cond=='<'){
		for(int i=0;i<matrix.size();i++){
			if(matrix[i][0].title==title){
				for(int j=0;j<matrix[0].size();j++){
					if(matrix[i][j].data.i<dependent_var) matrix[i][j].data.i=new_val;
				}
			}
		}
	}
	if(cond=='!'){
		for(int i=0;i<matrix.size();i++){
			if(matrix[i][0].title==title){
				for(int j=0;j<matrix[0].size();j++){
					if(matrix[i][j].data.i!=dependent_var) matrix[i][j].data.i=new_val;
				}
			}
		}
	}
}

void Table::update(string title, string new_name, string old_name){
	for(int i=0;i<matrix.size();i++){
		if(matrix[i][0].title==title){
			for(int j=0;j<matrix[0].size();j++){
				if(matrix[i][j].data.s==old_name){
					memset(matrix[i][j].data.s, 0, sizeof(char)*64);
					memcpy(matrix[i][j].data.s, new_name.c_str(), sizeof(char)*new_name.length());
				}
			}
		}
	}
}

void Table::sort(string title){
	//Find column first
	int column;
	int i=0;
	for(; i<matrix.size();i++){
		if(matrix[i][0].title==title) column=i;
	}
	
	if(matrix[i][0].type==INT){
		int i, j, key, numLength = matrix[column].size( );
		for(j = 1; j < numLength; j++)    // Start with 1 (not 0)
		{
			key = matrix[column][j].data.i;
			for(i = j - 1; (i >= 0) && (matrix[column][i].data.i < key); i--)   // Smaller values move up
			{
                 matrix[column][i+1] = matrix[column][i];
                 for(int k=0; k<matrix.size();k++){
                 	if(k!=column) {
                 		Data tmp=matrix[k][i+1];
                 		matrix[k][i+1]=matrix[k][i];
                 		matrix[k][i]=tmp;
                 	}
                 }
          }
         matrix[column][i+1].data.i = key;    //Put key into its proper location
     	}
     	return;
     }
     else if(matrix[i][0].type=DAT){
     	int i, j, numLength = matrix[column].size( );
		for(j = 1; j < numLength; j++)    // Start with 1 (not 0)
		{
			Date *key =  matrix[column][j].data.d;
			for(i = j - 1; (i >= 0) && (matrix[column][i].data.d < key); i--)   // Smaller values move up
			{
                 matrix[column][i+1] = matrix[column][i];
                 for(int k=0; k<matrix.size();k++){
                 	if(k!=column) {
                 		Data tmp=matrix[k][i+1];
                 		matrix[k][i+1]=matrix[k][i];
                 		matrix[k][i]=tmp;
                 	}
                 }
          }
         matrix[column][i+1].data.d = key;    //Put key into its proper location
     	}
     	return;
     }
}
	
Table Table::select(CNode * conditions)
{
	Table new_table;
	for(int i=0; i<matrix[0].size();i++){
			vector<Data> new_vec;
			for(int k=0; k<matrix.size();k++){
				new_vec.push_back(matrix[i][k]);
				}
			//if(evaluate_condition(conditions,new_vec) new_table.insert_data(new_vec);
		}
	}
	


Table Table::load(string file_name){
	Table new_table;
	char *cstr, *p;
	 cstr = new char [file_name.size()+1];
	 strcpy (cstr, file_name.c_str());
	ifstream filestream (cstr);
	string line;
	int number_of_lines=0;
  	while (std::getline(filestream, line))
	{
       	++number_of_lines;
	}
	  		Date date;
	filestream.close();
	filestream.open(cstr);
	if(filestream.is_open()){
		//Set Tablename
		getline(filestream,line);
		new_table.set_tablename(line);
		//Get titles
		getline(filestream,line);
		cstr = new char [line.size()+1];
		strcpy (cstr, line.c_str());
		p=strtok (cstr," ");
		new_table.matrix.resize(0);
  		for (int i=0; p!=NULL; i++)
  		{
  			new_table.matrix.resize(new_table.matrix.size()+1);
  			new_table.matrix[i].resize(number_of_lines-3);
  			new_table.matrix[i][0].title=p;
    		p=strtok(NULL," ");
  		}
  		//Getting types
  		int phi=0;
  		for (int i=0; i<new_table.matrix.size(); i++)
  		{
  			filestream>>phi;
  			if(phi==0) new_table.matrix[i][0].type=INT;
  			else if(phi==1) new_table.matrix[i][0].type=STR;
  			else if(phi==2) new_table.matrix[i][0].type=DAT;
 
  		}
  		//Filling Table, first row
  		int integer_input;
  		string string_input;
  		for(int i=0; i<matrix.size();i++){
  			if(new_table.matrix[i][0].type==INT){
  				filestream>>integer_input;
  				new_table.matrix[i][0].data.i=integer_input;
  			}
  			else if(new_table.matrix[i][0].type==STR){
  				filestream>>string_input;
  				memset(new_table.matrix[i][0].data.s, 0, sizeof(char)*64);
  				memcpy(new_table.matrix[i][0].data.s, string_input.c_str(), sizeof(char)*string_input.length());
  			}
  			else{
  				filestream>>date;
  				vec_date.push_back(date);
  				new_table.matrix[i][0].data.d=&vec_date[vec_date.size()-1];
  			}
  		}
 
  		//Filling rest of table
  		integer_input=0;
  		string_input=" ";
  		for(int i=1;!filestream.eof() && i<number_of_lines-3;i++){
  			for(int j=0; !filestream.eof() && j<new_table.matrix.size();j++){
  				if(new_table.matrix[j][0].type==INT){
  					filestream>>integer_input;
  					new_table.matrix[j][i].data.i=integer_input;
  					new_table.matrix[j][i].type=new_table.matrix[j][0].type;
  					new_table.matrix[j][i].title=new_table.matrix[j][0].title;
  					
  				}
  				else if(new_table.matrix[j][0].type==STR){
  					filestream>>string_input;
  					memset(new_table.matrix[j][i].data.s, 0, sizeof(char)*64);
  					memcpy(new_table.matrix[j][i].data.s, string_input.c_str(), sizeof(char)*string_input.length());
  					new_table.matrix[j][i].type=new_table.matrix[j][0].type;
  					new_table.matrix[j][i].title=new_table.matrix[j][0].title;
  				}
  				else if(new_table.matrix[j][0].type==DAT){
  					filestream>>date;
  					new_table.matrix[j][i].type=new_table.matrix[j][0].type;
  					new_table.matrix[j][i].title=new_table.matrix[j][0].title;
  					vec_date.push_back(date);
  					new_table.matrix[j][i].data.d=&vec_date[vec_date.size()-1];  
  				}
  			}
  		}
  	  			
  	}
  	filestream.close();
	return new_table;
}

void Table::write(){
	ofstream file_stream;
	string temp=table_name;
	string txt= ".txt";
	temp.append(txt);
	char *cstr;
	cstr = new char [temp.size()+1];
	strcpy (cstr, temp.c_str());
	file_stream.open(cstr);
	if(file_stream.is_open()){
		file_stream<<table_name<<"\n";
		for(int i=0; i<matrix.size();i++){
			file_stream<<matrix[i][0].title<<" ";
			}
		file_stream<<"\n";
		for(int i=0; i<matrix.size();i++){
			file_stream<<matrix[i][0].type<<" ";
			}
		file_stream<<"\n";
		int offset;
		for(int i=0; i<matrix[0].size();i++){
			for(int j=0;j<matrix.size();j++){
				if(matrix[j][0].type==INT){
					file_stream<<matrix[j][i].data.i<<" ";
				}
				else if(matrix[j][0].type==STR){
					file_stream<<matrix[j][i].data.s<<" ";
				}
				else {
					file_stream<<matrix[j][i].data.d<<" ";
				}
			}
			file_stream<<"\n";
		}
	
}
}
class Database{
  public:
    vector<Table> table_list;
    void add_table(Table);
  void print_to_file();
    void open(string);
    void close(string);
	void write(string);
	Table create_table(string, vector<string>, vector<int>, string);
	Table& find(string);
    Database();
	};

Table& Database::find(string s)
{
	for(int i = 0; i<table_list.size(); i++)
	{
		if (table_list[i].table_name.compare(s) == 0) return table_list[i];
		std::cout<<s<<"  !=  "<<table_list[i].table_name<<"\n";
	}
	std::cout<<"No table with that name found.\n";

}


	Table Database::create_table(string tablename, vector<string> attribute_names, vector<int> types, string primary_key)
	{
		Key new_key(primary_key, " ");
		int num_attributes = attribute_names.size();
		vector<Data> d(num_attributes);
		for(int i=0; i<num_attributes; i++)
		{
			d[i].title = attribute_names[i];
			if(types[i] == 0) d[i].type = INT;
			else d[i].type = STR;
		}
		Table new_t(tablename, d, new_key);
		add_table(new_t);
		return new_t;
	}

void Database::add_table(Table t){
  table_list.push_back(t);
  }
void Database::print_to_file(){
  ofstream out;
  out.open("DATABASE.txt");
  for(int k=0; k<table_list.size(); k++){
  out<<"---------------------------------------------------------------------------"<<"\n";
    for(int j=0; j<table_list[k].matrix.size();j++){
    out<<table_list[k].matrix[j][0].title<<"\t";
   }
  out<<"\n";
  for(int i=0; i<table_list[k].matrix[0].size(); i++){
    for(int j=0; j<table_list[k].matrix.size();j++){
      out<<table_list[k].matrix[j][i]<<"\t";
     }
    out<<"\n";
    }
  }
  out.close();
}  
void Database::open(string s){
  string txt=".txt";
  s.append(txt);
  Table t = t.load(s);
  t.show();
  t.set_tablename(s.substr(0, s.size()-4));
  add_table(t);
  }

void Database::close(string s){
  int i=0;
  for(; i<table_list.size();i++){
    if(table_list[i].table_name==s) {
      table_list[i].write();
      break;
    }
    }
  table_list.erase(table_list.begin( ) + i);
  }

void Database::write(string s)
{
	find(s).write();
}

Database::Database(){} 

#endif