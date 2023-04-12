import math
import random
from tqdm import tqdm
n=192
d=45
power_of_two=[2**i for i in range(0,n)]
power_of_two_with_zero=[0]+power_of_two


def log2(x):
    if x==0:
        return -1
    return round(math.log(x,2))

def find_linear(n,d):
    lst=[]
    for t1 in [1]:
        for t2 in power_of_two:
            if (t1-t2)%d==0 and t1<t2:
                lst.append((log2(t1),log2(t2)))
    return lst


def check(t):
    mn=10000000
    for x in t:
        if x!=0:
            mn=min(mn,x)
    return mn

def find_quadratic(n,d):
    lst=[]
    for t1 in tqdm([0,1]):
        for t2 in power_of_two_with_zero:
            for t3 in power_of_two_with_zero:
                for t4 in power_of_two_with_zero:
                    if (t1+t2-t3-t4)%d==0 and t1<t2 and t3<t4 and check([t1,t2,t3,t4])==1:
                        lst.append((log2(t1),log2(t2),log2(t3),log2(t4)))
    return lst


print(find_linear(n,d))
#lst=find_quadratic(n,d)
lst=[]
text=""
for x in lst:
    text+=f"{{{x[0]},{x[1]},{x[2]},{x[3]}}},"
print(len(lst))
print(text)
