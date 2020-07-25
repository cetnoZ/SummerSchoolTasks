import sys
import os.path
import os
import time

solution_name = "solution.cpp"

def print_error(text):
    print('\033[31;1m' + text + '\033[m')

def print_info(text):
    print('\033[34;1m' + text + '\033[m')

def print_good(text):
    print('\033[32;1m' + text + '\033[m')

if __name__ == "__main__":

    if len(sys.argv) > 1:
        solution_name = sys.argv[1]

    print_info("Solution {solution_name} will be tested!".format(solution_name=solution_name))
    
    solution_path = "solution_to_test/" + solution_name
    
    if not os.path.exists(solution_path):
        print_error("File {name} not exists!".format(name=solution_path))
        exit(1)
    #build solution to test
    os.system("(cd solution_to_test/ && PROJECT={solution_name} make)".format(solution_name=solution_name))

    cnt_success_tests = 0
    for test in range(1, 51, 1):
        in_test = "tests/in/" + str(test)
        out_test = "tests/out/" + str(test)
        res_test = "tests/result/" + str(test)

        if not os.path.exists(in_test):
            print_error("Error no exists: {file}".format(file=in_test))
            exit(1)

        if not os.path.exists(out_test):
            print_error("Error no exists: {file}".format(file=out_test))
            exit(1)
        
        project_name = solution_name[:solution_name.find('.')]#cut file extention
        ###start solution with in_test data
        #
        os.system("{ "+("time ./solution_to_test/{project_name} <{in_test} >{res_test}".format(project_name=project_name, in_test=in_test, res_test=res_test))+" ; } 2> tmp")
        #
        execution_time = 2
        with open('tmp') as file:
            line = file.readline()
            execution_time = float(line[line.find('system') - 4: line.find('system')])

        print("##########################################")
        print("Test {test} finished".format(test=str(test)))
        print("Time: " + str(execution_time))
        cmp_res = os.system("cmp {out_test} {res_test}".format(out_test=out_test, res_test=res_test))
        if cmp_res != 0 or execution_time > 2:
            print_error("Not success")
        else:
            print_good("OK")
            cnt_success_tests += 2
    
    print_info("Result: {res}".format(res=cnt_success_tests))

        

