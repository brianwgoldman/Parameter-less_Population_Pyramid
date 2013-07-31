import sys
import glob
from os import path
from subprocess import call

executable = path.join("Release", "P3")
folder = sys.argv[1]
repeats = int(sys.argv[2])
bound_file = path.join(folder, "bounds.txt")
arguments = path.join("config", "default.txt ") + " ".join(sys.argv[3:])
single_run = "-experiment single_run"
try:
    with open(bound_file, 'r') as f:
        least, most, maxed = map(int, f.read().strip().split())
except (IOError, ValueError):
    least, most, maxed = 0, 1, 0


def examine_results(folder, repeats, pop_size):
    successes = 0
    for i in range(repeats):
        filename = path.join(folder, "%0.5i_%0.5i.dat" % (pop_size, i))
        try:
            with open(filename, 'r') as f:
                lastline = f.read().strip().split('\n')[-1]
                if lastline.split()[0] != '1':
                    return "failed"
                else:
                    successes += 1
        except (IOError, IndexError) as e:
            pass
    return "finished" if successes == repeats else "incomplete"

def start_run(folder, repeats, pop_size):
    for i in range(repeats):
        filename = path.join(folder, "%0.5i_%0.5i" % (pop_size, i))
        if not path.exists(filename + ".dat") and not path.exists(filename + ".start"):
            # touch the start file
            open(filename + ".start", 'w').close()
            extra = "-pop_size %i -outfile %s.dat -problem_seed %i -seed %i"
            extra = extra % (pop_size, filename, i, i)
            combined = " ".join([executable, arguments, single_run, extra])
            print combined
            call(combined.split())
            return
    print "RUNS INCOMPLETE"

while maxed == 0:
    print "FIRST LOOP", least, most
    pop_size = most
    # Try and find a job at this level
    status = examine_results(folder, repeats, pop_size)
    if status == "failed":
        least = most
        most *= 2
    elif status == "finished":
        maxed = 1
    else:  # incomplete
        start_run(folder, repeats, pop_size)
        break

while maxed > 0 and least + 1 < most:
    print "SECOND LOOP", least, most
    pop_size = (most + least) / 2
    status = examine_results(folder, repeats, pop_size)
    if status == "failed":
        least = pop_size
    elif status == "finished":
        most = pop_size
    else:  # incomplete
        start_run(folder, repeats, pop_size)
        break

with open(bound_file, 'w') as f:
    f.write("%i %i %i\n" % (least, most, maxed))

if maxed != 0 and least + 1 == most:
    with open(path.join(folder, "result.txt"), 'w') as f:
        f.write(" ".join([executable, arguments, '-pop_size %i' % most, '\n']))
