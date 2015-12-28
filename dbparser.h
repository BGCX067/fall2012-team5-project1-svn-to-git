#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include "Table.h"
#pragma once


/*
This Parser class takes in a stringstream. The statement() function will try to parse the stringstream
and throw an error if it is not properly formatted. The grammar is implemented using accept() and expect()
functions. This parser is essentially a "recursive descent" parser. Some functions inspired by the 
wikipedia page: http://en.wikipedia.org/wiki/Recursive_descent_parser
*/

struct Literal
{
	typedef enum {_int, _string, _undefined} kind;

	kind k;
	std::string s;
	int i;

	Literal(): k(_undefined), s(""), i(-999)
	{}

	Literal(int ii):k(_int), s(""), i(ii)
	{}

	Literal(std::string ss): k(_string), s(ss), i(-999)
	{}

	Literal& operator= (Literal& other)
	{
		s = other.s;
		k = other.k;
		i = other.i;
		return *this;
	}

};

struct update_set
{
	std::vector<std::string> identifiers;
	std::vector<Literal> literals;

	update_set()
	{}

	void add_ident(std::string s)
	{
		identifiers.push_back(s);
	}
	void add_literal(Literal l)
	{
		literals.push_back(l);
	}
	void clear()
	{
		identifiers.clear();
		literals.clear();
	}

};

class Parser
{
	public:
	/*
	Helper functions and lower level functions for the parser
	*/
	typedef enum
	{
		_left_arrow, _identifier, _select, _project, _rename, 
		_plus, _minus, _crossjoin, _equality, _equals ,_not_equals, _lt, _lte, 
		_gt, _gte, _open, _close, _write, _exit, _show, _create_table, 
		_update, _insert, _delete, _lparen, _rparen, _where, _set, _values, _quote, 
		_comma, _OR, _AND, _semicolon, _relation, _primary_key, _varchar, _integer, _number 
	} Symbol;

	char cur_char;
	Symbol cur_sym;
	std::string current_identifier;
	Literal cur_lit;
	int cur_num;
	Table cur_table;
	std::istringstream &ipt;
	update_set cur_update_set;
	std::vector<std::string> att_list;
	std::vector<int> type_list;
	std::vector<Literal> lits;
	int cur_type;
	Database &database;
	
	
	Parser(std::istringstream &ss, Database &db): ipt(ss), database(db)
	{
		
	}
	
	bool is_alpha(char c)
	{
		if((c < 'A' || c > 'z')) return false;
		return true;
	}
	bool is_digit(char c)
	{
		if(c == '1' || c=='2' || c=='3' ||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='0')
			return true;
		return false;
	}
	bool get_number()
	{
		ipt.unget();
		char c = ipt.get();
		if(is_digit(c))
		{
			while(is_digit(c))
				c = ipt.get();
			ipt.unget();
			return true;
		}
		ipt.unget();
		return false;
	}
	void throw_fake_error(std::string s)
	{
		std:: cout<<"\n\n"<<"Error:  "<<s<<"\n*****************\n"
		<<"Use an exit command to end this program now.\n";
		char c;
		while(true) ;
	}
	//This function checks to see if the string passed in is coming up the in the stream. 
	//if not, it ungets the characters and returns false
	bool find_string(std::string s)
	{
		char cur_c;
		int len = s.length();
		int i=0;
		for(; i < len; i++)
		{
			cur_c = ipt.get();
			if(cur_c == s[i]) ;
			else if (cur_c == ';') break;
			else break;
		}
		if(i == len) return true;
		else
		{
			for(; i>=0; i--)
			{
				ipt.unget();
			}
			return false;
		}
	}
	
	
	//This function checks for a string in the stream, but it ungets the characters either way
	bool is_string(std::string s)
	{
		char cur_c;
		int len = s.length();
		int i=0;
		for(; i < len; i++)
		{
			cur_c = ipt.get();
			if(cur_c == s[i]) ;
			else if (cur_c == ';') break;
			else break;
		}
		bool flag = (i == len);

		for(; i>=0; i--)
		{
			ipt.unget();
		}
		return flag;
	}
	
