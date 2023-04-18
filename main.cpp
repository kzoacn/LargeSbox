#include <cmath>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<map> 
#include "bitbasis.hpp" 
using namespace std;

#define RAIN 
//#define AIM

#ifdef RAIN

    #define N 128

    #if N==128
        #define LOGD 16
    #elif N==192
        #define LOGD 16
    #elif N==256
        #define LOGD 16
    #elif N==8
        #define LOGD 2
    #endif

    #define T (2*LOGD)
    #define FREE_VARS (2*LOGD)
    #define QUAD_VARS (FREE_VARS+FREE_VARS*(FREE_VARS-1)/2)
#endif

#ifdef AIM
    #define N 192
    #define D 45
    #define T 12
    #define FREE_VARS 12
    #define QUAD_VARS (FREE_VARS+FREE_VARS*(FREE_VARS-1)/2)
#endif


const int n=N; 
vector<int> irr_poly;


void set_parameters(){
    irr_poly.resize(n+1); 

#if N==128
    irr_poly[n]=1;
    irr_poly[7]=1;
    irr_poly[2]=1;
    irr_poly[1]=1;
    irr_poly[0]=1;
 

#elif N==192

    irr_poly[n]=1;
    irr_poly[7]=1;
    irr_poly[2]=1;
    irr_poly[1]=1;
    irr_poly[0]=1;
 
#elif N==256
    irr_poly[n]=1;
    irr_poly[10]=1;
    irr_poly[5]=1;
    irr_poly[2]=1;
    irr_poly[0]=1;
     
#elif N==8
    irr_poly[n]=1;
    irr_poly[4]=1;
    irr_poly[3]=1;
    irr_poly[1]=1;
    irr_poly[0]=1;
#endif

}



struct Term{
    int var1,var2;
    Term(int a){
        var1=var2=a;
    }
    Term(int a,int b){
        var1=a;
        var2=b;
    }
    bool operator==(const Term& t)const{
        return var1==t.var1 && var2==t.var2;
    }
    bool operator<(const Term& t)const{
        if(var1!=t.var1)return var1<t.var1;
        return var2<t.var2;
    }
    Term operator*(const Term& oth)const{
            vector<int>vars;
            vars.push_back(var1);
            vars.push_back(var2);
            vars.push_back(oth.var1);
            vars.push_back(oth.var2);
            //sort and unique
            sort(vars.begin(),vars.end());
            vars.erase(unique(vars.begin(),vars.end()),vars.end());
            if(vars.size()==1)
                return Term(vars[0],vars[0]);
            if(vars.size()==2)
                return Term(vars[0],vars[1]);
            puts("error");
            throw;
            return Term(-1,-1);
    }
};

string to_str(int x){
    string s;
    if(x==0)return "0";
    while(x){
        s+=char('0'+x%10);
        x/=10;
    }
    reverse(s.begin(),s.end());
    return s;
}

string name(int id){
    if(id<n)
        return string("x")+to_str(id);
    return string("y")+to_str(id-n);
}



struct Expression{
    vector<Term> terms;
    int constant;
    // exp= \sum terms + constant
    Expression(){
        constant=0;
    }
    Expression(int c){
        constant=c;
    }
    Expression(Term t){
        terms.push_back(t);
        constant=0;
    }

    void print(){
        sort(terms.begin(),terms.end());
        vector<string>text;
        for(auto term :terms){
            if(term.var1==term.var2){
                text.push_back(name(term.var1));
            }else{
                text.push_back(name(term.var1)+name(term.var2));
            }
        }
        if(constant){
            text.push_back("1");
        }
        for(int i=0;i<text.size();i++){
            if(i)cout<<" + ";
            cout<<text[i];
        }
        cout<<endl;
    }

    void add_term(Term term){ 
        for(int i=0;i<terms.size();i++){
            if(terms[i]==term){
                terms.erase(terms.begin()+i);
                return;
            }
        }
        terms.push_back(term);
    }

