from matplotlib import pyplot as plt
from os import listdir

from const import *

# def color(k):
#     match k:
#         case 0:
#             return "r"
#         case 1:
#             return "g"
#         case 2:
#             return "y"
#         case 3:
#             return "m"
#         case 4:
#             return "b"
#         case 5: 
#             return "c"
#         case _:
#             return "k"

def color(k):
    match k:
        case 1:
            return "g"
        case 5:
            return "r"
        case _:
            return "b"

# def color(k):
#     if k == 40:
#         return "m"
#     elif k > 20:
#         return "b"
#     elif k == 0:
#         return "r"
#     elif k < 20:
#         return "g"
#     else:
#         return "g"

# def names(k,legend):
#     if k == 40: return legend+"40"
#     elif k == 60: return legend+">20"
#     elif k == 0: return legend+"0"
#     elif k == 2: return legend+"<20"
#     else: return ""

def behavior_name(k):
    match k:
        case 0:
            return "immobile"
        case 1:
            return "rotation"
        case 2:
            return "random"
        case 3:
            return "back-and-forth"
        case 4:
            return "in square"
        case 5:
            return "in swarm"
        case _:
            return "in S"

def fill_dico(dico):
    files = sorted(listdir(rac+folder))
    for f in files:
        data = f.split("_")
        behave = int(data[0])
        nb = int(data[1].split(".")[0])
        file = open(rac+folder+f,"r")
        line = file.readline()
        if not (behave in dico):
            dico[behave] = {}
        dico[behave][nb] = []
        while line:
            try:
                l = int(line)
                if l == -200 and behave == 0:
                    dico[behave][nb].append(-500)
                else:
                    dico[behave][nb].append(l)
            except:
                pass
            line = file.readline()
            
        
def sum(l):
    sum = 0
    for e in l:
        sum += e
    return sum

def mean(l):
    res = sum(l)
    if res == 0:
        return 0
    else:
        return res/len(l)

def smooth(x,y) :
    k = 5
    new_x = []
    new_y = []
    for xi in x :
        new_x.append(xi)
    for i in range(len(y)):
        new_y.append(mean(y[max(i-k,0):i+k+1]))
    return new_x,new_y

def plot_mean_dico(dico,legend):
    plt.figure("Mean")
    for behavior_number,d in dico.items():
        x = []
        y = []
        for nb,l in d.items():
            x.append(nb)
            y.append(mean(l))
        x,y = smooth(x,y)
        plt.plot(x,y,color=color(behavior_number),label=legend+behavior_name(behavior_number))
    plt.legend()
    plt.xlabel("number of guards")
    plt.ylabel("average time taken by the player")
    plt.show()
    
def scatter_all_dico(dico,legend):
    for behavior_number,d in dico.items():
        cpt = 0
        plt.figure(legend+" : "+str(behavior_number))
        for nb,l in d.items():
            cpt += len(l)
            y = [e for e in l if e<301 and e>-301]
            x = [nb for i in range(len(y))]
            plt.scatter(x,y,color=color(behavior_number))
        plt.xlabel("number of guards")
        plt.ylabel("time taken by the player")
        plt.show()
        print(cpt)
            
def plot_achive_rate(dico,legend):
    plt.figure("Rate")
    for behavior_number,d in dico.items():
        x = []
        y = []
        for nb,l in d.items():
            x.append(nb)
            y.append(sum([1/len(l) for e in l if e > 0]))
        x,y = smooth(x,y)
        plt.plot(x,y,color=color(behavior_number),label=legend+behavior_name(behavior_number))
    plt.legend()
    plt.xlabel("number of guards")
    plt.ylabel("player success rate")
    plt.show()