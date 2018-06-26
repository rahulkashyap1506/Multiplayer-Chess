#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;
int board[8][8], boarduid[8][8], win=0, fm=0;

////////////////////////////////
////////// Structures //////////
////////////////////////////////

struct block
{
    int x, y, flag;
};

block push(int x, int y, int flag)
{
    block t;
    t.x=x;
    t.y=y;
    t.flag=flag;
    return t;
}

struct lmv
{
    int muid;
    block i;
    int ruid;
}last;

//////////////////////////////////
////////// Declarations //////////
//////////////////////////////////

////////// Class //////////
class piece;
class pawn;
class rook;
class bishop;
class knight;
class queen;
class king;

////////// Piece //////////
vector<pawn> vpwn[2];
vector<rook> vrk[2];
vector<bishop> vbshp[2];
vector<knight> vknght[2];
vector<queen> vqn[2];
vector<king> vkng[2];

////////// Function //////////
void print(vector<block> v, int p=0);
block ploc(int uid);
void remuid(int uid);
void promote(int c);
int castling(int c, int y);
int en_passant(int c, int y);
int pleft();
int check(block b, int c);
int checkmate(int c, int stl=0);
void turn(int c);
void undo();

////////// Header File //////////
#include"pieces_m1.h"

///////////////////////////////
////////// Main Body //////////
///////////////////////////////

int main()
{
    int i, j, c, pstat, chk=0;
    vector<block> v;

    ////////// Piece Definitions //////////
    for(i=0; i<8; i++)
    {
        vpwn[0].push_back(pawn());
        vpwn[0][i].input(pwn*10+i+1, pwn, 1, i);
        vpwn[1].push_back(pawn());
        vpwn[1][i].input(-1*(pwn*10+i+1), -pwn, 6, i);
    }
    for(i=0; i<2; i++)
    {
        vrk[0].push_back(rook());
        vrk[0][i].input(rk*10+i+1, rk, 0, i*7);
        vrk[1].push_back(rook());
        vrk[1][i].input(-1*(rk*10+i+1), -rk, 7, i*7);

        vbshp[0].push_back(bishop());
        vbshp[0][i].input(bshp*10+i+1, bshp, 0, 2+i*3);
        vbshp[1].push_back(bishop());
        vbshp[1][i].input(-1*(bshp*10+i+1), -bshp, 7, 2+i*3);

        vknght[0].push_back(knight());
        vknght[0][i].input(knght*10+i+1, knght, 0, 1+i*5);
        vknght[1].push_back(knight());
        vknght[1][i].input(-1*(knght*10+i+1), -knght, 7, 1+i*5);
    }
    vqn[0].push_back(queen());
    vqn[0][0].input(qn*10+1, qn, 0, 3);
    vqn[1].push_back(queen());
    vqn[1][0].input(-1*(qn*10+1), -qn, 7, 3);

    vkng[0].push_back(king());
    vkng[0][0].input(kng*10+1, kng, 0, 4);
    vkng[1].push_back(king());
    vkng[1][0].input(-1*(kng*10+1), -kng, 7, 4);

    ////////// Game Loop //////////
    print(v);

    last.i.flag=1;
    c=0;
    do
    {
        turn(c);
        chk=checkmate(c);
        pstat=pleft();
        if(pstat==1)
        {
            cout<<"Insufficient Material\n";
            win=2;
        }
        if(fm==101)
        {
            print(v);
            cout<<"Fifty Move Rule\n";
            win=2;
        }
        if(chk==1)
        {
            cout<<"\n\nCheckmate\n";
            if(c==0)
                cout<<"Black WON";
            else if(c==1)
                cout<<"White WON";
        }
        if(win==1)
            cout<<"Resign";
        else if(win==2)
            cout<<"Draw";
        else if(win==3)
            cout<<"Stalemate";
        c=1-c;
    }while(chk==0 && win==0);
}

//////////////////////////////////////////
////////// Function Definitions //////////
//////////////////////////////////////////