    void add_terms(const vector<Term>&oth_terms){
        // if v contains pr , remove it
        // else add it
        set<Term>S;
        for(auto pr : terms){
            if(S.count(pr)){
                S.erase(pr);
            }else{
                S.insert(pr);
            }
        }
        for(auto pr : oth_terms){
            if(S.count(pr)){
                S.erase(pr);
            }else{
                S.insert(pr);
            }
        }
        terms.clear();
        for(auto pr : S){
            terms.push_back(pr);
        }
    }

    void simplify(){
        set<Term>S;
        for(auto pr : terms){
            if(S.count(pr)){
                S.erase(pr);
            }else{
                S.insert(pr);
            }
        }
        terms.clear();
        for(auto pr : S){
            terms.push_back(pr);
        }
    }

    Expression operator+(const Expression& oth)const{
        Expression res=*this;
        res.add_terms(oth.terms);
        res.constant=constant^oth.constant;
        return res;
    } 
    
    Expression operator*(const Expression& oth)const{
        Expression res;
        for(auto t1: terms){
            for(auto t2: oth.terms){
                res.terms.push_back(t1*t2); 
            }
        }
        if(constant)
            res.add_terms(oth.terms);
        
        if(oth.constant)
            res.add_terms(terms);

        res.simplify();

        res.constant=constant&oth.constant;
        return res;
    }

    /*Expression subs(vector<Expression> repr)const{
        Expression res;
        for(auto term : terms){
            if(term.var1==term.var2)
                res=res+repr[term.var1];
            else
                res=res+repr[term.var1]*repr[term.var2];
        }
        if(constant){
            res.constant^=1;
        }
        return res;
    }*/
};

typedef vector<Expression> Poly;


vector<bitset<n> >rand_mat(){// ensure that the matrix is invertible
    int r=0;
    vector<bitset<n> >vec;
    BitBasis<n> basis;
    while(r<n){
        bitset<n>bs;
        for(int i=0;i<n;i++)
            bs[i]=rand()%2;
        if(basis.insert(bs)){
            vec.push_back(bs);
            r++;
        }
    }
    return vec;
}

Poly multiply_matrix(Poly a,vector<bitset<n> >mat){
    Poly b;
    b.resize(a.size());
    for(int i=0;i<a.size();i++){
        for(int j=0;j<n;j++){
            if(mat[j][i]){
                b[j]=b[j]+a[i];
            }
        }
    }
    return b;
}


Poly multiply(Poly a,Poly b){
    Poly c;
    c.resize(a.size()+b.size()-1);

    for(int i=0;i<a.size();i++){
        for(int j=0;j<b.size();j++){
            Expression exp=a[i]*b[j];
            //add_pairs(c[i+j],terms);
            for(auto term : exp.terms)
                c[i+j].terms.push_back(term);
            c[i+j].constant^=exp.constant;
        }
    } 

    for(auto &cc:c)
        cc.simplify();

    // mod c by irr_poly
    for(int i=(int)c.size()-1;i>=n;i--){
        for(int j=0;j<n;j++)if(irr_poly[j]){
            //add_pairs(c[i-(n-j)],c[i]);
            for(auto term : c[i].terms)
                c[i-(n-j)].terms.push_back(term);
            c[i-(n-j)].constant^=c[i].constant;
        }
    }
    c.resize(n);
    for(auto &cc:c)
        cc.simplify();
    
    return c;
}



Poly add(Poly a,Poly b){
    Poly c;
    c.resize(a.size());
    for(int i=0;i<a.size();i++){
        c[i]=a[i]+b[i];
    }
    
    return c;
}

Poly square(Poly a){
    //return multiply(a,a);
    static Poly sq;
    static int done=0;
    if(!done){
        done=1;
        sq.resize(n);
        for(int i=0;i<n;i++)
            sq[i].terms.push_back(Term(i,i)); 
        sq=multiply(sq,sq);
    }

    Poly res;
    res.resize(n);
    for(int i=0;i<n;i++){
        for(auto pr : sq[i].terms){
            res[i]=res[i]+a[pr.var1];
        }
    }
    return res;
}
Poly combine(const vector<Poly> &power,int t1,int t2){
    if(t1==-1)
        return power[t2];
    if(t2==-1)
        return power[t1];
    return multiply(power[t1],power[t2]);
}


