import sys
from os import path

total = 0
runs = 0
bases = set()
for filename in sys.argv[1:]:
    if 'tuning' in filename or 'p3' in filename:
        print 'skipping', filename
        continue
    try:
        with open(filename, 'r') as f:
            data = f.read().strip().split('\n')
    except IOError:
        print "Failed", filename
        continue
    base = path.split(filename)[0]
    if base not in bases:
        print base
        bases.add(base)
    if data[0][0] == '#':
        evals = [int(line.strip().split()[1]) for line in data[1:]]
        clean = [min(eval, 100000000) for eval in evals if 0 <= eval]
        value = sum(clean)
        runs += len(clean)
        if len(clean) != len(evals):
            print filename, [eval for eval in evals if eval < 0]
    else:
        value = min(int(data[-1].split()[1]), 100000000)
        if value < 0:
            print filename, value
            value = 0
        else:
            runs += 1
    total += value
print total, "%e" % total, 'runs', runs
