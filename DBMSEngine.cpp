#include <iostream>
#include "Table.h"
#include <string>
using namespace std;

int main(){
	//TABLE INITIALIZATION
	Key k ("animals","kind");
	string str = "Obama";
	vector<Data> v (4);
	Data p,q,r,s;
	q.type=INT; p.type=INT; s.type=STR;
	q.data.i=5; p.data.i=6;
	s.title="candidate"; q.title="texasvotes"; p.title="alabamavotes";
	memset(s.data.s, 0, sizeof(char)*64);
	memcpy(s.data.s, str.c_str(), sizeof(char)*str.length());
	v[0]=s; v[1]=p; v[2]=q; v[3]=q;
	
	//INSERTING DATA
	string hur= "Romney";
	vector<Data> d (3);

	Data x,y,z;
	x.type=STR; y.type=INT; z.type=INT;
	y.data.i=2; z.data.i=3;
	x.title="candidate"; y.title="alabamavotes";
	memcpy(x.data.s, hur.c_str(), sizeof(char)*hur.length());
	d[0]=x; d[1]=y; d[2]=z;
	
	//TABLE FUNCTIONS
	Table t ("election", v, k);
	cout<<"Initialized Table: "<<endl;
	t.show();
	t.insert_data(d);
	cout<<"Added Romney Row:  "<<endl;
	t.show();
	
	//SELECT FUNCTION
	cout<<"Selecting only Obama: "<<endl;
	Table w=t.select("candidate", "Obama");
	w.show();
	cout<<"Combining Obama selection with table: "<<endl;
	t.combination(w).show();
  
    //PROJECT FUNCTION
  cout<<"Projecting 'alabamavotes': "<<endl;
  Table P=t.project("alabamavotes");
  P.show();
  
  //DIFFERENCE FUNCTION
  //cout<<"Difference between Table and Obama Selection"<<endl;
  //Table D=t.difference(w);
 // D.show();

  //CREATE DATABASE 
  Database elections;
  elections.add_table(t);
  elections.add_table(w);
  elections.add_table(P);
 // elections.table_list.add_table(D);
  
  //OUTPUT DATABASE TO FILE
  elections.print_to_file();
  
  }