	//This function gets an identifier if it is coming up in the stream
	bool get_identifier()
	{
		int i=0;
		char c;
		std::string s;
		c = ipt.get();
		s += c;
		if(is_alpha(c))
		{
			while(is_alpha(c) || is_digit(c))
			{
				c = ipt.get();
				s+=c;
			}
			
			ipt.unget();
			s = s.substr(0, s.size()-1);
			current_identifier = s;
			return true;
		}
		return false;
	}
	
	
	//The main function of the parser. This function gets the next symbol, and sets cur_sym
	void get_sym()
	{
		/*
		Check for the string equivalent of various different symbols. Set cur_symbol and stuff accordingly
		*/
		char c = ipt.get();
		while(c == ' ')
			c = ipt.get();
		ipt.unget();
		
		
		if(find_string("<-"))
		{
			cur_sym = _left_arrow;
			//std::cout<<"[Left arrow] ";
		}
		else if(find_string("select "))
		{
			cur_sym = _select;
			//std::cout<<"[select] ";
		}
		
		else if(find_string("("))
		{
			cur_sym = _lparen;
			//std::cout<<"[Left paren]";
		}
		else if(find_string(")"))
		{
			cur_sym = _rparen;
			//std::cout<<"[Right paren]";
		}
		else if(find_string("project"))
		{
			cur_sym = _project;
			//std::cout<<"[project] ";
		}
		
		else if(find_string("=="))
		{
			cur_sym = _equality;
			//std::cout<<"[equality]";
		}
		else if(find_string("="))
		{
			cur_sym = _equals;
			//std::cout<<"[equals] ";
		}	
		else if(find_string("!="))
		{
			cur_sym = _not_equals;
			//std::cout<<"[not equals] ";
		}
		else if(find_string("<"))
		{
			cur_sym = _lt;
			//std::cout<<"[less than] ";
		}
		else if(find_string(">"))
		{
			cur_sym = _gt;
			//std::cout<<"[greater than] ";
		}
		else if(find_string("<="))
		{
			cur_sym = _lte;
			//std::cout<<"[less than or equal] ";
		}
		else if(find_string(">="))
		{
			cur_sym = _gte;
			//std::cout<<"[greater than or equal] ";			
		}
		else if(find_string("rename"))
		{
			cur_sym = _rename;
			//std::cout<<"[rename] ";			
		}
		else if(find_string("+"))
		{
			cur_sym = _plus;
			//std::cout<<"[plus] ";
		}	
		else if(find_string("-"))
		{
			cur_sym = _minus;
			//std::cout<<"[minus] ";			
		}	
		else if(find_string("*"))
		{
			cur_sym = _crossjoin;
			//std::cout<<"[multiply] ";	
		}
		else if(find_string("OPEN"))
		{
			cur_sym = _open;
			//std::cout<<"[open] ";	
		}
		else if(find_string("CLOSE"))
		{
			cur_sym = _close;
			//std::cout<<"[close] ";	
		}	
		else if(find_string("WRITE"))
		{
			cur_sym = _write;
			//std::cout<<"[write] ";	
		}
		else if(find_string("EXIT"))
		{
			cur_sym = _exit;
			//std::cout<<"[exit] ";	
		}	
		else if(find_string("SHOW"))
		{
			cur_sym = _show;
			//std::cout<<"[show] ";	
		}	
		else if(find_string("CREATE TABLE"))
		{
			cur_sym = _create_table;
			//std::cout<<"[create table] ";	
		}	
		else if(find_string("UPDATE"))
		{
			cur_sym = _update;
			//std::cout<<"[update] ";	
		}	
		else if(find_string("INSERT INTO"))
		{
			cur_sym = _insert;
			//std::cout<<"[insert into] ";	
		}	
		else if(find_string("DELETE FROM"))
		{
			cur_sym = _delete;
			//std::cout<<"[delete from] ";	
		}
		else if(find_string("WHERE"))
		{
			cur_sym = _where;
			//std::cout<<"[where] ";	
		}	
		else if(find_string("SET"))
		{
			cur_sym = _set;
			//std::cout<<"[set] ";	
		}
		else if(find_string("VALUES FROM"))
		{
			cur_sym = _values;
			//std::cout<<"[values from] ";	
		}
		else if(find_string("\""))
		{
			cur_sym = _quote;
		}
		else if(find_string(","))
		{
			cur_sym = _comma;
			//std::cout<<"[comma] ";	
		}
		else if(find_string("&&"))
		{
			cur_sym = _AND;
			//std::cout<<"[AND] ";	
		}
		else if(find_string("||"))
		{
			cur_sym = _OR;
			//std::cout<<"[OR] ";	
		}
		else if(find_string(";"))
		{
			cur_sym = _semicolon;
			//std::cout<<"[semicolon] ";
		}
		else if(find_string("RELATION"))
		{
			cur_sym = _relation;
			//std::cout<<"[relation] ";
		}
		else if(find_string("PRIMARY KEY"))
		{
			cur_sym = _primary_key;
			//std::cout<<"[primary key] ";
		}
		else if(find_string("VARCHAR"))
		{
			cur_sym = _varchar;
			//std::cout<<"[varchar] ";
		}
		else if(find_string("INTEGER"))
		{
			cur_sym = _integer;
			//std::cout<<"[integer] ";
		}
		else if(get_identifier()){
			cur_sym = _identifier;
			//std::cout<<"[identifier] ";	
		}
		else if(get_number())
		{
			cur_sym = _number;
			//std::cout<<"[number] ";
		}
		else
		{
			std::string s = "Bad symbol dude: " + ipt.get();
			throw_fake_error(s);
		}
		std::cout<<"\n";

	}
	
