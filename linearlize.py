import sys
import os
import re
import math
import random
import time
from itertools import chain, combinations
from functools import reduce

#factor 2^n-1
factors={}
factors[8]=[3,5,17]
factors[128]=[ 3 , 5 , 17 , 257 , 641 , 65537 , 274177 , 6700417 , 67280421310721]
factors[192]=[  3, 3 , 5 , 7 , 13 , 17 , 97 , 193 , 241 , 257 , 641 , 673 , 65537 , 6700417 , 22253377 , 18446744069414584321]
factors[256]=[3 , 5 , 17 , 257 , 641 , 65537 , 274177 , 6700417 , 67280421310721 , 59649589127497217 , 5704689200685129054721]

#all subset of a list
def all_subsets(ss):
    return chain(*map(lambda x: combinations(ss, x), range(0, len(ss)+1)))

def product(numbers):
    return reduce(lambda x, y: x * y, numbers, 1)

def all_divisors(n):
    return [product(s) for s in all_subsets(factors[n])]

#x^h = (x^d)^s * x^{2^t}
# (h-2^t)%d=0
def find_rain(n,h,d):
    h=h+2**n-1 
    assert (2**n-1)%d==0
    for i in range(n):
        if (h-2**i)%d==0:
            return i
    return -1
    
#For AIM, linearlize two Sbox
def find_aim(n,h1,h2,d):
    h1=h1+2**n-1
    h2=h2+2**n-1
    assert (2**n-1)%d==0
    lst=[]
    for i in range(n):
        if (h1-2**i)%d==0 and (h2-2**i)%d==0:
            return i
    return -1


print('Finding for Rain')
for n in [8,128,192,256]:
    h=2**n-2
    print("n=%d"%n)
    print("h=2**%d-2"%n)
    divisors=all_divisors(n)
    for d in divisors:
        t=find_rain(n,h,d)
        if t!=-1:
            print("x^{2**%d-2} = (x^%d)^s * x^{2^%d} " % (n,d,t))

print('Finding for AIM')
for n in [192]:
    h1=2**5-1
    h2=2**29-1
    print("n=%d"%n)
    print("h1=2**5-1")
    print("h2=2**29-1")
    divisors=all_divisors(n)
    for d in divisors:
        t=find_aim(n,h1,h2,d)
        if t!=-1:
            print("x^{2**5-1} = (x^%d)^s * x^{2^%d} " % (d,t))
            print("x^{2**29-1} = (x^%d)^s * x^{2^%d} " % (d,t))