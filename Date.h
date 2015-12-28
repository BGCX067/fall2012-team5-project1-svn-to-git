#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;
int max (int a, int b)
{
   if (a>b) return(a) ; else return (b);
}

int min (int a, int b)
{
   if (a>b) return(b); else return (a);
}

class Date
{
 public:
    Date ();
    Date ( int mn, int day, int yr);
    void display();
    int GetMonth();
    void SetMonth(int mn);
    int month, day, year;
    int DaysSoFar();
    
    inline bool operator==( const Date&d){
    	return(month==d.month && day==d.day && year==d.year);
    }
    inline bool operator!=( const Date&d){
    	return(month!=d.month && day!=d.day && year!=d.year);
    }
    Date &operator= (const Date & other)
	{
	month=other.month;
	day=other.day;
	year=other.year;
	}
};



istream &operator>>(istream& is, Date& d)
{
	int month, day, year;
	is>>month;
	is.ignore(1);
	is>>day;
	is.ignore(1);
	is>>year;
	d.month=month;
	d.year=year;
	d.day=day;
	return is;
}

ostream &operator<<(ostream& os, const Date& d)
{
   os << d.month << '/' << d.day << "/" << d.year ;
   return os;
}
 Date::Date ()
{
   month = day = year = 1;
}

Date::Date (int mn, int dy, int yr)
{
   static int length[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   month = max(1, mn);
   month = min(month,12);

   day = max(1,dy);
   day = min(day, length[month]);

   year = max(1, yr);
}

void Date::display()
{
   static char *name[] = {"nothing", "January", "February", "March", "April",
            "May", "June", "July", "August", "September", "October",
            "November", "December" };

   cout << '\n' << name[month] << ' ' << day << "," << year << '\n';
   cout << "Days so far: " << DaysSoFar() << '\n';
}

int Date::DaysSoFar()
{
   int total = 0;
   static int length[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   for (int i=1; i < month; i++) total += length[i];
   total += day;
   return (total);
}

int Date::GetMonth()
{
   return month;
}

void Date::SetMonth(int mn)
{
   month = max(1, mn);
   month = min(month, 12);
}
