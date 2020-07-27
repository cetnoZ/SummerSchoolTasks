import os
import random
for i in range(1, 51, 1):
    os.system("./generator {test_number} {random}".format(test_number=i, random=random.randint(1, 10000)))