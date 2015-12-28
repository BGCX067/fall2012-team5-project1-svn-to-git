#include <iostream>
#include "Table.h"
#include <string>

using namespace std;
void option_selection();
	Database db;

void intro(){
	string input;
	cout<<"Would you like to read table('read') from file or create new('create') tables?"<<endl;
	cin>>input;
	bool user_created=false;
	bool car_created=false;
	bool maint_created=false;
		bool keep_reading=true;
	if(input=="create"){
		vector<Data> v;
		Data name, phone, email;
		name.type=STR; phone.type=STR; email.type=STR;
		name.title="name"; phone.title="phone"; email.title="email";
		v.push_back(name); v.push_back(phone); v.push_back(email);
		Key k ("name","string");
		Table User ("User", v, k);
		db.add_table(User);

		vector<Data> d;
		Data progress, entry_date, release_date, garage_number, VIN;
		progress.type=INT; entry_date.type=DAT; release_date.type=DAT; garage_number.type=INT; VIN.type=STR;
		progress.title="Progress"; entry_date.title="EntryDate"; release_date.title="ReleaseDate"; garage_number.title="GarageNumber"; VIN.title="VIN";
		d.push_back(name); d.push_back(progress); d.push_back(entry_date); d.push_back(release_date), d.push_back(garage_number),d.push_back(VIN);
		Table Maint_Log ("MaintLog", d, k);
		db.add_table(Maint_Log);	

		vector<Data> c;
		Data year, model;
		year.type=INT; model.type=STR;
		year.title="Year"; model.title="Model"; 
		c.push_back(year); c.push_back(model); c.push_back(VIN);
		Table Car ("Car", c, k);
		db.add_table(Car);
		user_created=true;
		car_created=true;
		maint_created=true;
		cout<<"New tables created."<<endl;
	}
	else if(input=="read"){
		while(keep_reading){
		cout<<"Which table would you like to load?"<<endl;
		cout<<"1: User Table"<<endl;
		cout<<"2: Maintenance Log"<<endl;
		cout<<"3: Car Table"<<endl;
		cin>>input;
		if(input=="1"){
			ifstream inputfile ("User.txt");
			if(inputfile.is_open()){
				char decision;
				cout<<"File 'User.txt' found. Upload this file? (y/n)"<<endl;
				cin>>decision;
				if(decision=='y'){
					string user="User";
					cout<<"Reading User.txt..."<<endl;
					db.open(user.c_str());
					if(db.find(user.c_str()).isempty())cout<<"READ FAILED"<<endl;
					else {
						db.find(user.c_str()).show();
						user_created=true;
						}
				}
				else if(decision=='n'){
					cout<<"Input name of file you would like to read: "<<endl;
					string input;
					cin>>input;
					cout<<"Reading "<<input<<"..."<<endl;
					db.open(input.c_str());
					if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				}
			}
			else {
				cout<<"Input name of file you would like to read: "<<endl;
				cin>>input;
				cout<<"Reading "<<input<<"..."<<endl;
				db.open(input.c_str());
				if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				cout<<"Loaded file "<<input<<".txt into User Table"<<endl;
				db.find(input.c_str()).show();
			}
			user_created=true;
			inputfile.close();
		}
		else if(input=="2"){
			ifstream inputfile1 ("MaintLog.txt");
			if(inputfile1.is_open()){
				char decision;
				cout<<"File 'MaintLog.txt' found. Upload this file? (y/n)"<<endl;
				cin>>decision;
				if(decision=='y'){
					string maintlog="MaintLog";
					cout<<"Reading MaintLog.txt..."<<endl;
					db.open(maintlog.c_str());
					if(db.find("MaintLog").isempty())cout<<"READ FAILED"<<endl;
				}
				else if(decision=='n'){
					cout<<"Input name of file you would like to read: "<<endl;
					string input;
					cin>>input;
					cout<<"Reading "<<input<<"..."<<endl;
					db.open(input.c_str());
					if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				}
			}
			else {
				cout<<"Input name of file you would like to read: "<<endl;
				cin>>input;
				cout<<"Reading "<<input<<"..."<<endl;
				db.open(input.c_str());
				if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				cout<<"Loaded file "<<input<<".txt into Maintenance Log"<<endl;
				db.find(input.c_str()).show();
			}
			maint_created=true;
		}
		else if(input=="3"){
			ifstream inputfile2 ("Car.txt");
			if(inputfile2.is_open()){
				char decision;
				cout<<"File 'Car.txt' found. Upload this file? (y/n)"<<endl;
				cin>>decision;
				if(decision=='y'){
					string car="Car";
					cout<<"Reading Car.txt..."<<endl;
					db.open(car.c_str());
					if(db.find(car.c_str()).isempty())cout<<"READ FAILED"<<endl;
					else db.find(car.c_str()).show();
				}
				else if(decision=='n'){
					cout<<"Input name of file you would like to read: "<<endl;
					string input;
					cin>>input;
					cout<<"Reading "<<input<<"..."<<endl;
					db.open(input.c_str());
					if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				}
			else {
				cout<<"Input name of file you would like to read: "<<endl;
				cin>>input;
				cout<<"Reading "<<input<<"..."<<endl;
				db.open(input.c_str());
				if(db.find(input.c_str()).isempty())cout<<"READ FAILED"<<endl;
				cout<<"Loaded file "<<input<<".txt into Car Table"<<endl;
				db.find(input.c_str()).show();
			}
		}
		car_created=true;
	}
	cout<<"Would you like to read in another file? (y/n)"<<endl;
	char continue_reading;
	cin>>continue_reading;
	if(continue_reading=='y') keep_reading=true;
	else keep_reading=false;
}

	}
	if(car_created==false){
		Key k ("name","string");
		cout<<"Car Table was not created. Creating car table now..."<<endl;
		vector<Data> c;
		Data year, model,VIN;
		year.type=INT; model.type=STR; VIN.type=INT;
		year.title="Year"; model.title="Model"; VIN.title="VIN";
		c.push_back(year); c.push_back(model); c.push_back(VIN);
		Table Car ("Car", c, k);
		db.add_table(Car);
	}
	if(maint_created==false){
			Key k ("name","string");
		cout<<"Maintenance Log was not created. Creating maintenance log now..."<<endl;
		vector<Data> d;
		Data progress, entry_date, release_date, garage_number, VIN, name;
		name.type=STR; progress.type=INT; entry_date.type=DAT; release_date.type=DAT; garage_number.type=INT; VIN.type=INT;
		name.title="name"; progress.title="Progress"; entry_date.title="EntryDate"; release_date.title="ReleaseDate"; garage_number.title="GarageNumber"; VIN.title="VIN";
		d.push_back(name); d.push_back(entry_date); d.push_back(release_date), d.push_back(garage_number),d.push_back(VIN);
		Table Maint_Log ("MaintLog", d, k);
		db.add_table(Maint_Log);
	}
	if(user_created==false){
	
		cout<<"User table was not created. Creating user table now..."<<endl;
		vector<Data> v;
		Data name, phone, email;
		name.type=STR; phone.type=STR; email.type=STR;
		name.title="name"; phone.title="phone"; email.title="email";
		v.push_back(name); v.push_back(phone); v.push_back(email);
		Key k ("name","string");
		Table User ("User", v, k);
		db.add_table(User);
	}
		
}
void display_menu(){
  cout<<"\n"<<"MAIN MENU:"<<endl;
  cout<<"1.Insert Data"<<endl;
  cout<<"2.Sort Maintenance Table"<<endl;
  cout<<"3.Display Table"<<endl;
  cout<<"4.Update Data"<<endl;
  cout<<"5.Select Data"<<endl;
  cout<<"6.Project Data"<<endl;
  cout<<"7.Display Database"<<endl;
  cout<<"8.Load from File"<<endl;
  cout<<"9.Open"<<endl;
  cout<<"10.Close"<<endl;
  cout<<"11.Exit"<<endl;
  
}

