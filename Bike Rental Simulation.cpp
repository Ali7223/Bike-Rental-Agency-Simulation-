#include <iostream>
#include <sstream>
#include <iomanip>
#include <dos.h>
#include <conio.h>

using namespace std;


class DayOfYear{
unsigned int dayofyear, year;

const static int days[];

public :
    DayOfYear(int day_year=1, int _year=365){
    dayofyear=day_year;
    year=_year;
    }

    DayOfYear(int _day, int _month, int _year){

    dayofyear=days[_month-1];

    dayofyear+=_day;
    year=_year;
         }

    int get_dayofYear(){return dayofyear;}
    DayOfYear operator ++(){

    int dayofyear1;
    int year1=year;
    if (dayofyear%365==0){
        year1=year+1;
        dayofyear1=1;
    }
    dayofyear1=dayofyear+1;
     DayOfYear temp(dayofyear1,year1);
    return temp;
    }
    friend istream& operator >>(istream &input , DayOfYear &Day){
    char date[10] ;
    int day,month;
    input>>date;
    istringstream ss(date);
    ss>>Day.year;
    ss.ignore();
    ss>>month;
    ss.ignore();
    ss>>day;
    Day.dayofyear=days[month-1]+day;
    return input;
    }
    friend ostream &operator <<(ostream & output, DayOfYear dy){
    int month=0, day;
    while (dy.dayofyear>dy.days[month])
        month++;

    day=dy.dayofyear-dy.days[month-1];
    output<<setiosflags(ios::fixed | ios::showpoint);
    output<<dy.year<<"-";
    (month<10)? output<<setw(2)<<setfill('0')<<month: output <<month;
    output<<"-";
    (day<10)? output<<setw(2)<<setfill('0')<<day : output <<day;
    return output;


    }
    };

const int DayOfYear::days[]={0,31,59,90,120,151,181,212,242,273,304,334,365};

class Vehicle{
    private:
    const int no;
    string model;
    float price24h;

public:
      Vehicle(int _no, string _model, float _price):no(_no){

      model=_model;
      price24h=_price;
      }

      virtual ~Vehicle(){
      }
      int get_no(){return no;}
      string get_model(){return model;}
      float get_price(int no_ofdays){return no_ofdays*price24h;}
      virtual void print()=0;
};
class Bike:public Vehicle{
public :
    Bike(int _no,string _model, float _price = 9.99):Vehicle(_no,_model,_price){
    }
    virtual void print(){
    cout<<get_no()<<" : "<<get_model()<<" (Bike) "<<endl; }

};

//Subtask Emotor Vehicle
class EMotorVehicle:public Vehicle{
public:
    EMotorVehicle(int _no, string _model, float _price):Vehicle(_no,_model, _price){}
    virtual bool is_streetLegal()=0;
    virtual void print(){
    cout<<get_no()<<": "<<get_model();
    if(!is_streetLegal())
        cout<<"(not street Legal)";
   }

};

// Subtask Class Ebike
class Ebike:public EMotorVehicle{
    public:
    Ebike(int _no, string _model, float _price=29.99):EMotorVehicle(_no,_model,_price){}
    virtual bool is_streetLegal(){return true;}
    virtual void print(){
    EMotorVehicle::print();
    cout<<" (Ebike)"<<endl;
    }
};

//Subtask class Escooter

class EScooter:public EMotorVehicle{
    bool streetLegal;
public:
    EScooter(int _no,string _model,float _price=19.99, bool _legal=false):EMotorVehicle(_no, _model, _price),streetLegal(_legal){}
    virtual bool is_streetLegal(){return streetLegal;}
    virtual void print(){
        EMotorVehicle::print();
        cout<<" (EScooter)"<<endl;
    }
};

//Subtask class rental
class Rental{
private:
    const int no;
    DayOfYear from;
    int days;
    string customer;
    static int last_no;
public:
    Rental(string _rentingPerson, DayOfYear _start,int _rentalDays=1):customer(_rentingPerson),from(_start),days(_rentalDays),no(last_no){

    last_no++;
    }

    int get_days(){return days;}
    DayOfYear get_from(){return from;}
    DayOfYear get_until(){
    DayOfYear temp;
    temp=from;
    int x=temp.get_dayofYear();
    for(int i=x;i<x+days;i++)
    temp=++temp;
    return temp;
    }
    void print(){

    cout<<get_from()<<" to "<<get_until()<<" rental no. "<<no<<" for "<<customer<<endl;
    }
};

int Rental::last_no=1;

//subtask class schedule
class Schedule{
    private:
        Vehicle *vehicle;
        Rental *year[365];
        Schedule *next;

    public:
Schedule(Vehicle *ptr){
        vehicle=ptr;
        for (int i=0;i<365;++i)
            year[i]=NULL;//Only addresses comes here
        next=NULL;
    }
Vehicle* get_vehicle(){return vehicle;}
Schedule* get_next(){return next;}
void set_next(Schedule *ptr){next=ptr;}

bool isFree(DayOfYear date, int _noDays){
            int x=date.get_dayofYear()-1;
            for(int i=x;i<x+_noDays;i++)
            if (year[date.get_dayofYear()]!=NULL)
                return false;
            else
                return true;
        }
float book(string customer, DayOfYear day, int _noDays){

            Rental *p=new Rental(customer,day, _noDays);
            int x=day.get_dayofYear()-1;
            for(int i=x;i<x+_noDays;i++){

                        year[i]=p;
        }
            return vehicle->get_price(_noDays);
        }
void print(DayOfYear date){
            vehicle->print();
            if(year[date.get_dayofYear()]!=NULL)
            year[date.get_dayofYear()]->print();
        }
void print(){
             cout <<"Schedule For ";
             vehicle->print();
             for(int i=0;i<365;i++)
                if (year[i]!=NULL)

        {
                int j;
                for(j=0;j<i;j++)
                    if (year[i]==year[j])
                        break;
                    if(i==j){
                        year[i]->print();
                        cout<<", price :" <<vehicle->get_price(year[i]->get_days())<<endl;

                    }



             }
                }

};
//Subtask Agency