	//This function returns true and gets the next symbol if cur_sym is equal to s
	bool accept(Symbol s)
	{
		if(cur_sym == s)
		{
			if(cur_sym != _semicolon ) get_sym();
			return true;
		}
		return false;
	}
	
	//This function throws an error if s is not accepted. Used for things like parenthesis
	bool expect(Symbol s)
	{
		if(accept(s))
			return true;
		std::cout<<s;
		throw_fake_error("failed expect.");
		return false;
	}
	
	//This function gets a set of symbols, and returns false if the set is not complete
	bool get_set(Symbol s[], int j)
	{
		bool flag = true;
		int i=0;
		while(flag)
		{
			i = 0;
			for(; i<j; i++)
			{
				if(!(accept(s[i])))
				{
					flag = false;
					break;
				}
			}
		}
		return (i == 0);
	}
	
	/*
	**************************************Grammar for parser, higher level implementation *******************************************
	*/

	bool statement()
	{
		get_sym();
		if(query())
		{
			expect(_semicolon);
			std::cout<<"\nPARSE SUCCEEDED\n";
			return true;
		}
		else if(command())
		{
			expect(_semicolon);
			std::cout<<"\nPARSE SUCCEEDED\n";
			return true;
		}
		if(accept(_semicolon)) return true;
		else throw_fake_error("Must enter a query or a command (properly formatted).\n");
	}

	bool query()
	{
		if(accept(_identifier))
		{
			std::string s = current_identifier;
			if(accept(_left_arrow))
			{
				if(expression())
				{
					//cur_table.table_name = s;
					//database.add_table(cur_table);
					std::cout<<"Storing cur_table as "<<s<<"\n";
					return true;
				}
				else throw_fake_error("Must enter expression after left arrow.\n");
			}
			else throw_fake_error("Must enter a left facing arrow after relation name.\n");
		}
		return false;
	}
	
	bool command()
	{
		/*
		if(accept(_open) || accept(_close) || accept(_write))
		{
			if(accept(_identifier))
				return true;
			throw_fake_error("Need an identifier after OPEN/CLOSE/WRITE/CREATE TABLE.");
		}
		*/

		if(is_open()) return true;
		else if(is_close()) return true;
		else if(is_write()) return true;
		else if(create_table()) return true;
		else if(accept(_exit)) return true;
		
		else if(update())
		{
			return true;
		}
		else if(accept(_insert) && accept(_identifier)
				&& accept(_values))
		{
			if(accept(_relation))
			{
				std::string s = current_identifier;
				if(expression())
				{
					//database.insert_into(s, cur_table);
					std::cout<<"Calling insert into :"<<s<<", cur_table\n";
					return true;
				}
			}
			else
			{
				std::string s = current_identifier;
				expect(_lparen);
				lits.clear();
				literal();
				lits.push_back(cur_lit);
				while(accept(_comma))
				{
					literal();
					lits.push_back(cur_lit);
				}
				expect(_rparen);
				//database.insert_into(s, lits);
				std::cout<<"calling insert("<<s<<", "<<lits.size()<<"\n";
				return true;
			}
			throw_fake_error("Something messed up after VALUES FROM.");
		}
		else if(show()) 
			{
				return true;
			}
		else if(accept(_delete) && accept(_identifier) && accept(_where) && condition()) return true;
		
		//else if(accept(_semicolon)) return true;

		return false;
	}
	
