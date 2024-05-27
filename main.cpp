#include<bits/stdc++.h>
using namespace std;
ifstream in("date.in");
ofstream out("date.out");
template<typename T>
void afisazav(vector<T> v)
{
    cout<<v.size()<<"\n";
    for ( T x : v )
    {
        cout<<x<<" ";
    }
    cout<<'\n';
}
template<typename T>
class B_Tree
{
    const static int NR_FII = 10;
    ///nr_min_fii = NR_FII => nr_min_k = NR_FII - 1
    ///nr_max_fii = 2 * NR_FII => nr_max_k = 2 * NR_FII - 1
    struct Nod
    {
        Nod* tata;
        vector<T> valori;
        Nod* fii[NR_FII*2+2] = {NULL};
        int nr_fii = 0;
        Nod(Nod* nod)
        {
            tata = nod -> tata;
            for ( T val : nod -> valori )
                valori.push_back(val);
            for ( int i = 1 ; i <= nod->nr_fii ; i++ )
            {
                fii[i] = nod->fii[i];
            }
            nr_fii = nod->nr_fii;
        }
        Nod(T x)
        {
            valori.push_back(x);
            nr_fii = 2;
            tata = NULL;
        }
        Nod(vector<T>& v, Nod* vfii[], int nr)
        {
            valori = v;
            for ( int i = 1 ; i <= nr ; i++ )
                fii[i] = vfii[i];
            nr_fii = nr;
            tata = NULL;
        }
        bool sunt_frunza()
        {
            for ( int i = 1 ; i <= nr_fii ; i++ )
            {
                if ( fii[i] != NULL )
                    return false;
            }
            return true;
        }

    };
    Nod* root;
public:
    B_Tree()
    {
        root = NULL;
    }
    int da_poz_min(Nod*& nod, T x)
    {
        if ( nod == NULL )
            return -1;
        int i = 0;
        while ( i < nod -> valori.size() && nod -> valori[i] <= x )
            i++;
        return i - 1;
    }
    int da_poz_pe_tata(Nod*& nod, int poz)
    {
        Nod* t = nod -> tata;
        int poz_t = da_poz_min(t, nod->valori[poz]);
        return poz_t;
    }
    Nod* da_fratele_dreapta(Nod*& nod, int poz)
    {
        return nod->tata->fii[da_poz_pe_tata(nod, poz)+3];
    }
    Nod* da_fratele_stanga(Nod*& nod, int poz)
    {
        return nod->tata->fii[da_poz_pe_tata(nod, poz)+1];
    }
    Nod* cauta(Nod*& nod, T x)
    {
        if ( nod == NULL )
            return nod;
        int i = da_poz_min(nod, x);
        if ( i == -1 )
            return cauta(nod->fii[1], x);
        if ( nod -> valori[i] == x )
            return nod;
        return cauta(nod -> fii[i+2], x);
    }
    void adauga_n_nod(Nod*& nod, T x, Nod* fiul_adaugat=NULL)
    {
        nod -> nr_fii++;
        nod -> valori.push_back(x);
        nod -> fii[nod -> nr_fii] = fiul_adaugat;
        int i = nod -> valori.size() - 1, j = nod -> nr_fii;
        while ( i >= 1 && nod -> valori[i-1] > nod -> valori[i] )
        {
            swap(nod -> valori[i], nod -> valori[i-1]);
            swap(nod -> fii[j], nod -> fii[j-1]);
            j--;
            i--;
        }
    }
    void splituieste(Nod*& nod)
    {
        vector<T> vnou;
        Nod* fii_nou[NR_FII*2+1] = {NULL};
        int j = 0;
        fii_nou[++j] = nod->fii[nod->valori.size()/2+2];
        for ( int i = nod -> valori.size()/2+1 ; i < nod -> valori.size() ; i++ )
        {
            vnou.push_back(nod->valori[i]);
            fii_nou[++j] = nod -> fii[i+2];
        }
        Nod* fratele = new Nod(vnou, fii_nou, j);
        T mediana = nod -> valori[nod -> valori.size()/2];
        nod -> valori.resize(nod->valori.size()/2);
        nod -> nr_fii = nod -> valori.size() + 1;
        if ( nod -> tata != NULL ){
            fratele -> tata = nod -> tata;
            incearca_adaugare(nod->tata, mediana, fratele);
        }
        else
        {
            nod -> tata = new Nod(mediana);
            nod -> tata -> fii[1] = nod;
            nod -> tata -> fii[2] = fratele;
            fratele -> tata = nod -> tata;
            if ( nod == this -> root )
                this -> root = nod -> tata;
        }
    }
    void incearca_adaugare(Nod*& nod, T x, Nod* fiu_adaugat=NULL)
    {
        if ( nod == NULL )
        {
            nod = new Nod(x);
            return;
        }
        int aux = da_poz_min(nod, x);
        if ( nod -> valori[aux] == x ){
            cout<<"L-am mai gasit pe "<<x;
            return;
        }
        adauga_n_nod(nod, x, fiu_adaugat);
        if ( nod -> valori.size() >= 2 * NR_FII )
        {
            splituieste(nod);
        }
    }
    Nod* cauta_locul(Nod*& nod, T x)
    {
        if ( nod == NULL )
            return nod;
        int i = da_poz_min(nod, x);
        if ( i == -1 )
        {
            if ( nod -> fii[1] == NULL )
                return nod;
            return cauta_locul(nod -> fii[1], x);
        }
        if ( nod -> valori[i] == x )
            return NULL;
        if ( nod -> fii[i+2] == NULL && nod -> fii[i+1] == NULL )
            return nod;
        return cauta_locul(nod -> fii[i+2], x);
    }

    void sterge(Nod* nod, T x)
    {
        int poz = da_poz_min(nod, x);
        if ( nod -> sunt_frunza() )
        {
            if ( nod -> valori.size() >= NR_FII )
            {
                nod -> valori.erase(nod->valori.begin() + poz);
                nod -> nr_fii--;
                return;
            }
        }
    }






    void af(Nod* nod, int niv)
    {
        if ( nod == NULL )
            return;
        cout<<"Sunt pe nivelul "<<niv<<" cu valorile: ";
        afisazav(nod->valori);
        cout<<'\n';
        for ( int i = 1 ; i <= nod -> nr_fii ; i++ )
        {
            af(nod->fii[i], niv+1);
        }
    }
    void afiseaza()
    {
        af(this->root, 1);
    }
    void adauga(T x);
    bool gaseste(T x);
    void sterge(T x);
};
template<typename T>
bool B_Tree<T>::gaseste(T x)
{
    if ( cauta(this -> root, x) == NULL )
        return false;
    return true;
}
template<typename T>
void B_Tree<T>::adauga(T x)
{
    if ( this -> root == NULL )
    {
        this -> root = new Nod(x);
        return;
    }
    Nod * aux = cauta_locul(this -> root, x);
    if ( aux == NULL )
        return;

    incearca_adaugare(aux, x);
}
template<typename T>
void B_Tree<T>::sterge(T x)
{

}
int main()
{
    B_Tree<int> tree;
    for ( int i = 1 ; i <= 10000 ; i++ ){
        tree.adauga(i);
    }
    for ( int i = 1 ; i <= 25 ; i++ ){
        if ( i == 23 )
            tree.afiseaza();
        tree.sterge(i);
    }
    tree.afiseaza();
}
