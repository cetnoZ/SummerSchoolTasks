import os 
import sys

for i in range(1, 51, 1):
    os.system("./solution <../tests/in/{in_test} >../tests/result/{res}".format(in_test=str(i), res=str(i)))
    