	bool is_open()
	{
		if(accept(_open) && accept(_identifier))
		{
			std::cout<<"Calling open("<<current_identifier<<")\n";
			return true;
		}
		return false;
	}

	bool is_close()
	{
		if(accept(_close) && accept(_identifier))
		{
			//database.close(current_identifier);
			std::cout<<"Calling close("<<current_identifier<<")\n";
			return true;
		}
		return false;
	}

	bool is_write()
	{
		if(accept(_write) && accept(_identifier))
		{
			std::cout<<"Calling write ("<<current_identifier<<")\n";
			return true;
		}
		return false;
	}

	bool show()
	{
		if(accept(_show) && atomic_expression())
		{
			std::cout<<"Calling cur_table.show\n";
			return true;
		}
		return false;
	}

	bool update()
	{
		if(accept(_update) && accept(_identifier))
		{
			expect(_set);
			std::string s = current_identifier;
			get_update_set();
			expect(_where);
			condition();
			//database.update(s, cur_update_set, cur_conditions);
			std::cout<<"calling update: "<<s<<", "<<cur_update_set.identifiers.size()<<", "<<cur_update_set.literals.size()<<"\n";
			return true;
		}
		return false;
	}

	bool get_update_set()
	{
		bool flag = true;
		std::string ident;
		Literal l;
		cur_update_set.clear();
		while(flag)
		{
			if(accept(_identifier))
			{
				ident = current_identifier;
			}
			else break;
			
			if(accept(_equals)) ;
			else break;

			if(literal())
			{
				l = cur_lit;
			}
			else break;

			cur_update_set.add_ident(ident);
			cur_update_set.add_literal(l);
		}
		return true;
	}

	bool create_table()
	{
		if(accept(_create_table) && accept(_identifier))
		{
			std::string s = current_identifier;
			expect(_lparen);
			if(typed_attribute_list())
			{
				std::vector<std::string> l1 = att_list;
				expect(_rparen);
				expect(_primary_key);
				expect(_lparen);
				if(attribute_list() && accept(_rparen))
				{
					//database.create_table(s, att_list, type_list,current_identifier);
					std::cout<<"Calling create table: "<<s<<", "<<att_list.size()<<", "<<current_identifier<<"\n";
					return true;
				}
			}
			throw_fake_error("Something messed up after CREATE TABLE.");
		}
		return false;
	}
	
	bool typed_attribute_list()
	{
		att_list.clear();
		type_list.clear();
		if(accept(_identifier) && type())
		{
			att_list.push_back(current_identifier);
			type_list.push_back(cur_type);
			while(accept(_comma))
			{
				if(accept(_identifier) && type())
				{
					att_list.push_back(current_identifier);
					type_list.push_back(cur_type);
				}
				else break;
			}
			return true;
		}
		return false;
	}
	bool type()
	{
		if(accept(_integer))
		{
			cur_type = 1;
			return true;
		}
		else if(accept(_varchar))
		{
			if(accept(_lparen) && accept(_number) && accept(_rparen))
			{
				cur_type = 2;
				return true;
			}
			return false;
		}
		return false;
	}
	
	bool expression()
	{
 
		if (selection() || prod_un_dif() || projection() || rename()
		    || atomic_expression())
		{
			return true;	
		}
		else if(accept(_semicolon)) return true;
		return false;
	}
	
