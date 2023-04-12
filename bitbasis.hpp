#ifndef BITBASIS_HPP_
#define BITBASIS_HPP_
 
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <bitset>
using std::vector;
using std::set;
using std::bitset;
template<int n>
struct BitBasis{
    vector<bitset<n> >bs;

    BitBasis(){
        bs.resize(n);
    }

    bool insert(const bitset<n> &_a){
        bitset<n> a=_a;
        for(int i=0;i<n;i++){
            if(a[i]){
                if(!bs[i][i]){
                    bs[i]=a;
                    return true;
                }else{
                    a^=bs[i];
                }
            }
        }
        return false;
    }
    void clear(){
        bs.clear();
        bs.resize(n);
    }
    int rank()const{
        int res=0;
        for(int i=0;i<n;i++)
            if(bs[i][i])
                res++;
        return res;
    }
    vector<int> basic_vars(){
        vector<int>res;
        for(int i=0;i<n;i++)
            if(bs[i][i])
                res.push_back(i);
        return res;
    }
    vector<int> free_vars(){
        vector<int>res;
        for(int i=0;i<n;i++)
            if(!bs[i][i])
                res.push_back(i);
        return res;
    }
};


#endif