#include <iostream>
#include <fstream>
using namespace std;

string s;

struct node { int N; char c; node *next; node *Left; node *Right; };
struct Stacker { Stacker *next; node *t; string code; };

void REKURS_make_code_of_tree(node *&t);
void REKURS_destruct_tree(node *&t);

int main () {

    cout << "cin filename to read\n";
    cin >> s;
    s += ".txt";
    ifstream fin(s);
    //поток считывани€ из файла

    if (!fin) {
		cout << "NO\n";
		return 0;
		//если им€ введено направильно, то ничего не делать
	} else {
		cout << "good\n";
	}

	s = "";
	string s1 = "";
	while ( !fin.eof() )
    {
        getline(fin, s1);
        s = s + s1 + "\n";
        s1 = "";
    } //считали всЄ

    if ( s == "" )  {
        cout << "is empty";
        return 0;
    } //пустую строку игнорим

    s.erase(s.size()-1); //там лишний \n

    int a[256];
    for ( int i = 0; i < 256; ++i ) {
        a[i] = 0;
    }
    for ( int i = 0; i < s.size(); ++i ) {
        a[ s[i] ] += 1;
        //строчка выше значит: а[номер символа по ASCII] += 1
    }

    int numbMax = -1;
    int quantity = -1;
    node *head = nullptr, *helper = nullptr, *helper2;

    do {

        numbMax = -1;
        quantity = -1;
        for ( int i = 0; i < 256; ++i ) {
            if ( a[i] > quantity && a[i] != 0 ) {
                quantity = a[i];
                numbMax = i;
                //символ с максимальной ненулевой встречаемостью сохран€етс€
            }
        }
        if ( quantity != -1 ) {
            a[numbMax] = 0;
            helper = new node;
            helper->c = numbMax;
            helper->Left = helper->Right = nullptr;
            helper->N = quantity;
            helper->next = head;
            head = helper;
            //итого получим список, где от меньшего к большему идут частоты встречаемости
        }

    } while ( quantity != -1 ); //символы с нулевой встречаемостью не кодируем

    while ( head->next->next != nullptr ) {
        //когда останетс€ 2 элемента, выполним вне цикла

        //сформируем новую вершинку
        helper = new node;
        helper->Left = head;
        helper->Right = head->next;
        head = head->next->next;
        helper->N = helper->Left->N + helper->Right->N;
        helper->next = nullptr;
        helper->Left->next = helper->Right->next = nullptr;

        //вставим так, чтобы ближе к голове оп€ть были самые маленькие
        if ( head->N > helper->N ) {
            helper->next = head;
            head = helper;
        } else {
            helper2 = head;
            while ( helper2->next != nullptr ) {
                if ( helper2->next->N >= helper->N )  {break;}
                //будем вставл€ть после helper2
                helper2 = helper2->next;
            }
            helper->next = helper2->next;
            helper2->next = helper;
        } //вставили, ну, € на это надеюсь

        //и всЄ, дальше оп€ть режем список и плетЄм дерево
    }

    //а теперь у нас 2 элемента
    helper = new node;
    helper->Left = head;
    helper->Right = head->next;
    helper->next = helper->Left->next = helper->Right->next = nullptr;
    head = helper;

    //дерево готово, сделаем кодировку и тобличку

    cout << "tree is ready\n";

    s1 = s;
    s = "";

    //тобличка, нужен обход вширь и "очередь"
    //можно сделать рекурсивно
    Stacker *stacker = nullptr, *sHelper = nullptr, *tail = nullptr;
    stacker = new Stacker;
    stacker->next = nullptr;
    stacker->t = head;
    stacker->code = "";
        //эта шн€га хранит текущую последовательность 0101
    tail = stacker;

    //обход
    while ( tail != nullptr ) {
        if ( tail->t->Left != nullptr ) {
            //если nullptr, то и правый тоже, так что и в него загл€нем

            sHelper = new Stacker;
            sHelper->t = tail->t->Right;
            sHelper->code = tail->code + "1";
            stacker->next = sHelper;
            stacker = stacker->next;
            stacker->next = nullptr;

            sHelper = new Stacker;
            sHelper->t = tail->t->Left;
            sHelper->code = tail->code + "0";
            stacker->next = sHelper;
            stacker = stacker->next;
            stacker->next = nullptr;

            sHelper = tail;
            tail = tail->next;
            delete sHelper;
        } else {
            s = s + "{" + tail->t->c + "}" + tail->code + "\n";
            sHelper = tail;
            tail = tail->next;
            delete sHelper;
        }
    }
    s = s + "[";

    REKURS_make_code_of_tree(head);

    s += "]";

    REKURS_destruct_tree(head);

    s1 =  s + "\n" + s1;
    s = "";

    cout << "file to answer\n";
    cin >> s;
    s += ".txt";

    ofstream fout;
    fout.open(s);
    //поток вписывани€ в файл

    if (!fout) {
        cout << "error";
		return 0;
	}

    cout << "file is opened\n";

	fout << s1;

	cout << "Ok/n";

    return 0;
}

void REKURS_make_code_of_tree(node *&t)
{
    if ( t == nullptr ) { return; }
    if ( t->Left == nullptr ) {
        s = s + "{" + t->c + "}";
    } else {
        s += "(";
        REKURS_make_code_of_tree( t->Left );
        s += ")(";
        REKURS_make_code_of_tree( t->Right );
        s += ")";
    }
    return;
}

void REKURS_destruct_tree(node *&t)
{
    if ( t == nullptr ) { return; }
    if ( t->Left == nullptr ) {
        delete t;
    } else {
        REKURS_destruct_tree( t->Left );
        REKURS_destruct_tree( t->Right );
        delete t;
    }
    return;
}