class Agency{
private:
    float profit_percent;
    float profit;
    Schedule *head,*next;

protected:
    Schedule* searchFor(int x){
    Schedule *next1=head;
    while(next1->get_next()!=NULL){
    int a=next1->get_vehicle()->get_no();
    if(a==x){
    return next1;
}

    else{
    next1=next1->get_next();
}
    }
    return next;
}



public:
Agency():profit_percent(0.20),profit(0.00), head(NULL),next(NULL){}
float get_profit(){return profit;    }
void set_profitPercentage(float x){profit_percent=x/100;}
void add(Vehicle *p){
        Schedule *node=new Schedule(p);
        if (head==NULL)
            head=node;
        else{
            for(next=head;next->get_next()!=NULL;next=next->get_next()){}
            next->set_next(node);
        }
        }


public:
bool isFree(int vehicle_number, DayOfYear date, int no_of_days){
    Schedule *p=searchFor(vehicle_number);
    return p->isFree(date,no_of_days);
    }
float book(int vehicle_number, string customer, DayOfYear date, int no_of_days){
     Schedule *p=searchFor(vehicle_number);
     float price=p->book(customer,date,no_of_days);
     profit=(price*profit_percent);
     return price;
     }


int chooseVehicle(){
     int x;
     for(next=head;next!=NULL;next=next->get_next()){next->get_vehicle()->print();}
     cout<<"choose vehicle number : ";
     cin>>x;

     return x;
}

void print(DayOfYear date){
      cout<<"Schedule for "<<date<<endl;
     for (next=head;next!=NULL;next=next->get_next())
         next->print(date);

     }

void print (){
     for (next=head;next!=NULL;next=next->get_next()){next->print();}
    cout<<"profit :"<<fixed<<setprecision(2)<<get_profit()<<" Euro "<<endl;
    }

};

DayOfYear today(31,01,2021);
DayOfYear tommorow=++today;
char menu(){

    char c;
cout<<"MENUE"<<endl;
cout<<"A end of Simulation"<<endl;
cout<<"B set new Simulation Date"<<endl;
cout<<"C new Rental manually"<<endl;
cout<<"D print rental of today "<<today<<endl;
cout<<"E print Rental of tommorow "<<tommorow<<endl;
cout<<"F print all rentals"<<endl;
cout<<"G print agency Profit"<<endl;
cout<<"H set agency profit percent"<<endl;
cout<<"I add Bikes"<<endl;
cout<<"J add Ebikes"<<endl;
cout<<"K add E-Scooters"<<endl;
cout<<"L new Rental simulation"<<endl;
cout<<"Your choice :";
cin>>c;
return c;

}


int main()

{
Agency obj;
int x=0;
int count1=0,count2=0,count3=0;

while(true){
        system("cls");
        switch(menu())
        {
        case 'a':
           exit(0);


        case 'b':

            cout<<"Input Date of today YYYY-MM-DD: ";
            cin>>today;
            tommorow=++today;

            break;

        case 'c':{
            int no=obj.chooseVehicle();
            DayOfYear day;
            string name;
            cout<<"Day of rental in format YYYY-MM-DD :";
            cin>>day;
            int x;
            cout<<"No of Days : ";
            cin>>x;
            cout<<"input data vehicle renter name : ";
            cin>>name;
            cout<<"booked price for this rental "<<fixed<<setprecision(2)<<obj.book(no,name, day, x);
            getch();
            break;
         }


        case 'd':
            obj.print(today);
            getch();

            break;

        case 'e':
           obj.print(tommorow);
            getch();
            break;

        case 'f':

            obj.print();
            getch();
            break;


        case 'g':
            cout<<"profit "<<obj.get_profit()<<endl;
            break;

        case 'h':{
            float profit;
            cout<<"Agency profit percent : ";
            cin>>profit;
            obj.set_profitPercentage(profit);
            break;
            }
        case 'i':{

            int a;
            cout<<"how many bikes in this simulation ?";
            cin>>a;

            for(int i=0;i<a;i++){
                Vehicle *ptr=new Bike(++x,"City"+to_string(++count1));
                obj.add(ptr);
            }
            break;
        }

        case 'j':{
            int y;

            cout<<"how many bikes in this simulation ?";
            cin>>y;


            for(int i=0;i<y;i++){

                Ebike *ptr1=new Ebike(++x,"Trekky"+to_string(++count2));
                obj.add(ptr1);
            }
            break;
        }
       case 'k':{
           int z;
           cout<<"how many EScooters in this simulation ?";
           cin>>z;

           for(int i=0;i<z;i++){
            EScooter *ptr2=new EScooter(++x,"Escooter"+to_string(++count3));
            obj.add(ptr2);
           }

           break;
       }
       case 'l':{
       cout<<"Did not do case L"<<endl;
       getch();}
       default:
            cout<<"Invalid Character Press any button to continue"<<endl;
            getch();
            break;


        }


}

}

