# ---
# jupyter:
#   jupytext:
#     formats: py:percent
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.17.3
#   kernelspec:
#     display_name: Python 3 (ipykernel)
#     language: python
#     name: python3
# ---

# %%
people = ['Dr. Christopher Brooks', 'Dr. Kevyn Collins-Thompson', 'Dr. VG Vinod Vydiswaran', 'Dr. Daniel Romero']

def split_title_and_name(person):
    return person.split()[0] + ' ' + person.split()[-1]

#option 1
for person in people:
    print(split_title_and_name(person) == (lambda person:person.split()[0]+' '+person.split()[-1])(person))

list(map(split_title_and_name, people)) == list(map((lambda person:person.split()[0]+' '+person.split()[-1]),people))


# %%
def times_tables():
    lst = []
    for i in range(10):
        for j in range (10):
            lst.append(i*j)
    return lst

times_tables() == [i*j for i in range(10) for j in range(10) ]

# %%
lowercase = 'abcdefghijklmnopqrstuvwxyz'
digits = '0123456789'

answer = [x+y+z+w for x in lowercase for y in lowercase for z in digits for w in digits]
print(answer[:5])


# %%
