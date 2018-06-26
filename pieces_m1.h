///////////////////////////////
////////// Constants //////////
///////////////////////////////
#define pwn 1
#define rk 3
#define bshp 4
#define knght 5
#define qn 7
#define kng 10

////////////////////////////////
////////// Base Class //////////
////////////////////////////////
class piece
{
protected:
    int value;
    int x, y;

public:
    int uid;
    int moves;
    int status;
    void input(int id, int val, int a, int b)
    {
        uid=id;
        value=val;
        x=a;
        y=b;
        moves=0;
        status=1;
        board[x][y]=val;
        boarduid[x][y]=id;
    }
    void mov(block b)
    {
        board[x][y]=0;
        boarduid[x][y]=0;
        last.muid=uid;
        last.i.x=x;
        last.i.y=y;
        if(last.i.flag==2)
        {
            last.i.flag=1;
        }
        else
        {
            last.i.flag=0;
            last.ruid=0;
        }
        x=b.x;
        y=b.y;
        remuid(boarduid[x][y]);
        board[x][y]=value;
        boarduid[x][y]=uid;
        moves++;
        fm++;
    }
    void rem()
    {
        board[x][y]=0;
        boarduid[x][y]=0;
        status=0;
        last.i.flag=2;
        last.ruid=uid;
        fm=0;
    }
    void revive()
    {
        board[x][y]=value;
        boarduid[x][y]=uid;
        status=1;
    }
};

/////////////////////////////////////
////////// Derived Classes //////////
/////////////////////////////////////

////////// Pawn //////////
class pawn: public piece
{
public:
    pawn(){}
    void movement(vector<block>& v);
    void mov(block b)
    {
        fm=0;
        piece::mov(b);
    }
};

void pawn::movement(vector<block>& v)
{
    if(status==0)
        return;
    int a=value/pwn, i;
    if(board[x+a][y]==0 && x+a>=0 && x+a<=7)
    {
        v.push_back(push(x+a, y, 1));
        if(moves==0)
        {
            if(board[x+2*a][y]==0)
                v.push_back(push(x+2*a, y, 1));
        }
    }
    for(i=-1; i<=1; i+=2)
        if(board[x+a][y+i]*a<0 && x+a>=0 && x+a<=7 && y+i>=0 && y+i<=7)
            v.push_back(push(x+a, y+i, 2));
}

////////// Rook //////////
class rook: public piece
{
public:
    rook(){}
    void movement(vector<block>& v);
};

void rook::movement(vector<block>& v)
{
    if(status==0)
        return;
    int i, j;
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(y+i*j>=0 && y+i*j<=7)
            {
                if(board[x][y+i*j]==0)
                    v.push_back(push(x, y+i*j, 1));
                else
                {
                    if(board[x][y+i*j]*value<0)
                        v.push_back(push(x, y+i*j, 2));
                    break;
                }
            }
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7)
            {
                if(board[x+i*j][y]==0)
                    v.push_back(push(x+i*j, y, 1));
                else
                {
                    if(board[x+i*j][y]*value<0)
                        v.push_back(push(x+i*j, y, 2));
                    break;
                }
            }
}

////////// Bishop //////////
class bishop: public piece
{
public:
    bishop(){}
    void movement(vector<block>& v);
};

void bishop::movement(vector<block>& v)
{
    if(status==0)
        return;
    int i, j;
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7 && y+i*j>=0 && y+i*j<=7)
            {
                if(board[x+i*j][y+i*j]==0)
                    v.push_back(push(x+i*j, y+i*j, 1));
                else
                {
                    if(board[x+i*j][y+i*j]*value<0)
                        v.push_back(push(x+i*j, y+i*j, 2));
                    break;
                }
            }
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7 && y-i*j>=0 && y-i*j<=7)
            {
                if(board[x+i*j][y-i*j]==0)
                    v.push_back(push(x+i*j, y-i*j, 1));
                else
                {
                    if(board[x+i*j][y-i*j]*value<0)
                        v.push_back(push(x+i*j, y-i*j, 2));
                    break;
                }
            }
}

////////// Knight //////////
class knight: public piece
{
public:
    knight(){}
    void movement(vector<block>& v);
};

void knight::movement(vector<block>& v)
{
    if(status==0)
        return;
    int i, j;
    for(i=-2; i<=2; i+=4)
        for(j=-1; j<=1; j+=2)
        {
            if(x+i>=0 && x+i<=7 && y+j>=0 && y+j<=7)
            {
                if(board[x+i][y+j]==0)
                    v.push_back(push(x+i, y+j, 1));
                else if(board[x+i][y+j]*value<0)
                    v.push_back(push(x+i, y+j, 2));
            }

            if(x+j>=0 && x+j<=7 && y+i>=0 && y+i<=7)
            {
                if(board[x+j][y+i]==0)
                    v.push_back(push(x+j, y+i, 1));
                else if(board[x+j][y+i]*value<0)
                    v.push_back(push(x+j, y+i, 2));
            }
        }
}

////////// Queen //////////
class queen: public piece
{
public:
    queen(){}
    void movement(vector<block>& v);
};

void queen::movement(vector<block>& v)
{
    if(status==0)
        return;
    int i, j;
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(y+i*j>=0 && y+i*j<=7)
            {
                if(board[x][y+i*j]==0)
                    v.push_back(push(x, y+i*j, 1));
                else
                {
                    if(board[x][y+i*j]*value<0)
                        v.push_back(push(x, y+i*j, 2));
                    break;
                }
            }
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7)
            {
                if(board[x+i*j][y]==0)
                    v.push_back(push(x+i*j, y, 1));
                else
                {
                    if(board[x+i*j][y]*value<0)
                        v.push_back(push(x+i*j, y, 2));
                    break;
                }
            }
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7 && y+i*j>=0 && y+i*j<=7)
            {
                if(board[x+i*j][y+i*j]==0)
                    v.push_back(push(x+i*j, y+i*j, 1));
                else
                {
                    if(board[x+i*j][y+i*j]*value<0)
                        v.push_back(push(x+i*j, y+i*j, 2));
                    break;
                }
            }
    for(i=-1; i<=1; i+=2)
        for(j=1; j<8; j++)
            if(x+i*j>=0 && x+i*j<=7 && y-i*j>=0 && y-i*j<=7)
            {
                if(board[x+i*j][y-i*j]==0)
                    v.push_back(push(x+i*j, y-i*j, 1));
                else
                {
                    if(board[x+i*j][y-i*j]*value<0)
                        v.push_back(push(x+i*j, y-i*j, 2));
                    break;
                }
            }
}

////////// King //////////
class king: public piece
{
public:
    king(){}
    void movement(vector<block>& v);
};

void king::movement(vector<block>& v)
{
    if(status==0)
        return;
    int i, j, c;
    block b;
    b.flag=1;
    c=value/kng;
    for(i=-1; i<=1; i++)
        for(j=-1; j<=1; j++)
            if(i!=0 || j!=0)
            {
                b.x=x+i;
                b.y=y+j;
                if(board[x+i][y+j]==0 && x+i>=0 && x+i<=7 && y+j>=0 && y+j<=7)
                    v.push_back(push(x+i, y+j, 1));
                else if(board[x+i][y+j]*value<0 && x+i>=0 && x+i<=7 && y+j>=0 && y+j<=7)
                    v.push_back(push(x+i, y+j, 2));
            }
}