////////// Print Board //////////
void print(vector<block> v, int p)
{
    int i, j, k, x[8][8];
    char ch, a[8][8];
    string str[2]={"\0", "\n\nIllegal Move"};
    system("cls");

    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
            x[i][j]=0;

    for(i=0; i<v.size(); i++)
        x[v[i].x][v[i].y]=v[i].flag;

    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
        {
            if(board[i][j]>0)
                a[i][j]='A';
            else if(board[i][j]<0)
                a[i][j]='a';
            else
                a[i][j]=' ';

            switch(abs(board[i][j]))
            {
                case pwn:
                    a[i][j]+=15;
                    break;
                case rk:
                    a[i][j]+=17;
                    break;
                case bshp:
                    a[i][j]+=1;
                    break;
                case knght:
                    a[i][j]+=13;
                    break;
                case qn:
                    a[i][j]+=16;
                    break;
                case kng:
                    a[i][j]+=10;
                    break;
            }
        }

    ch='a';
    cout<<"\n ";
    for(i=0; i<8; i++)
        cout<<"       "<<ch++<<" ";

    for(i=7; i>=0; i--)
    {
        cout<<"\n    ";
        for(j=0; j<=72; j++)
            cout<<"-";
        cout<<"\n    |";
        for(j=0; j<8; j++)
            cout<<"        |";
        cout<<"\n  "<<i+1<<" |";
        for(j=0; j<8; j++)
        {
            if(x[i][j]==1)
                cout<<"   __   |";
            else if(x[i][j]==2)
                cout<<"   "<<a[i][j]<<abs(boarduid[i][j]%10)<<"   #";
            else if(a[i][j]!=' ')
                cout<<"   "<<a[i][j]<<abs(boarduid[i][j]%10)<<"   |";
            else
                cout<<"        |";
        }
        cout<<"  "<<i+1<<"\n    |";
        for(j=0; j<8; j++)
            cout<<"        |";
    }
    cout<<"\n    ";
    for(j=0; j<=72; j++)
        cout<<"-";
    ch='a';
    cout<<"\n ";
    for(i=0; i<8; i++)
        cout<<"       "<<ch++<<" ";
    cout<<str[p];
}

////////// UID -> Location(block) //////////
block ploc(int uid)
{
    int i, j;
    block b;
    for(i=0; i<8; i++)
        for(j=0; j<8; j++)
            if(boarduid[i][j]==uid)
            {
                b=push(i, j, 1);
                return b;
            }
}

////////// Remove Piece //////////
void remuid(int uid)
{
    int v, i, c;
    if(uid==0)
        return;
    v=abs(uid/10);
    i=abs(uid%10)-1;
    c=(uid>0)?0:1;
    switch(v)
    {
    case pwn:
        vpwn[c][i].rem();
        break;
    case rk:
        vrk[c][i].rem();
        break;
    case bshp:
        vbshp[c][i].rem();
        break;
    case knght:
        vknght[1][i].rem();
        break;
    case qn:
        vqn[c][i].rem();
        break;
    }
}

////////// Pawn Promotion //////////
void promote(int c)
{
    int i, flag=0, choice, t;
    block b;
    for(i=0; i<vpwn[c].size(); i++)
    {
        b=ploc(vpwn[c][i].uid);
        if(b.x==(1-c)*7)
        {
            flag=1;
            break;
        }
    }

    if(flag==1)
    {
        cout<<"\nPawn no."<<i+1<<" can now be promoted. Options -"
            <<"\n 1. Rook"
            <<"\n 2. Knight"
            <<"\n 3. Bishop"
            <<"\n 4. Queen"
            <<"\n\nChoice - ";
        cin>>choice;
        vpwn[c][i].rem();
        t=(c==0)?1:-1;
        switch(choice)
        {
        case 1:
            i=vrk[c].size();
            vrk[c].push_back(rook());
            vrk[c][i].input(t*(rk*10+i+1), t*rk, b.x, b.y);
            break;
        case 2:
            i=vrk[c].size();
            vknght[c].push_back(knight());
            vknght[c][i].input(t*(knght*10+i+1), t*knght, b.x, b.y);
            break;
        case 3:
            i=vbshp[c].size();
            vbshp[c].push_back(bishop());
            vbshp[c][i].input(t*(bshp*10+i+1), t*bshp, b.x, b.y);
            break;
        case 4:
            i=vqn[c].size();
            vqn[c].push_back(queen());
            vqn[c][i].input(t*(qn*10+i+1), t*qn, b.x, b.y);
            break;
        }
    }
}

////////// Castling //////////
int castling(int c, int y)
{
    int i, j, t, chk=0;
    block a, b;

    if(vkng[c][0].moves==0 && vrk[c][y].moves==0)
    {
        b=ploc(vrk[c][y].uid);
        if(board[c*7][1]==0 && board[c*7][2]==0 && board[c*7][3]==0 && y==0)
        {
            chk=0;
            for(j=2; j<=4; j++)
            {
                a.x=c*7;
                a.y=j;
                chk=check(a, c);
            }
            if(chk==0)
            {
                a.y=2;
                vkng[c][0].mov(a);
                a.y=3;
                vrk[c][0].mov(a);
                last.muid=vkng[c][0].uid;
                return 1;
            }
        }

        if(board[c*7][5]==0 && board[c*7][6]==0 && y==1)
        {
            chk=0;
            for(j=4; j<=6; j++)
            {
                a.x=c*7;
                a.y=j;
                chk=check(a, c);
            }
            if(chk==0)
            {
                a.y=6;
                vkng[c][0].mov(a);
                a.y=5;
                vrk[c][1].mov(a);
                last.muid=vkng[c][0].uid;
                return 1;
            }
        }
    }
    return 0;
}

