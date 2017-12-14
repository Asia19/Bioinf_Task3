#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Queue
{
	int *p, n;
	int si, ei;

public:
	Queue()
	{
		n = 100;
		p = new int[n];
		si = 0;
		ei = 0;
	}
	Queue(int n)
	{
		this->n = n;
		p = new int[n];
		si = 0;
		ei = 0;
	}
	Queue & operator = (const Queue &q)
	//Queue(const Queue &q)
	{
		int *pp = new int[q.n];		
		si = 0;
		ei = 0;
		for (int i = 0; i < q.ei; i++)
		{
			pp[i] =q.p[i];
			ei = (ei + 1) % n;
		}
		delete [] p;
		p = pp;
		return *this;
	}	
	bool empty()
	{
		return (ei == si);
	}
	bool full()
	{
		return (ei + 1) % n == si;
	}
	void push(char v)
	{
		if (full()) throw "Стек полон, операция push невозможна";
		p[ei] = v;
		ei = (ei + 1) % n;
	}
	int pop()
	{
		if (empty()) throw "Стек пуст, операция pop невозможна";
		si = (si + 1) % n;
		return p[si - 1];
	}
	string print()
	{
		string s;
		if (!empty())
		{
			for (int i = si; i < ei; i++)
			{
				stringstream number;
				number << p[i];
				s += number.str();
				s += "  ";
				//s += '\n';
			}
			s += '\n';
		}
		else
		{
			s += "queue is empty";
			s += '\n';
		}
		return s;
	}
	/*~Queue(void)
	{
		delete[] p;
	}*/
};


class MatrixDNA
{
  
	int m, n, k;
	int fullStr;
	char **mas;
	int row_pl;
public:
	MatrixDNA(int m, int n, int k)
	{
		this->m = m;
		this->n = n;
		this->k = k;
		fullStr = 0;
		row_pl = n - k + 1;
		mas = new char*[m];
		for (int i = 0; i < m; i++)
			mas[i] = new char[n];
	}
	void push(string s)
	{
		if (fullStr != m)
		{
			for (int i = 0; i < n; i++)
				mas[fullStr][i] = s[i];
			fullStr++;
		}
		else throw "You're trying push too many string";
	}
	void print()
	{
		if (fullStr != m) throw "it're not enough strings";
		
	}
	int maxScore(Queue *s)
	{
		
		int *score = new int[row_pl];
		for (int ii = 0; ii < row_pl; ii++) score[ii] = 0;
		for (int i = 0; i < row_pl; i++) //проходим по всем переданным очередям, считаем для них score
		{
			int *M = new int[k];
			int *st = new int[m];	
			for (int ii = 0; ii < m; ii++)
				st[ii] = s[i].pop();    //текущие стартовые позиции
			
			for (int c= 0; c < k; c++)
			{
				int *letters = new int[4];
				for (int ii = 0; ii < 4; ii++) letters[ii] = 0;
				for (int r = 0; r < m; r++)
				{
					switch (mas[r][st[r] + c])
					{
					case 'A': letters[0]++;break;
					case 'C': letters[1]++; break;
					case 'G': letters[2]++; break;
					case 'T': letters[3]++; break;
					}
				}
				
				int max = letters[0];
				for (int ii = 1; ii < 4; ii++)
					if (letters[ii]>max) max = letters[ii];	
				M[c] = max;
				
			}
			delete[] st;
			
			
			for (int ii = 0; ii < k; ii++)
				score[i] += M[ii];
		
		}
		int max = score[0];
		int maxi = 0;
		for (int ii = 1; ii < row_pl; ii++)
			if (score[ii]>=max)
			{
				max = score[ii];
				maxi = ii;
			}
		return maxi; /// возвращаем у какой из переданных очередей(наборов старторвых позиций) максимальный score
	}
};

Queue rec_func(int q, int m, int row_pl, Queue _s, MatrixDNA *dna)
{	            
	Queue *s = new Queue[row_pl+2];
	Queue *ss = new Queue[row_pl+2];
	int max;
	if (q == m)
	{		
		for (int i = 0; i < row_pl; i++)
		{
			s[i] = _s;
			s[i].push(i);
			ss[i] = s[i];
		}
	
		max = dna->maxScore(s);
		
	}
	else
	{		
		for (int i = 0; i < row_pl; i++)
		{
			s[i] = _s;
			s[i].push(i);
			ss[i] = s[i];
			int qq = q + 1;		
			ss[i] = rec_func(qq, m, row_pl, s[i], dna);
			
		}	
		max = dna->maxScore(ss);		
	}

	return ss[max];
}


void main() {
	//считываем по строкам из файла  /////////

	string ss;
	ifstream fin("input.txt", ios_base::in);
	getline(fin, ss);
	int l1=ss.length();
	int ii;
	for (ii = 0; ii < l1; ii++)
		if (ss[ii] == ' ') break;
	string kk = ss.substr(0, ii);
	string mm = ss.substr(ii+1, l1);
	
	int k = stoi(kk);
	int m = stoi(mm);
	string *s = new string[m];
	//////////////////////////////////////////
	for (int i = 0; i < m; i++)
	{
		getline(fin, s[i]);
	}
	int n = s[0].length();
	MatrixDNA *dna = new MatrixDNA(m, n, k);
	for (int i = 0; i < m; i++)
		dna->push(s[i]);
	dna->print();

	
	//
	int row_pl = n - k + 1;
	int q = 1;
	Queue st(2 * m);
	Queue the_end(2 * m);
	the_end=rec_func(q, m, row_pl, st, dna);
	cout<<"Start positions:  "<<the_end.print()<<endl;

	string Result = "";
	//int l;
	for (int i = 0; i < m; i++)
	{
		int l = the_end.pop();		
		Result += s[i].substr(l, k);
		Result += '\n';		
	}	

	cout << "Result: " << endl << Result << endl;

	//// записываем результат в файл   ////////////////////////////
	ofstream fout("output.txt", ios_base::out | ios_base::trunc);
	fout << Result;
    fout.close();
	
	delete[] s;


}