	bool prod_un_dif()
	{
		if(atomic_expression())
		{
			Table t = cur_table;
			std::cout<<"t = cur_table\n";
			if(accept(_minus) && atomic_expression())
			{
				//cur_table = t.difference(cur_table);
				std::cout<<"Calling difference of t and cur_table";
				return true;
			}
			else if(accept(_plus) && atomic_expression())
			{
				//cur_table = t.combination(cur_table);
				std::cout<<"Calling plus of t and cur_table";
				return true;
			}
			else if(accept(_crossjoin) && atomic_expression())
			{
				//cur_table = crossjoin(t, cur_table);
				std::cout<<"calling crossjoin of t and cur_table\n";
				return true;
			}
		}
		return false;
	}
	
	bool atomic_expression()
	{
		if (accept(_lparen))
		{
			if(expression() && accept(_rparen))
			{
				return true;
			}
			throw_fake_error("Cant find expression after parenthesis");
		}
		else if (accept(_identifier))
		{
			//cur_table = database.find(current_identifier);
			std::cout<<"setting cur_table to find("<<current_identifier<<")\n";
			return true;
		}
		return false;	
	}
	
	bool selection()
	{
		if(accept(_select))
		{
			expect(_lparen);
			condition();
			//std::vector<Condition> c = cur_conditions;
			expect(_rparen);
			if(atomic_expression())
			{
				//cur_table = cur_table.select(c);
				std::cout<<"Calling cur_table.select(conditions)\n";
				return true;
			}
			else throw_fake_error("You need an atomic expression after that condition fool.");
			return false;
		}
		return false;
	}
	
	bool condition()
	{
		if(conjunction())
		{
			bool flag = true;
			while(accept(_OR))
			{
				flag = false;
				if(!(conjunction())) break;
				flag = true;
			}
			if (flag) return true;
			throw_fake_error("Enter a valid set of conjuctions and ||.");
		}
		return false;
	}
	
	bool conjunction()
	{
		if(comparison())
		{
			bool flag = true;
			while(accept(_AND))
			{
				flag = false;
				if(!(comparison())) break;
				flag = true;
			}
			if (flag) return true;
			throw_fake_error("Enter a valid set of comparisons and &&.");
		}
		return false;
	}
	
	bool comparison()
	{
		if(operand() && op() && operand()) return true;
		else if (accept(_lparen) && condition() && accept(_rparen)) return true;
		return false;
	}
	
	bool op()
	{
		if(accept(_equality) || accept(_not_equals) || accept(_lt) ||
		   accept(_lte) || accept(_gt) || accept(_gte)) return true;
		
		return false;
	}
	
	bool operand()
	{
		if(literal()) return true;
		else if (accept(_identifier)) return true;
		
		return false;
	}
	
	bool literal()
	{
		if(accept(_quote) && accept(_identifier) && accept (_quote))
		{
			//std::cout<<"[literal] ";
			Literal l(current_identifier);
			cur_lit = l;
			return true;
		}
		else if(accept(_number))
		{
			Literal l(cur_num);
			cur_lit = l;
			return true;
		}
		else return false;
	}
	
	bool projection()
	{
		if(accept(_project) && accept(_lparen) && attribute_list() && accept(_rparen) && atomic_expression())
		{
			//Table t = cur_table.project(att_list);
			std::cout<<"Calling cur_table = cur_table.project("<<att_list.size()<<")\n";
			//cur_table = t;
			return true;
		}
		return false;
	}
	
	bool attribute_list()
	{
		att_list.clear();
		if(accept(_identifier))
		{
			att_list.push_back(current_identifier);
			bool flag = true;
			while(accept(_comma))
			{
				flag = false;
				if(!(accept(_identifier))) break;
				att_list.push_back(current_identifier);
				flag = true;
			}
			if(flag)
			{
			std::cout<<"[attribute list] ";
			return true;
			}
			throw_fake_error("Bad attribute list.");
		}
		return false;
	}
	
	bool rename()
	{
		if(accept(_rename) && accept(_lparen) && attribute_list() && accept(_rparen) && atomic_expression())
		{
			//cur_table.rename()
			std::cout<<"Calling cur_table.rename("<<att_list.size()<<")/n";
			return true;
		}
		return false;
	}	
};