// Распознает:  подряд должно идти четное число 1, затем нечетное число 0.
#include <iostream>
using namespace std;


class Avto
{
	typedef void (Avto:: *lexfunk) ();
	lexfunk FunkGo[5][3];
	int state;
		
    public:
		Avto();
	    void q01();
	    void q02();
	    void q11();
	    void q12();
	    int Translit(char ch);
	    void Error();
	    void start(char *str);
		freand void start(char *str,Avto a);
	   
};

/*typedef void (Avto:: *lexfunk) ();
lexfunk FunkGo[5][3]={
			                  {&Avto::q01,&Avto::q11,&Avto::Error},
		                      {&Avto::q02,&Avto::Error,&Avto::Error},
		                      {&Avto::q01,&Avto::Error,&Avto::Error},
		                      {&Avto::Error,&Avto::q12,&Avto::Error},
		                      {&Avto::q01,&Avto::q11,&Avto::Error}
		                     };*/
int main()
{
	char str[30]="11112000";
	Avto a;

	//gets(str);
	start(str,a);
	return 0;
}
Avto::Avto()
	   {
		   state=0;
           FunkGo[0][0]=&Avto::q01; FunkGo[0][1]=&Avto::q11; FunkGo[0][2]=&Avto::Error;
		   FunkGo[1][0]=&Avto::q02; FunkGo[1][1]=&Avto::Error; FunkGo[1][2]=&Avto::Error;
		   FunkGo[2][0]=&Avto::q01; FunkGo[2][1]=&Avto::Error; FunkGo[2][2]=&Avto::Error;
		   FunkGo[3][0]=&Avto::Error; FunkGo[3][1]=&Avto::q12; FunkGo[3][2]=&Avto::Error;
		   FunkGo[4][0]=&Avto::q01; FunkGo[4][1]=&Avto::q11; FunkGo[4][2]=&Avto::Error;
		                      
	   }
void Avto:: q01() {state=1;}
	  
void Avto:: q02() {state=2;}
	  
void Avto:: q11() {state=3;}
	  
void Avto:: q12() {state=4;}
	   	  
int Avto::  Translit(char ch)
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

/*void Avto::start(char *str)
	   {
		   int n,i,j;
		   n=strlen(str);
		   for(i=0; i<n; i++)
		   {
			   j=Translit(str[i]);
              (this->*FunkGo[state][j])();
		   }
		   if(state==1)printf("yes\n");
		   else 
			   printf("no\n");
	   }
	   */

void start(char *str,Avto a)
	   {
		   int n,i,j;
		   n=strlen(str);
		   for(i=0; i<n; i++)
		   {
			   j=a.Translit(str[i]);
              (this->*FunkGo[a.state][j])();
		   }
		   if(state==1)printf("yes\n");
		   else 
			   printf("no\n");
	   }