void display_insert_data(){
  cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table: ";
  int option;
  string cx_name;
  string cx_phone;
  string cx_email;
  int year_input; string model_input; string vin_input;
  int garage_number;
  Date entry_date;
  Date release_date;
Data email, phone, name;
Data vin, model, year;
Data status, entrydate, releasedate, garagenum;
        Date datenull (0,0,00);
vector<Data> vec;
  if(cin>>option){
    switch(option){
      case 1:
        cout<<"\n"<<"To insert customer information, please enter the customer's name:";
        cin>>cx_name;
        name.type=STR; name.title="name";
        memset(name.data.s, 0, sizeof(char)*64);
		memcpy(name.data.s, cx_name.c_str(), strlen(cx_name.c_str())+1);
        cout<<"Please enter the customer's phone number: ";
        cin>>cx_phone;
        phone.type=STR; phone.title="phone";
        memset(phone.data.s, 0, sizeof(char)*64);
		memcpy(phone.data.s, cx_phone.c_str(), strlen(cx_phone.c_str())+1);
        cout<<"Please enter the customer's email address: ";
        cin>>cx_email; 
        email.type=STR; email.title="email";
        memset(email.data.s, 0, sizeof(char)*64);
		memcpy(email.data.s, cx_email.c_str(), strlen(cx_email.c_str())+1);
        vec.push_back(name);
        vec.push_back(phone);
        vec.push_back(email);
        db.find("User").insert_data(vec);
        db.find("User").show();
        cout<<"Customer's information was added successfully!";
        break;
      case 2:
        cout<<"\n"<<"To insert vehicle information, please insert the vehicle's ID number:";
        cin>>vin_input;
        vin.type=STR; vin.title="VIN";
        memset(vin.data.s, 0, sizeof(char)*64);
		memcpy(vin.data.s, vin_input.c_str(), strlen(vin_input.c_str())+1);
        cout<<"Please enter the vehicle's model: ";
        cin>>model_input;
        model.type=INT; name.title="model";
        memset(model.data.s, 0, sizeof(char)*64);
		memcpy(model.data.s, model_input.c_str(), strlen(model_input.c_str())+1);
        cout<<"Please enter the vehicle's year: ";
        cin>>year_input;
        year.type=INT; year.title="year";
        year.data.i=year_input;
        //cout<<"Please enter the garage number, where the vehicle will be serviced: ";
        //cin>>garage_number;
        vec.push_back(year);
        vec.push_back(model);
        vec.push_back(vin);
        db.find("User").insert_data(vec);
        db.find("User").show();
        cout<<"Vehicle's information was added successfully!";
        break;
      case 3:
        cout<<"\n"<<"To enter a record into the Maintenance log, please insert the customer's name: ";
        cin>>cx_name;
        name.type=STR; name.title="name";
        memset(name.data.s, 0, sizeof(char)*64);
		memcpy(name.data.s, cx_name.c_str(), strlen(cx_name.c_str())+1);
        cout<<"Please enter the vehicle's ID number:";
        cin>>vin_input;
        vin.type=STR; vin.title="VIN";
        memset(vin.data.s, 0, sizeof(char)*64);
		memcpy(vin.data.s, vin_input.c_str(), strlen(vin_input.c_str())+1);
        cout<<"Please enter the entry date: ";
        cin>>entry_date;
        entrydate.type=DAT; entrydate.title="EntryDate";
        entrydate.data.d=&entry_date;
        status.type=INT; status.title="Progress";
        status.data.i=0;
        releasedate.type=DAT; releasedate.title="ReleaseDate";
        releasedate.data.d=&datenull;
        cout<<"Please enter the garage number this vehicle will be stored in: "<<endl;
        cin>>garage_number;
        garagenum.type=INT; garagenum.title="GarageNumber";
        garagenum.data.i=garage_number;
        vec.push_back(name);
        vec.push_back(status);
        vec.push_back(entrydate);
        vec.push_back(releasedate);
        vec.push_back(garagenum);
        vec.push_back(vin);
        db.find("MaintLog").insert_data(vec);
        cout<<"The maintenance status on this vehicle has been set to PENDING"<<endl;
        cout<<"The release date on this vehicle has been set to UNSPECIFIED"<<endl;
        cout<<"Maintenance log record was added successfully!"<<endl;
        db.find("MaintLog").show();
        break;
      default:cout<<"That was an Invalid option";
          break;
          
    }
  }
  display_menu();
  option_selection();
  
}
void display_sort_data(){
	cout<<"How would you like to sort the maintenance table?"<<endl;
	cout<<"[1] By entry date"<<endl;
	cout<<"[2] By release date"<<endl;
	cout<<"[3] By completion status"<<endl;
	int choice;
	if(cin>>choice){
		switch(choice){
			case 1:
				cout<<"Sorting Maintenance table by entry date... "<<endl;
				db.find("MaintLog").sort("EntryDate");
				db.find("MaintLog").show();
				break;
			case 2:
				cout<<"Sorting Maintenance table by entry date... "<<endl;
				db.find("MaintLog").sort("ReleaseDate");	
				db.find("MaintLog").show();
				break;
		}
	}
	display_menu();
	option_selection();
				

}