////////// En passant //////////
int en_passant(int c, int y)
{
    int t;
    block l, b;
    if(y>vpwn[c].size())
        return 0;
    t=(c==0)?1:-1;
    l=ploc(last.muid);
    b=ploc(vpwn[c][y].uid);
    if(abs(last.muid/10)==pwn && l.x==(4-c) && vpwn[1-c][abs(last.muid%10)-1].moves==1 && b.x==(4-c))
    {
        vpwn[1-c][abs(last.muid%10)-1].rem();
        l.x+=t;
        vpwn[c][y].mov(l);
        return 1;
    }
    return 0;
}

////////// Pieces Left //////////
int pleft()
{
    int p[2], r[2], b[2], n[2], q[2], i, total=0;

    for(i=0; i<2; i++)
    {
        p[i]=vpwn[i].size();
        r[i]=vrk[i].size();
        b[i]=vbshp[i].size();
        n[i]=vknght[i].size();
        q[i]=vqn[i].size();
        total+=p[i]+r[i]+b[i]+n[i]+q[i];
    }

    switch(total)
    {
    case 0:
        return 1;
    case 1:
        if(b[0]==1 || b[1]==1 || n[0]==1 || n[1]==1)
            return 1;
        break;
    case 2:
        if(b[0]==1 && b[1]==1)
        {
            block B[2];
            for(i=0; i<2; i++)
            {
                B[i]=ploc(vbshp[i][0].uid);
                b[0]=B[i].x+B[i].y;
            }
            if((b[0]%2==0 && b[1]%2==0) || (b[0]%2!=0 && b[1]%2!=0))
                return 1;
        }
        break;
    }
    return 0;
}

////////// Check //////////
int check(block b, int c)
{
    vector<block> t;
    int i, j, chk=0;

    for(i=0; i<vpwn[1-c].size(); i++)
        vpwn[1-c][i].movement(t);
    for(i=0; i<vrk[1-c].size(); i++)
        vrk[1-c][i].movement(t);
    for(i=0; i<vbshp[1-c].size(); i++)
        vbshp[1-c][i].movement(t);
    for(i=0; i<vknght[1-c].size(); i++)
        vknght[1-c][i].movement(t);
    for(i=0; i<vqn[1-c].size(); i++)
        vqn[1-c][i].movement(t);
    vkng[1-c][0].movement(t);

    for(i=0; i<t.size(); i++)
        if(b.x==t[i].x && b.y==t[i].y)
            return 1;

    return 0;
}

////////// Checkmate //////////
int checkmate(int c, int stl)
{
    int i, j, t, chk, total;
    block a, b;
    vector<block> v;

    t=(c==0)?1:-1;
    a=ploc(t*(kng*10+1));
    chk=check(a, c);
    if(chk==0 && stl==0)
        return 0;

    chk=0;
    total=0;
    for(i=0; i<vpwn[c].size(); i++)
    {
        v.clear();
        vpwn[c][i].movement(v);
        for(j=0; j<v.size(); j++)
        {
            vpwn[c][i].mov(v[j]);
            chk+=check(a, c);
            undo();
        }
        total+=j;
    }
    for(i=0; i<vrk[c].size(); i++)
    {
        v.clear();
        vrk[c][i].movement(v);
        for(j=0; j<v.size(); j++)
        {
            vrk[c][i].mov(v[j]);
            chk+=check(a, c);
            undo();
        }
        total+=j;
    }
    for(i=0; i<vbshp[c].size(); i++)
    {
        v.clear();
        vbshp[c][i].movement(v);
        for(j=0; j<v.size(); j++)
        {
            vbshp[c][i].mov(v[j]);
            chk+=check(a, c);
            undo();
        }
        total+=j;
    }
    for(i=0; i<vknght[c].size(); i++)
    {
        v.clear();
        vknght[c][i].movement(v);
        for(j=0; j<v.size(); j++)
        {
            vknght[c][i].mov(v[j]);
            chk+=check(a, c);
            undo();
        }
        total+=j;
    }
    for(i=0; i<vqn[c].size(); i++)
    {
        v.clear();
        vqn[c][i].movement(v);
        for(j=0; j<v.size(); j++)
        {
            vqn[c][i].mov(v[j]);
            chk+=check(a, c);
            undo();
        }
        total+=j;
    }
    v.clear();
    vkng[c][0].movement(v);
    for(i=0; i<v.size(); i++)
    {
        vkng[c][0].mov(v[i]);
        chk+=check(ploc(vkng[c][0].uid), c);
        undo();
    }
    total+=i;

    if(chk==total)
        return 1;

    return 0;
}

