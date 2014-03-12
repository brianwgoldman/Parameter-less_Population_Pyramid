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
        problem, length, solver = path.basename(filename).split('.')[:-1]
        solver = pretty_names[solver]
        length = str(int(length))
    except ValueError:
        print "Bad filename", filename
        continue
    with open(filename, 'r') as f:
        lines = f.read().strip().split('\n')
    if len(lines) != 101:
        print >> sys.stderr, filename, len(lines)
        continue
    for run, line in enumerate(lines[1:]):
        print sep.join([problem, length, solver, str(run)] + line.split(' '))
