import sys
import glob
from os import path, remove
from subprocess import call
import signal
import time

def signal_handler(signal, frame):
        raise Exception('Signal received, exiting')
signal.signal(signal.SIGINT, signal_handler)


executable = path.join("Release", "P3")
folder = sys.argv[1]
repeats = int(sys.argv[2])
bound_file = path.join(folder, "bounds.txt")
arguments = path.join("config", "default.cfg ") + " ".join(sys.argv[3:])
single_run = "-experiment single_run"

def update(least, most, maxed):
    with open(bound_file, 'w') as f:
        f.write("%i %i %i\n" % (least, most, maxed))

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
            extra = "-pop_size %i -dat_file %s.dat -problem_seed %i -seed %i"
            extra = extra % (pop_size, filename, i, i+1)
            combined = " ".join([executable, arguments, single_run, extra])
            print combined
            try:
                call(combined.split())
            except Exception as e:
                print e
                sys.exit()
            finally:
                remove(filename + ".start")
            return
    print "RUNS INCOMPLETE"
    sys.exit()


while True:
    try:
        with open(bound_file, 'r') as f:
            least, most, maxed = map(int, f.read().strip().split())
    except (IOError, ValueError):
        least, most, maxed = 0, 1, 0

    while maxed == 0:
        print "FIRST LOOP", least, most
        pop_size = most
        # Try and find a job at this level
        status = examine_results(folder, repeats, pop_size)
        if status == "failed":
            least = most
            most *= 2
            update(least, most, maxed)
        elif status == "finished":
            maxed = 1
            update(least, most, maxed)
        else:  # incomplete
            start_run(folder, repeats, pop_size)
            break

    while maxed > 0 and least + 1 < most:
        print "SECOND LOOP", least, most
        pop_size = (most + least) / 2
        status = examine_results(folder, repeats, pop_size)
        if status == "failed":
            least = pop_size
            update(least, most, maxed)
        elif status == "finished":
            most = pop_size
            update(least, most, maxed)
        else:  # incomplete
            start_run(folder, repeats, pop_size)
            break

    if maxed != 0 and least + 1 == most:
        with open(path.join(folder, "result.txt"), 'w') as f:
            f.write(" ".join([executable, arguments, '-pop_size %i' % most, '\n']))
        break