////////// Player Turn //////////
void turn(int c)
{
    vector<block> v;
    block b;
    int uid, i, flag, stl, chk, p=0;
    char ch[2][2];

    if(checkmate(c)==1)
    {
        print(v);
        return;
    }
    chk=check(ploc(vkng[c][0].uid), c);

    loop_0:
    v.clear();
    print(v, p);
    p=0;
    if(chk==1)
        cout<<"\n\nCHECK";
    if(c==0)
        cout<<"\n\nWhite - ";
    else if(c==1)
        cout<<"\n\nBlack - ";
    cin>>ch[0];

    loop_1:
    i=(int)ch[0][1]-49;
    switch(int(ch[0][0]))
    {
    case 'p':
        vpwn[c][i].movement(v);
        break;
    case 'r':
        vrk[c][i].movement(v);
        break;
    case 'b':
        vbshp[c][i].movement(v);
        break;
    case 'n':
        vknght[c][i].movement(v);
        break;
    case 'q':
        vqn[c][i].movement(v);
        break;
    case 'k':
        vkng[c][i].movement(v);
        break;
    case 'c':
        flag=castling(c, i);
        if(flag==1)
            return;
        else if(flag==0)
            goto loop_0;
        break;
    case 'e':
        flag=0;
        if(i<=vpwn[c].size())
            flag=en_passant(c, i);
        if(flag==1)
            return;
        else if(flag==0)
            goto loop_0;
    case 'u':
        undo();
        return;
    case 'R':
        win=1;
        return;
    case 'D':
        win=2;
        return;
    default:
        v.clear();
        print(v);
        if(c==0)
            cout<<"\n\nWhite - ";
        else if(c==1)
            cout<<"\n\nBlack - ";
        cin>>ch[0];
        goto loop_1;
    }

    print(v, p);
    if(c==0)
        cout<<"\n\nWhite - ";
    else if(c==1)
        cout<<"\n\nBlack - ";
    cin>>ch[1];

    if(ch[1][0]!='t' || v.size()==0)
    {
        v.clear();
        ch[0][0]=ch[1][0];
        ch[0][1]=ch[1][1];
        goto loop_1;
    }

    loop_2:
    print(v);
    if(chk==1)
        cout<<"\n\nCHECK";
    cout<<"\n\nTo - ";
    cin>>ch[1];

    b.x=(int)ch[1][1]-49;
    b.y=(int)ch[1][0]-97;

    flag=0;
    for(i=0; i<v.size(); i++)
        if(b.x==v[i].x && b.y==v[i].y)
        {
            b=v[i];
            flag=1;
            break;
        }

    if(flag==0)
        goto loop_2;

    i=(int)ch[0][1]-49;
    switch(int(ch[0][0]))
    {
    case 'p':
        vpwn[c][i].mov(b);
        promote(c);
        break;
    case 'r':
        vrk[c][i].mov(b);
        break;
    case 'b':
        vbshp[c][i].mov(b);
        break;
    case 'n':
        vknght[c][i].mov(b);
        break;
    case 'q':
        vqn[c][i].mov(b);
        break;
    case 'k':
        vkng[c][i].mov(b);
        break;
    }
    v.clear();
    chk=check(ploc(vkng[c][0].uid), c);
    if(chk==1)
    {
        p=1;
        undo();
        chk=check(ploc(vkng[c][0].uid), c);
        stl=0;
        stl=checkmate(c, 1);
        if(chk==0 && stl==1)
        {
            win=3;
            return;
        }
        goto loop_1;
    }
}

////////// Undo Move //////////
void undo()
{
    int c, i;

    c=(last.muid>0)?0:1;
    i=abs(last.muid%10)-1;

    switch(abs(last.muid/10))
    {
    case pwn:
        vpwn[c][i].mov(last.i);
        vpwn[c][i].moves-=2;
        break;
    case rk:
        vrk[c][i].mov(last.i);
        vrk[c][i].moves-=2;
        break;
    case bshp:
        vbshp[c][i].mov(last.i);
        vbshp[c][i].moves-=2;
        break;
    case knght:
        vknght[c][i].mov(last.i);
        vknght[c][i].moves-=2;
        break;
    case qn:
        vqn[c][i].mov(last.i);
        vqn[c][i].moves-=2;
        break;
    case kng:
        vkng[c][i].mov(last.i);
        vkng[c][i].moves-=2;
        break;
    }

    if(last.i.flag==1)
    {
        i=abs(last.ruid%10)-1;

        switch(abs(last.ruid/10))
        {
        case pwn:
            vpwn[1-c][i].revive();
            break;
        case rk:
            vrk[1-c][i].revive();
            break;
        case bshp:
            vbshp[1-c][i].revive();
            break;
        case knght:
            vknght[1-c][i].revive();
            break;
        case qn:
            vqn[1-c][i].revive();
            break;
        case kng:
            vkng[1-c][i].revive();
        }
    }
}
