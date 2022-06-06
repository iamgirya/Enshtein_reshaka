
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
using namespace std;
int n = 5;/////////////////////////////

struct komponenta;

struct svoistvo
{
    int level;
    string imya;
    int smesh = 0;
    komponenta* numkom;
};

struct komponenta
{
    vector <svoistvo*> yach;
    int shir = 0;
};



vector <svoistvo*> all;
vector < tuple<string, char, string>> uslovia;
vector < tuple<string, char, string>> vstavka;




svoistvo* getsvoim(string a)
{
    for (auto i : all)
        if (i->imya == a) return i;
    cout << "error";
}
komponenta* getkomsv(svoistvo* a)
{
    return a->numkom;
}




bool mojvstav(komponenta* th, vector <vector<string>>& tab, int i)
{
    for (auto u : th->yach)
    {
        if (tab[u->smesh + i][u->level] != "")
            return false;
    }
    return true;
}
void vstav(komponenta* th, vector <vector<string>>& tab, int i, vector <komponenta*>& kuski)
{
    for (auto u : th->yach)
    {
        tab[u->smesh + i][u->level] = u->imya;
    }
    for (auto i = kuski.begin(); i != kuski.end(); i++)
    {
        if (*i == th)
        {
            kuski.erase(i);
            break;
        }
    }
}
void slepit(komponenta* one, komponenta* two, vector <komponenta*> &kuski)
{
    for (auto i : two->yach)
    {
        one->yach.push_back(i);
        i->numkom = one;
    }
    int max = 0;
    for (auto i : one->yach)
    {
        if (i->smesh > max)
            max = i->smesh;
    }
    one->shir = max;
    
    for (auto i = kuski.begin(); i != kuski.end(); i++)
    {
        if (*i == two)
        {
            kuski.erase(i);
            break;
        }
    }
}

void ispuslov(tuple<string, char, string>& ysl, vector <komponenta*> &kuski)
{
    if (get<1>(ysl) == '^')
    {
        svoistvo* lsvo = getsvoim(get<0>(ysl));
        svoistvo* rsvo = getsvoim(get<2>(ysl));
        komponenta* lkom = getkomsv(getsvoim(get<0>(ysl)));
        komponenta* rkom = getkomsv(getsvoim(get<2>(ysl)));
        int lsm = lsvo->smesh;
        int rsm = rsvo->smesh;

        if (lsm < rsvo->smesh)
        {
            for (auto i : lkom->yach)
            {
                i->smesh += rsm - lsm;
            }

        }
        else if (lsm >= rsm)
        {
            for (auto i : rkom->yach)
            {
                i->smesh += lsm - rsm;
            }

        }
        slepit(lkom, rkom, kuski);
    }
    else if (get<1>(ysl) == '>')
    {
        svoistvo* lsvo = getsvoim(get<0>(ysl));
        svoistvo* rsvo = getsvoim(get<2>(ysl));
        komponenta* lkom = getkomsv(getsvoim(get<0>(ysl)));
        komponenta* rkom = getkomsv(getsvoim(get<2>(ysl)));
        int lsm = lsvo->smesh;
        int rsm = rsvo->smesh;

        if (lsm < rsm -1)
        {
            for (auto i : lkom->yach)
            {
                i->smesh += rsm -1 - lsm;
            }

        }
        else if (lsm >= rsm -1)
        {
            for (auto i : rkom->yach)
            {
                i->smesh += lsm - rsm +1;
            }

        }
        slepit(lkom, rkom, kuski);
    }
    else if (get<1>(ysl) == '<')
    {
        svoistvo* lsvo = getsvoim(get<0>(ysl));
        svoistvo* rsvo = getsvoim(get<2>(ysl));
        komponenta* lkom = getkomsv(getsvoim(get<0>(ysl)));
        komponenta* rkom = getkomsv(getsvoim(get<2>(ysl)));
        int lsm = lsvo->smesh;
        int rsm = rsvo->smesh;

        if (lsm + 1 < rsm)
        {
            for (auto i : lkom->yach)
            {
                i->smesh += rsm - 1 - lsm;
            }

        }
        else if (lsm + 1 >= rsm)
        {
            for (auto i : rkom->yach)
            {
                i->smesh += lsm - rsm + 1;
            }

        }
        slepit(lkom, rkom, kuski);
    }
    else if (get<1>(ysl) == '=')
    {
        vstavka.push_back(ysl);
    }
    else
    {
        cout << "error";
    }
}

