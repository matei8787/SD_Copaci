#include<bits/stdc++.h>
using namespace std;

template<typename T>
class Splay_Tree
{
    struct Nod
    {
        T val;
        Nod* st, *dr, *tata;
        Nod()
        {
            st = dr = tata = NULL;
        }
        Nod(T val)
        {
            this -> val = val;
            st = dr = tata = NULL;
        }
        Nod(Nod* nod)
        {
            if ( nod == NULL )
            {
                Nod();
                return;
            }
            val = nod -> val;
            st = nod -> st;
            dr = nod -> dr;
            tata = nod -> tata;
        }
        void afisare_tot()
        {
            cout<<"am valoarea "<<val<<"\nFiul din stanga are valoarea: "<<(st!=NULL?st->val:0)<<"\nFiul din dreapta are valoarea: "<<(dr!=NULL?dr->val:0)<<"\nIar tatal meu este: "<<(tata!=NULL?tata->val:0)<<"\n\n";
        }
        void afis()
        {
            afisare_tot();
            if ( tata )
                tata -> afisare_tot();
            if ( st )
                st -> afisare_tot();
            if ( dr )
                dr -> afisare_tot();
        }
    };
    Nod* root;
public:
    Splay_Tree()
    {
        root = NULL;
    }
    Splay_Tree(T val)
    {
        root = new Nod(val);
    }
    bool e_fiu_st(Nod* tata, Nod* fiu)
    {
        if ( tata -> st == fiu )
            return true;
        return false;
    }
    Nod* get_root()
    {
        return root;
    }
    Nod* gaseste(Nod* nod, T x)
    {
        if ( nod -> val == x )
            return nod;
        if ( nod -> val < x )
        {
            if ( nod -> dr == NULL )
                return nod;
            return gaseste(nod -> dr, x);
        }
        if ( nod -> st == NULL )
            return nod;
        return gaseste(nod -> st, x);
    }
    void roteste_st(Nod* tata, Nod* fiu)
    {
        tata -> st = fiu -> dr;
        if ( fiu -> dr )
            fiu -> dr -> tata = tata;
        fiu -> tata = tata -> tata;
        if ( fiu -> tata != NULL )
        {
            if ( e_fiu_st(tata->tata, tata) )
            {
                tata->tata->st = fiu;
            }
            else
            {
                tata -> tata -> dr = fiu;
            }
        }
        tata -> tata = fiu;
        fiu -> dr = tata;
    }
    void roteste_dr(Nod* tata, Nod* fiu)
    {
        tata -> dr = fiu -> st;
        if ( fiu -> st )
            fiu -> st -> tata = tata;
        fiu -> tata = tata -> tata;
        if ( fiu -> tata != NULL )
        {
            if ( e_fiu_st(tata->tata, tata) )
            {
                tata -> tata -> st = fiu;
            }
            else
            {
                tata -> tata -> dr = fiu;
            }
        }
        tata -> tata = fiu;
        fiu -> st = tata;
    }
    void roteste(Nod* tata, Nod* fiu)
    {
        if ( tata -> st != NULL && tata -> st == fiu )
            roteste_st(tata, fiu);
        else if ( tata -> dr != NULL )
            roteste_dr(tata, fiu);
    }
    void zig(Nod* nod)
    {
        roteste(nod->tata, nod);
    }
    void zigzag(Nod* nod)
    {
        roteste(nod->tata, nod);
        roteste(nod->tata, nod);
    }
    void zigzig(Nod* nod)
    {
        roteste(nod->tata->tata, nod->tata);
        roteste(nod->tata, nod);
    }
    void splay(Nod* nod)
    {
        if ( nod -> tata == NULL )
            return;
        if ( nod -> tata -> tata == NULL )
        {
            zig(nod);
            splay(nod);
            return;
        }
        int cat_st = 0;
        if ( e_fiu_st(nod->tata->tata, nod->tata) )
            cat_st++;
        if ( e_fiu_st(nod->tata, nod) )
            cat_st++;
        if ( cat_st % 2 == 0 )
            zigzig(nod);
        else
            zigzag(nod);
        splay(nod);
    }
    void adauga(Nod*& nod, Nod* last, T x)
    {
        if ( nod == NULL )
        {
            nod = new Nod(x);
            if ( last != NULL )
                nod -> tata = last;
            return;
        }
        if ( nod -> val == x )
            return;
        if ( nod -> val > x )
            adauga(nod->st, nod, x);
        else
            adauga(nod->dr, nod, x);

    }
    void afisare(Nod* nod, int niv)
    {
        if ( nod == NULL )
            return;
        cout<<"Sunt pe nivelul "<<niv<<" si ";
        nod->afisare_tot();
        afisare(nod->st, niv+1);
        afisare(nod->dr, niv+1);
    }
    Nod* cauta_nod_min(Nod* nod)
    {
        if ( nod -> st )
            return cauta_nod_min(nod->st);
        return nod;
    }
    Nod* cauta_nod_max(Nod* nod)
    {
        if ( nod -> dr )
            return cauta_nod_min(nod->dr);
        return nod;
    }
    void afisare()
    {

        afisare(this->root, 1);
    }
    void adauga(T x);
    void sterge(T x);
    bool gaseste(T x);
};
template<typename T>
void Splay_Tree<T>::adauga(T x)
{
    adauga(this -> root, this->root, x);
}
template<typename T>
bool Splay_Tree<T>::gaseste(T x)
{
    Nod* aux = gaseste(this->root, x);
    splay(aux);
    this -> root = aux;
    if ( aux -> val == x )
        return true;
    return false;
}
template<typename T>
void Splay_Tree<T>::sterge(T x)
{
    Nod* aux = gaseste(this -> root, x);
    if ( aux -> val != x )
        return;
    Nod* schimb = NULL;
    if ( aux -> st )
    {
        schimb = cauta_nod_max(aux->st);
    }
    else if ( aux -> dr )
    {
        schimb = cauta_nod_min(aux->dr);
    }
    Nod* tatal = aux -> tata;
    if ( schimb == NULL )
    {
        if ( tatal -> st == aux )
            tatal -> st = NULL;
        else
            tatal -> dr = NULL;
        delete aux;
    }
    else
    {
        swap(aux->val, schimb->val);
        if ( schimb -> tata -> st == schimb )
            schimb -> tata -> st = NULL;
        else
            schimb -> tata -> dr = NULL;
        delete schimb;
    }
    if ( tatal )
        splay(tatal);
}
int main()
{
    Splay_Tree<int> tree(5);
    tree.adauga(2);
    tree.adauga(4);
    tree.adauga(17);
    tree.adauga(1);
    tree.adauga(7);
    tree.adauga(10);
    int op, x;
    while ( cin>>op>>x )
    {
        if ( op == 1 )
        {
            tree.adauga(x);
        }
        else if ( op == 2 )
        {
            if ( tree.gaseste(x) )
                cout<<"L-am gasit\n";
            else
                cout<<"NU\n";
        }
        else
        {
            tree.sterge(x);
        }
    }
    return 0;

}
