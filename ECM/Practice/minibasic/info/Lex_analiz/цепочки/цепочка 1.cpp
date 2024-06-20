// Распознает:  подряд должно идти четное число 1, затем нечетное число 0.
#include <iostream>
using namespace std;


class Avto
{
	typedef void (Avto:: *lexfunk) ();
	lexfunk FunkGo[5][3];
	int state;
	string str;
		
    public:
		Avto();
	    void q01();
	    void q11();
	    void q20();
	    void q30();
	    int Translit(char ch);
	    void Error();
	    void start(string str);
	   
};


int main()
{
	string st="11000";
	Avto a;

	//gets(str);
	a.start(st);
	return 0;
}
Avto::Avto()
	   {
		   state=0;
           FunkGo[0][0]=&Avto::Error; FunkGo[0][1]=&Avto::q01;   FunkGo[0][2]=&Avto::Error;
		   FunkGo[1][0]=&Avto::Error; FunkGo[1][1]=&Avto::q11;   FunkGo[1][2]=&Avto::Error;
		   FunkGo[2][0]=&Avto::q20;   FunkGo[2][1]=&Avto::q01;   FunkGo[2][2]=&Avto::Error;
		   FunkGo[3][0]=&Avto::q30;   FunkGo[3][1]=&Avto::Error; FunkGo[3][2]=&Avto::Error;
		   FunkGo[4][0]=&Avto::q20;   FunkGo[4][1]=&Avto::Error; FunkGo[4][2]=&Avto::Error;
		                      
	   }
void Avto:: q01() {state=1;}
	  
void Avto:: q11() {state=2;}
	  
void Avto:: q20() {state=3;}
	  
void Avto:: q30() {state=4;}
	   	  
int Avto::  Translit(char ch)                 // определяется класс входного символа
	   {
		   switch (ch)
		   {
              case '0':  return 0;;
			  case '1':  return 1;
			  default :  return 2;
		   };
	   }
void Avto:: Error()
	   {
		   cout << "Error: Symbol is not corrected!" << endl; exit(1);
	   }
void Avto::start(string st)
	   {
		   int n,i,j;
		   str=st;
		   n=str.size();
		   for(i=0; i<n; i++)
		   {
			   j=Translit(str[i]);
              (this->*FunkGo[state][j])();
		   }
		   if(state==3)printf("yes\n");
		   else 
			   printf("no\n");
	   }
	   