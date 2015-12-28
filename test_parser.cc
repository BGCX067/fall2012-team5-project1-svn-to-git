#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "dbparser.h"


int main()
{
	//std::string s = "cats_or_dogs <- dogs + ( select ( kind == \"cat\" ) animals);";
  std::string s = "common_names <- project (name) (select (aname == name && akind != kind) (a * animals));";
	//std::string s = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);"; 
	std::istringstream ss(s, std::istringstream::out);
	Parser p(ss);
	p.statement();
	
	
	return 0;
}