void doispuslov(tuple<string, char, string>& ysl, vector <vector<string>>& tab, vector <komponenta*>& kuski)
{
    svoistvo* svo = getsvoim(get<0>(ysl));
    komponenta* kom = getkomsv(getsvoim(get<0>(ysl)));
    int sme = stoi(get<2>(ysl))-1 - svo->smesh;
    vstav(kom, tab, sme, kuski);
}


int perebor(vector <vector<string>>& tab, vector <komponenta*>& kuski)
{
    int vstavleno = 0;

    
    for (int i = 0; i < kuski.size(); i++)
    {
        int kol = 0, koord;
        for (int j = 0; j < n - kuski[i]->shir; j++)
        {
            if (mojvstav(kuski[i], tab, j))
            {
                koord = j;
                kol++;
            }
        }
        if (kol == 1)
        {
            vstav(kuski[i], tab, koord, kuski);
            i--;
            vstavleno++;
        }
        else if (kol == 0)
        {
            vstavleno = -1;
            goto m;
        }
    }

    m:
    return vstavleno;
}






void vvodsv(vector <komponenta*> &kuski)
{
    ifstream i("sv.txt");
    while (!i.eof())
    {
        komponenta* k1 = new komponenta;
        svoistvo* s1 = new svoistvo;
        getline(i, s1->imya);
        string g;
        getline(i , g);
        s1->level = stoi(g)-1;
        s1->smesh = 0;
        s1->numkom = k1;

        k1->shir = 0;
        k1->yach.push_back(s1);

        all.push_back(s1);
        kuski.push_back(k1);
    }
    i.close();
}
void vvodus()
{
    ifstream i("us.txt");
    while (!i.eof())
    {
        string g, s1, s3;
        char s2;
        getline(i, g);
        for (int i = 0; i < g.size(); i++)
        {
            if (g[i] == ' ')
            {
                s1 = g.substr(0, i);
                s2 = g[i + 1];
                s3 = g.substr(i + 3, g.size() - i - 3);
                break;
            }
        }
        uslovia.push_back(make_tuple(s1, s2, s3));
    }
}


void work(vector <vector<string>> tab, vector <komponenta*> kuski, komponenta* th, int i)
{
    vstav(th, tab, i, kuski);
    while (true)
    {
        int k = perebor(tab, kuski);
        if (k == 0)
            break;
        else if (k < 0)
        {
            return;
        }
    }
    if (kuski.size() == 0)
    {
        for (auto i : tab)
        {
            for (auto j : i)
            {
                cout << j << "  ";
            }
            cout << endl;
        }
        cout << endl << "Win";
        return;
    }
    else
    {
        for (int j = 0; j < n - kuski[0]->shir; j++)
        {
            if (mojvstav(kuski[0], tab, j))
            {
                work(tab, kuski, kuski[0], j);
            }
        }
    }

}


void work(vector <vector<string>> tab, vector <komponenta*> kuski)
{
    while (true)
    {
        int k = perebor(tab, kuski);
        if (k == 0)
            break;
        else if (k < 0)
        {
            return;
        }
    }
    if (kuski.size() == 0)
    {
        for (auto i : tab)
        {
            for (auto j : i)
            {
                cout << j << "  ";
            }
            cout << endl;
        }
        cout << endl << "Win";
        return;
    }
    else
    {
        for (int j = 0; j < n - kuski[0]->shir; j++)
        {
            if (mojvstav(kuski[0], tab, j))
            {
                work(tab, kuski, kuski[0], j);
            }
        }
    }

}




int main()
{
    setlocale(LC_ALL, "Russian");
    vector <komponenta*> kuski;
    vector <vector<string>> tabl;
    tabl.resize(n);
    for (int i = 0; i < n; i++)
    {
        tabl[i].resize(n, "");
    }

    vvodsv(kuski);
    vvodus();
    for (auto i : uslovia)
    {
        ispuslov(i, kuski);
    }
    for (auto i : vstavka)
    {
        doispuslov(i, tabl, kuski);
    }
    work(tabl, kuski);
    

    cout << 1;
}