Poly pow(Poly x,int t,int c){
    //d=2^t+c, c=-1,0,1
    //x^d
    Poly res;
    res.resize(n);
    for(int i=0;i<n;i++)
        res[i]=Expression();
    res[0].constant=1;
    if(c==-1){
        for(int i=0;i<t;i++){
            res=multiply(res,x);
            x=square(x);
        }
        return res;
    }
    auto tx=x;
    for(int i=0;i<t;i++)
        tx=square(tx);
    res=tx;
    if(c==1)
        res=multiply(res,x);

    return res;
}
void verify_linear(){
    //assume x^d=g^d=\alpha
 #ifdef RAIN
    Poly g,alpha;
    g.resize(n);
    for(int i=0;i<n;i++)
        g[i]=Expression();
    g[0].constant=1;
    g[1].constant=1;

    srand(time(0));
    for(int i=0;i<n;i++)
        g[i].constant=rand()%2;

    alpha=pow(g,LOGD,1); 
    
    auto as=pow(alpha,LOGD,-1);

    Poly x;
    Poly power_of_x[n]; 

    x.resize(n);
    for(int i=0;i<n;i++){
        x[i]=Expression(Term(i,i));
    }
    //x^{2^2lg}=x^{(2^lg+1)*(2^lg-1)}x
    //x^{2^2lg}=a^{(2^lg-1)}x

    auto x2d=pow(x,2*LOGD,0);
    auto asx=multiply(as,x);
    auto equation = add(x2d,asx);
    
        
    BitBasis<n> basis; 

    for(auto eq: equation){  
        bitset<n>bs;
        for(auto term : eq.terms){
            bs[term.var1]=1;
        }
        basis.insert(bs);
    } 

    if(n-basis.rank()!=2*LOGD){
        puts("NO!");
        for(int i=0;i<n;i++)
            cout<<g[i].constant<<" ";
        cout<<endl;
        exit(0);
    }
#endif
    return ;
}

void count(){
    Poly x,y;

    x.resize(n);
    y.resize(n);
    for(int i=0;i<n;i++){
        x[i]=Expression(Term(i,i));
        y[i]=Expression(Term(n+i,n+i));
    }
    auto xy=multiply(x,y);
    int ans=0;
    for(auto eq: xy){
        ans+=eq.terms.size();
    }
    cout<<ans<<" "<<n*n<<endl;
    return ;
}

