// Распознает:  подряд должно идти четное число 1, затем нечетное число 0. Вариант 2.
#include <iostream>
using namespace std;


class Avto
{
	typedef void (Avto:: *lexfunk) ();
	lexfunk FunkGo[5][4];
	int state;
		
    public:
		Avto();
		void q01();
	    void q02();
	    void q11();
	    void q12();
	    int Translit(char ch);
	    void Error();
		void Error1();
	    void Acc();
		void Get_FunkGo(int i);
		
	   
};

/*typedef void (Avto:: *lexfunk) ();
lexfunk FunkGo[5][3]={
			                  {&Avto::q01,&Avto::q11,&Avto::Error},
		                      {&Avto::q02,&Avto::Error,&Avto::Error},
		                      {&Avto::q01,&Avto::Error,&Avto::Error},
		                      {&Avto::Error,&Avto::q12,&Avto::Error},
		                      {&Avto::q01,&Avto::q11,&Avto::Error}
		                     };*/
void start(char *str,Avto &a);                  // внешняя функция
int main()
{
	char str[30]="1111000";
	Avto a;

	//gets(str);
	start(str,a);
	return 0;
}
Avto::Avto()
	   {
		   state=0;
           FunkGo[0][0]=&Avto::q01; FunkGo[0][1]=&Avto::q11; FunkGo[0][2]=&Avto::Error; FunkGo[0][3]=&Avto::Error1;
		   FunkGo[1][0]=&Avto::q02; FunkGo[1][1]=&Avto::Error; FunkGo[1][2]=&Avto::Error; FunkGo[1][3]=&Avto::Acc;
		   FunkGo[2][0]=&Avto::q01; FunkGo[2][1]=&Avto::Error; FunkGo[2][2]=&Avto::Error; FunkGo[2][3]=&Avto::Error1;
		   FunkGo[3][0]=&Avto::Error; FunkGo[3][1]=&Avto::q12; FunkGo[3][2]=&Avto::Error; FunkGo[3][3]=&Avto::Error1;
		   FunkGo[4][0]=&Avto::q01; FunkGo[4][1]=&Avto::q11; FunkGo[4][2]=&Avto::Error; FunkGo[4][3]=&Avto::Error1;
		                      
	   }

void Avto::Get_FunkGo(int i)
{
	(this->*FunkGo[state][i])();
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
			  case '\0': return 3;
			  default :  return 2;
		   };
	   }
void Avto:: Error()
	   {
		   cout << "Error: Symbol is not corrected!" << endl; exit(1);
	   }
void Avto:: Error1()
	   {
		   cout << "No\n" << endl; exit(1);
	   }
void Avto:: Acc()
	   {
		   cout << "yes\n " << endl; exit(1);
	   }


void start(char *str, Avto &a)
	   {
		   int n,i,j;
		   n=strlen(str);
		   for(i=0; i<=n; i++)
		   {
			   j=a.Translit(str[i]);
               a.Get_FunkGo(j);
		   }
	   }