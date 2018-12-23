import random
import time
import csv

correct_string = 'hello world'

pop_size = 50
mutation_chance = 53

ascii_pool = 'abcdefghijklmnopqrstuvwxyz '

# Character frequencies from http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
char_freq = {' ': 19, 'e': 12.02, 't': 9.1, 'a': 8.12, 'o': 7.68, 'i': 7.31, 'n': 6.95, 's': 6.28, 'r': 6.02, 'h': 5.92, 'd': 4.32, 'l': 3.98, 'u': 2.88, 'c': 2.71, 'm': 2.61, 'f': 2.3, 'y': 2.11, 'w': 2.09, 'g': 2.03, 'p': 1.82, 'b': 1.49, 'v': 1.11, 'k': 0.69, 'x': 0.17, 'q': 0.11, 'j': 0.1, 'z': 0.07}

# Should the char-freq pool be used instead of random sampling for mutations
use_char_freq = True

def evaluator(genotype):
    return 1+sum(1 if e == correct_string[i] else 0 for i,e in enumerate(genotype))
def recombine(a, b=None):
    """Returns a tuple of 2 offspring from 2 parents
Lengths must match
Input can be two bitstrings, or 1 tuple"""
    if type(a) == tuple:
        b = a[1]
        a = a[0]
    if len(a) != len(b):
        return None
    split = random.randint(0, len(a))
    c = a[:split] + b[split:]
    d = b[:split] + a[split:]
    return (c,d)
def mutator(genotype):
    genotype = list(genotype)
    if random.randint(0, 99) < 2:
        mutatebit = random.randint(0,len(genotype)-1)
        genotype[mutatebit] = random.choices(list(char_freq.keys()), list(char_freq.values()))[0] if use_char_freq else random.choice(ascii_pool)
    return ''.join(genotype)

if __name__ == "__main__":
    gen = [''.join(random.choice(ascii_pool) for i in range(len(correct_string))) for _ in range(pop_size)]
    gen_idx = 0
    data = [{"gen_id": 0, "max_eval": max(evaluator(i) for i in gen), "avg_eval": sum(evaluator(i) for i in gen)/len(gen)}]
    data[0]["mad_eval"] = sum(abs(evaluator(i) - data[0]["avg_eval"]) for i in gen)/len(gen)
    while not all(i == correct_string for i in gen):
        #print("Generation", gen_idx)
        #print(gen)
        #print("Max evaluation:", max(evaluator(i) for i in gen))
        #print("Average evaluation:", sum(evaluator(i) for i in gen)/len(gen))
        gen_fit = [evaluator(i) for i in gen]
        int_gen = random.choices(gen, gen_fit, k=len(gen))
        new_gen = []
        for a,b in zip(int_gen[::2], int_gen[1::2]):
            new_gen.extend(recombine(a,b))
        new_gen = list(map(mutator, new_gen))
        gen = new_gen
        gen_idx += 1
        data.append({"gen_id": gen_idx, "max_eval": max(evaluator(i) for i in gen), "avg_eval": sum(evaluator(i) for i in gen)/len(gen)})
        data[-1]["mad_eval"] = sum(abs(evaluator(i) - data[0]["avg_eval"]) for i in gen)/len(gen)
    with open("data.csv", "w") as f:
        writer = csv.DictWriter(f, ("gen_id", "max_eval", "avg_eval"))
        writer.writeheader()
        for row in data:
            writer.writerow(row)
    print("Generation", gen_idx)
    #print([int(i,2) for i in gen])
    print(gen)
    print("Max evaluation:", max(evaluator(i) for i in gen))
print("Average evaluation:", sum(evaluator(i) for i in gen)/len(gen))