void display_table(){
  cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table to display: ";
  int option;
  if(cin>>option){
      switch(option){
        case 1: 
          cout<<"CUSTOMER INFORMATION:"<<endl;
          db.find("User").show();
          break;
        case 2:
          cout<<"VEHICLE INFORMATION:"<<endl;
          db.find("Car").show();
          break;
        case 3:
          cout<<"MAINTENANCE LOG:"<<endl;
          db.find("MaintLog").show();
          break;
        default:
          cout<<"That was an invalid option"<<endl;
          break;
      }
  }
  display_menu();
  option_selection();

}

void display_update_data(){
  cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table to update: ";
  int option; string cx_name; string cx_new_name; string cx_new_phone; string cx_new_email;
  int update_option; int vin; int new_vin; string new_model; int new_year; int new_garage_number;
  int new_progress; Date new_entrydate; Date new_releasedate;
  if(cin>>option){
      switch(option){
        case 1:
          cout<<"Please enter the name of the customer whose"<<endl;
          cout<<"information you would like to update: ";
          cin>>cx_name;
          cout<<"You may update the following:"<<endl;
          cout<<"1.Customer's Name"<<endl;
          cout<<"2.Customer's Phone Number"<<endl;
          cout<<"3.Customer's Email Address"<<endl;
          cout<<"\n"<<"Which information would you like to update for this customer: ";
          cin>>update_option;
          if(update_option==1){
            cout<<"Please enter the new name for the customer: ";
            cin>>cx_new_name;
            db.find("User").update("name", cx_new_name, cx_name);
            cout<<"\n"<<"Customer's name updated Successfully!";
          }
          else if(update_option==2){
            cout<<"Please enter the new phone number for the customer: ";
            cin>>cx_new_phone;
            db.find("User").update("phone", cx_new_phone, cx_name);
            cout<<"\n"<<"Customer's phone updated Successfully!";
          }
          else if(update_option==3){
            cout<<"Please enter the new email address for the customer: ";
            cin>>cx_new_email;
            db.find("User").update("email", cx_new_email, cx_name);
            cout<<"\n"<<"Customer's new email updated Successfully!";
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          
          break;
        case 2:
          cout<<"Please enter the vehicle ID number whose"<<endl;
          cout<<"information you would like to update: ";
          cin>>vin;
          cout<<"You may update the following:"<<endl;
          cout<<"1.Vehicle's ID Number"<<endl;
          cout<<"2.Model"<<endl;
          cout<<"3.Year"<<endl;
          cout<<"4.Garage number"<<endl;
          cout<<"\n"<<"Which information would you like to update for this vehicle: ";
          cin>>update_option;
          if(update_option==1){
            cout<<"Please enter the new vehicle ID number for this vehicle: ";
            cin>>new_vin;
            db.find("Car").update("VIN", new_vin, vin);
            cout<<"\n"<<"Vehicle's ID number updated Successfully!";
          }
          else if(update_option==2){
            cout<<"Please enter the new model for this vehicle: ";
            cin>>new_model;
            db.find("Car").update("model", new_model, "VIN");
            cout<<"\n"<<"Vehicle's Model updated Successfully!";
          }
          else if(update_option==3){
            cout<<"Please enter the new year for this vehicle: ";
            cin>>new_year;
            db.find("Car").update("year", vin, new_year);
            cout<<"\n"<<"Vehicle's year updated Successfully!";
          }
          else if(update_option==4){
            cout<<"Please enter the new garage number for this vehicle: ";
            cin>>new_garage_number;
            db.find("Car").update("garage", new_garage_number, vin);
            cout<<"\n"<<"Vehicle's garage number updated Successfully!";
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          break;
        case 3:
        	cout<<"Please enter the name of the person whose maintenance status you would like to update: "<<endl;
        	cin>>cx_name;
        	cout<<"You may update the following: "<<endl;
        	cout<<"[1] Customer name"<<endl;
        	cout<<"[2] Progress" <<endl;
        	cout<<"[3] Entry Date" <<endl;
        	cout<<"[4] Release Date"<<endl;
        	cout<<"[5] Garage Number"<<endl;
        	cout<<"[6] VIN"<<endl;
        	cout<<"\n"<<"Which information would you like to update for this vehicle: ";
          cin>>update_option;
          if(update_option==1){
            cout<<"Please enter the new customer name: ";
            cin>>cx_new_name;
            db.find("MaintLog").update("name", cx_new_name, cx_name);
            cout<<"\n"<<"Customer's name updated Successfully!";
          }
          else if(update_option==2){
            cout<<"Please enter the progress for this vehicle (0 is not started, 1 is incomplete, and 3 is completed): ";
            cin>>new_progress;
            db.find("MaintLog").update("progress", new_progress, cx_name);
            cout<<"\n"<<"Vehicle's progress updated Successfully!";
          }
          else if(update_option==3){
            cout<<"Please enter the new entry date for this vehicle: ";
            cin>>new_entrydate;
            db.find("MaintLog").update("EntryDate", new_entrydate, cx_name);
            cout<<"\n"<<"Vehicle's entry date updated Successfully!";
          }
          else if(update_option==4){
            cout<<"Please enter the new release date for this vehicle: ";
            cin>>new_releasedate;
            db.find("MaintLog").update("ReleaseDate", new_releasedate, cx_name);
            cout<<"\n"<<"Vehicle's release date updated Successfully!";
          }
          else if(update_option==5){
            cout<<"Please enter the new garage number for this vehicle: ";
            cin>>new_garage_number;
            db.find("MaintLog").update("GarageNumber", new_garage_number, cx_name);
            cout<<"\n"<<"Vehicle's garage number updated Successfully!";
          } 
          else if(update_option==6){
            cout<<"Please enter the new VIN for this vehicle: ";
            cin>>new_vin;
            db.find("MaintLog").update("VIN", new_vin, cx_name);
            cout<<"\n"<<"Vehicle's VIN updated Successfully!";
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          break;
        default:
          cout<<"That was an invalid option"<<endl;
          break;
      }
  }
  display_menu();
  option_selection();


}

void display_select_data(){
cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table to select from: ";
  int choice; string cx_new_name; string cx_new_phone; string cx_new_email; int option;
  if(cin>>option){
      switch(option){
        case 1:
          cout<<"Would you like to select based on:"<<endl;
          cout<<"[1]Customer name"<<endl;
          cout<<"[2]Customer phone number"<<endl;
          cout<<"[3]Customer email address"<<endl;
          cin>>choice;
          if(choice==1){
            cout<<"Please enter the name you would like to display: ";
            cin>>cx_new_name;
            db.find("User").select("name", cx_new_name).show();
          }
          else if(choice==2){
            cout<<"Please enter the phone number you would like to display: ";
            cin>>cx_new_phone;
            db.find("User").select("phone", cx_new_phone).show();
          }
          else if(choice==3){
            cout<<"Please enter the email you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("email", cx_new_email).show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          
          break;
        case 2:
          cout<<"Would you like to select based on:"<<endl;
          cout<<"[1]Car model"<<endl;
          cout<<"[2]Vehicle Identification Number"<<endl;
          cout<<"[3]Car year"<<endl;
          cin>>choice;
          if(choice==1){
            cout<<"Please enter the model you would like to display: ";
            cin>>cx_new_name;
            db.find("User").select("model", cx_new_name).show();
          }
          else if(choice==2){
            cout<<"Please enter the VIN you would like to display: ";
            cin>>cx_new_phone;
            db.find("User").select("VIN", cx_new_phone).show();
          }
          else if(choice==3){
            cout<<"Please enter the year you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("year", cx_new_email).show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          
          break;
    	case 3:
          cout<<"Would you like to select based on:"<<endl;
          cout<<"[1]Customer name"<<endl;
          cout<<"[2]EntryDate"<<endl;
          cout<<"[3]ReleaseDate"<<endl;
          cout<<"[4]Progress"<<endl;
          cout<<"[5]Garage Number"<<endl;
          cout<<"[6]VIN"<<endl;
          cin>>choice;
          if(choice==1){
            cout<<"Please enter the name you would like to display: ";
            cin>>cx_new_name;
            db.find("User").select("name", cx_new_name).show();
          }
          else if(choice==2){
            cout<<"Please enter the Entry Date you would like to display: ";
            cin>>cx_new_phone;
            db.find("User").select("EntryDate", cx_new_phone).show();
          }
          else if(choice==3){
            cout<<"Please enter the Release Date you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("ReleaseDate", cx_new_email).show();
          }
          else if(choice==4){
            cout<<"Please enter the Progress you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("Progress", cx_new_email).show();
          }
          else if(choice==5){
            cout<<"Please enter the Garage Number you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("GarageNumber", cx_new_email).show();
          }
          else if(choice==6){
            cout<<"Please enter the VIN you would like to display: ";
            cin>>cx_new_email;
            db.find("User").select("VIN", cx_new_email).show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
            break;
        default:
          cout<<"That was an invalid option"<<endl;
          break;
      }
  }
  display_menu();
  option_selection();




}

void display_project_data(){
cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table to project from: ";
  int choice; string cx_new_name; string cx_new_phone; string cx_new_email; int option;
  if(cin>>option){
      switch(option){
        case 1:
          cout<<"Would attribute would you like to project:"<<endl;
          cout<<"[1]Customer name"<<endl;
          cout<<"[2]Customer phone number"<<endl;
          cout<<"[3]Customer email address"<<endl;
          cin>>choice;
          if(choice==1){
            db.find("User").project("name").show();
          }
          else if(choice==2){
            db.find("User").project("phone").show();
          }
          else if(choice==3){
            db.find("User").project("email").show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          
          break;
        case 2:
          cout<<"Would you like to select based on:"<<endl;
          cout<<"[1]Car model"<<endl;
          cout<<"[2]Vehicle Identification Number"<<endl;
          cout<<"[3]Car year"<<endl;
          cin>>choice;
          if(choice==1){
            db.find("User").project("model").show();
          }
          else if(choice==2){
            db.find("User").project("VIN").show();
          }
          else if(choice==3){
            db.find("User").project("year").show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
          
          break;
    	case 3:
          cout<<"Would you like to select based on:"<<endl;
          cout<<"[1]Customer name"<<endl;
          cout<<"[2]EntryDate"<<endl;
          cout<<"[3]ReleaseDate"<<endl;
          cout<<"[4]Progress"<<endl;
          cout<<"[5]Garage Number"<<endl;
          cout<<"[6]VIN"<<endl;
          cin>>choice;
          if(choice==1){
            db.find("User").project("name").show();
          }
          else if(choice==2){
            db.find("User").project("EntryDate").show();
          }
          else if(choice==3){
            db.find("User").project("ReleaseDate").show();
          }
          else if(choice==4){
            db.find("User").project("Progress").show();
          }
          else if(choice==5){
            db.find("User").project("GarageNumber").show();
          }
          else if(choice==6){
            db.find("User").project("VIN").show();
          }
          else{
            cout<<"\n"<<"Invalid option";
          }
            break;
        default:
          cout<<"That was an invalid option"<<endl;
          break;
      }
  }
  display_menu();
  option_selection();
}

void display_database(){
	for(int i=0; i<db.table_list.size();i++){
		db.table_list[i].show();
	}

}

void display_load_from_file(){


}
void display_open(){
	cout<<"Enter name of file to open into database: "<<endl;
	string file_name;
	cin>>file_name;
	db.open(file_name);
}

void display_close(){
	cout<<"Enter name of table to close and remove from database: "<<endl;
	string file_name;
	cin>>file_name;
	db.close(file_name);
}
/*
void display_search(){
	cout<<"Which table would you like to search?"<<endl;
  cout<<"\n"<<"1.Customer Information"<<endl;
  cout<<"2.Vehicle Information"<<endl;
  cout<<"3.Maintenance Log"<<endl;
  cout<<"\n"<<"Please Select a Table: ";
  int option;
	switch(option){
		case 1:
			cout<<"What 
		case 2:
		case 3:


}*/

void display_exit(){
	return;

}
void option_selection(){
  int option;
  string invalid;
  cout<<"\n"<<"Please select an option to continue: ";
  if(cin>>option){ 
      switch(option){
        case 1:display_insert_data();
          break;
            
        case 2:display_sort_data();
          break;
        case 3:display_table();
          break;
        case 4:display_update_data();
          break;
        case 5:display_select_data();
          break;
        case 6:display_project_data();
          break;
        case 7:display_database();
          break;
        case 8:display_load_from_file();
          break;
        case 9:display_open();
          break;
        case 10:display_close();
          break;
        case 11:return;
          break;
        	return;
          break;
        default:cout<<"That was not a valid option"<<endl;
            option_selection();
          break;
      }
  }
  display_menu();
  option_selection();
}

int main(){
intro();
  display_menu();
  option_selection();

}