void generate(){

    //assume x^d=1
    Poly x;
    Poly power_of_x[n]; 

    x.resize(n);
    for(int i=0;i<n;i++){
        x[i]=Expression(Term(i,i));
    }
    auto tx=x;
    for(int i=0;i<n;i++){ 
        power_of_x[i]=tx; 
        tx=square(tx);
    }

    vector<Expression> equations;

    for(auto t : {T}){
        //x^(2^t)+x=0 
        auto equation=add(power_of_x[t],x);

        for(auto eq: equation){
            eq.simplify();
            equations.push_back(eq);
        }
    }

    cout<<"### Linear equations: ###"<<endl;
    for(auto eq : equations)
        eq.print();
    
    BitBasis<n> basis;
    //Finding free variables O(n^3)
    for(auto eq : equations){
        bitset<n>bs;
        for(auto term : eq.terms){
            bs[term.var1]=1;
        }
        basis.insert(bs);
    }
    auto basic_vars=basis.basic_vars();
    auto free_vars=basis.free_vars();
    int rank=basis.rank();

    cout<<"rank = "<<rank<<endl;
    for(auto var : basic_vars){
        cout<<"basic var: "<<name(var)<<endl;
    }
    for(auto var : free_vars){
        cout<<"free var: "<<name(var)<<endl;
    }
    vector<Expression> repr_x;// represent basic vars as linear combination of free vars O(n^2)
    repr_x.resize(n);
    for(int i=0;i<n;i++){
        if(basis.bs[i][i]){
            for(int j=0;j<n;j++){
                if(basis.bs[i][j]&&!basis.bs[j][j]){
                    repr_x[i]=repr_x[i]+Expression(Term(j));
                }
            }
        }else{
            repr_x[i]=Expression(Term(i));
        }
    }
    cout<<"### represent vars as free vars ###"<<endl;
    for(int i=0;i<n;i++){
        auto r=repr_x[i];
        cout<<name(i)<<" = ";
        r.print();
    }



    vector<Poly> power_of_repr_x;
    power_of_repr_x.resize(n); 

    tx=repr_x;
    for(int i=0;i<n;i++){ 
        //cerr<<"calc x^2^"<<i<<" which is represented by free vars"<<endl;
        power_of_repr_x[i]=tx; 
        tx=square(tx);
    }


    cout<<"### finding quadratic equations ###"<<endl;
    auto mat = rand_mat(); 
    auto r = multiply_matrix(repr_x,mat); 

    equations.clear();

    auto repr_x_c=repr_x;
    for(int i=0;i<n;i++)
        repr_x_c[i].constant=rand()%2;

    vector<Poly> equations_over_GF2n;

#ifdef RAIN
    auto rx=multiply(r,repr_x_c); 
    auto r2x_r=add(multiply(square(r),repr_x_c),r); 
    auto rx2_x=add(multiply(r,square(repr_x_c)),repr_x_c); 
    auto xd=multiply(power_of_repr_x[LOGD],repr_x);
    auto linear_mul_x=add(multiply(power_of_repr_x[T],repr_x)
                ,multiply(power_of_repr_x[0],repr_x));

    equations_over_GF2n.push_back(rx);
    equations_over_GF2n.push_back(r2x_r);
    equations_over_GF2n.push_back(rx2_x);
    equations_over_GF2n.push_back(xd);
    equations_over_GF2n.push_back(linear_mul_x);
#endif
#ifdef AIM
    auto rx=multiply(r,repr_x_c);  
    auto xd=add(repr_x,repr_x);
    auto linear_mul_x=add(multiply(power_of_repr_x[T],repr_x)
                ,multiply(power_of_repr_x[0],repr_x));
    equations_over_GF2n.push_back(rx);
    equations_over_GF2n.push_back(xd);
    equations_over_GF2n.push_back(linear_mul_x);
#endif



    //Finding l
    //Solving final quadratic equations costs O(quad_vars^3)
    BitBasis<QUAD_VARS> basis2;
    
    map<pair<int,int>,int>mapping;
    int quad_vars=0;
    for(auto v1 : free_vars)
    for(auto v2 : free_vars){
        if(v1<=v2)
            mapping[make_pair(v1,v2)]=quad_vars++;
    }


    for(auto equation : equations_over_GF2n){
        for(auto eq: equation){ 
            equations.push_back(eq);
        }
    }

    for(auto eq : equations){
        eq.print();
    }

    for(auto eq : equations){
        bitset<QUAD_VARS>bs;
        for(auto term : eq.terms){
            bs[mapping[make_pair(term.var1,term.var2)]]=1;
        }
        basis2.insert(bs);
    }
 
    cout<<"Field : GF(2^"<<n<<")"<<endl;
#ifdef RAIN
    cout<<"Guess : x^(2^"<<LOGD<<"+1)"<<endl;
#endif
#ifdef AIM
    cout<<"Guess : x^45"<<endl;
#endif 
    cout<<"number of free variable : "<<n-rank<<endl;
    cout<<"number of quadratic variables : "<<quad_vars<<endl;
    cout<<"number of linear independent quadratic equations : "<<basis2.rank()<<endl;
    if(basis2.rank() >= quad_vars)
#ifdef RAIN
        cout<<"Complexity : 2^"<<(n-LOGD+3*(log2(max(n,quad_vars))-log2(n)))<<endl;
#endif
#ifdef AIM
    cout<<"Complexity : 2^"<<(n-log2(D)+3*(log2(max(n,quad_vars))-log2(n)))<<endl;
#endif 
    else
        cout<<"Unsolvable"<<endl;
}


int main(){
    srand(123);
    set_parameters();
    //for(int i=0;i<100;i++){
    //    cout<<"checking "<<i<<endl;
    //}
    //count();
    generate();

    return 0;
}