#include<iostream>
using namespace std;

struct el_ts
{
	int label;
	int ind_tl;
	int next;
};
class Hash
{
	int n_max, n_os, NTS;
	el_ts *t;

	int hash_func(int key)
	{
		return key % n_os + 1;
	}
public:
	Hash(int n1=400, int n2=100)
	{
		n_max=n1; n_os=n2;NTS=n_os+1;
		t=new el_ts[n_max];
		for(int i=0; i< n_max; i++)
			t[i].label=t[i].ind_tl=t[i].next=-1;
	}
	Hash(Hash &a)
	{
		n_max=a.n_max;  n_os=a.n_os;  NTS=a.NTS;
		t=new el_ts[n_max];
		for(int i=0; i<n_max; i++)
			t[i]=a.t[i];
	}

	~Hash() {delete []t;}

	
	int Find(int key)
	{
		int i;
		i=hash_func(key);
		if(t[i].label==key) return i;
		while(t[i].next!=-1)
		{
			i=t[i].next;
			if(t[i].label==key) return i;
		}
		return -1;
	}
	int Add(int label1, int ind1)
	{
		int i,j;
		i=hash_func(label1);
		if(t[i].label==-1)                               //
		{
			t[i].label=label1;
			t[i].ind_tl=ind1;
		}
		else
		{
			//cout<<NTS<<endl;
			if(NTS>=n_max)return -2;                     //
			if(t[0].next==-1)                            //
			{	
				/*priv(NTS)*/
				t[NTS].label=label1;
	    		t[NTS].ind_tl=ind1;
				while(t[i].next!=-1)
					i=t[i].next;
				t[i].next=NTS;

                NTS++;
			}
			else
			{
				j=t[0].next;

				/*priv(j)*/

				t[j].label=label1;
	    		t[j].ind_tl=ind1;
				while(t[i].next!=-1)
					i=t[i].next;
				t[i].next=j;


				t[0].next=t[j].next;
				t[j].next=-1;
			}
		}
        return 0;
	}
	Hash operator=(Hash &a)
	{
		if(this==&a) return *this;
		n_max=a.n_max;  n_os=a.n_os;  NTS=a.NTS;
		delete []t;
		t=new el_ts[n_max];
		for(int i=0; i<n_max; i++)
			t[i]=a.t[i];
	}
	void Print_t()
	{
		for(int i=0; i< NTS; i++)
			if(t[i].label!=-1)
			cout<<i<<"  "<<t[i].label<<"  "<<t[i].ind_tl<<t[i].next<<endl;
	}
};

/*
const int k=9;
int main()
{
	int i,label1, ind1, kod;
	Hash B;
	for(i=1; i<=k; i++)
	{
		cin>>label1>>ind1;
		kod=B.Find(label1);
		cout<<"kod="<<kod<<endl;
		if(kod==-1)
		   B.Add(label1,ind1);
		else
			cout<<"error"<<endl;
	}
	B.Print_t();
	cout<<"END"<<endl;

	return 0;
}

*/
