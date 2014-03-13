# Takes as command line arguments all of the data file names and creates
# outputs the combined data in comma separated format.  Example:
# python create_csv.py clean/*.dat > complete.csv
# This will read all of the data files in the folder "clean" and put that
# information together to create the complete.csv file.
import sys
from os import path


headers = ['problem', 'length', 'solver', 'run', 'fitness', 'evaluations']
sep = ','

pretty_names = {"hc": "Hill Climber",
                "lambdalambda": "1+(Lambda Lambda)",
                "ltga": "LTGA",
                "p3": "P3"}

print sep.join(headers)
for filename in sys.argv[1:]:
    try:
        # Infer run information using the filename
        problem, length, solver = path.basename(filename).split('.')[:-1]
        solver = pretty_names[solver]
        length = str(int(length))
    except ValueError:
        print "Bad filename", filename
        continue
    with open(filename, 'r') as f:
        lines = f.read().strip().split('\n')
    # Ensures the file has the correct number of lines
    if len(lines) != 101:
        print >> sys.stderr, filename, len(lines)
        continue
    # output the translated lines
    for run, line in enumerate(lines[1:]):
        print sep.join([problem, length, solver, str(run)] + line.split(' '))
