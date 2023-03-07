#!/bin/python

from typing import Tuple

breakpoints = []

def compute_breakpoints():
    max = 1_000_000
    global breakpoints
    breakpoints.append(0)
    
    level = 0
    acu = 0
    while(acu <= max):
        level += 6
        acu += level
        breakpoints.append(acu)

compute_breakpoints()


def lower_bound(coll, item) -> int:
    start = 0
    size = len(coll)

    while (start < size):
        ptr = start + (size - start) // 2

        if coll[ptr] >= item:
            size = ptr
        else:
            start = ptr + 1
   
    if start < len(coll) and coll[start] < item:
        start += 1
      
    return start
            

def coordinates(point: int) -> Tuple[int, int]:
    if point == 1:
        return (0, 0)
    
    level = lower_bound(breakpoints, point - 1)
    level_start = breakpoints[level - 1] + 1
    items_in_level = breakpoints[level] - breakpoints[level - 1]
    chunk_size = items_in_level // 6
    index_in_level = point - level_start - 1
    index_in_chunk = index_in_level % chunk_size
    chunk_number = index_in_level // chunk_size
    is_corner = index_in_chunk == chunk_size - 1


    vertical = 0
    diagonal = 0

    if chunk_number == 0 or chunk_number == 3:
        vertical = index_in_chunk + 1
    elif chunk_number == 1 or chunk_number == 4:
        vertical = chunk_size
    elif chunk_number == 2 or chunk_number == 5:
        vertical = chunk_size - index_in_chunk - 1
    
    if chunk_number < 3:
        vertical *= -1


    if chunk_number == 5 or chunk_number == 2:
        diagonal = index_in_chunk + 1
    elif chunk_number == 0 or chunk_number == 3:
        diagonal = chunk_size
    elif chunk_number == 1 or chunk_number == 4:
        diagonal = chunk_size - index_in_chunk - 1
    
    if not (chunk_number < 2 or chunk_number == 5):
        diagonal *= -1
    
    return (vertical, diagonal)

def binom(n: int, k: int) -> int:
    acu = 1
    for i in range(k + 1, n + 1):
        acu *= i
    for i in range(1, n - k + 1):
        acu //= i
    return acu
    
def distance(fr: Tuple[int, int], to: Tuple[int, int]) -> Tuple[int, int]:
    diffX = fr[0] - to[0]
    diffY = fr[1] - to[1]
    aDiffX = abs(diffX)
    aDiffY = abs(diffY)
    
    is_fine = (diffX * diffY) >= 0
    if is_fine:
        sum = aDiffX + aDiffY
        alt = binom(sum, aDiffX)
        return (sum, alt)
    
    new_diag = fr[1] + diffX;
    new_diff = to[1] - new_diag
    is_top = new_diff * new_diag > 0
    # print(f"Not fine {diffX}, {diffY}, {new_diff}, {is_top}")

    if is_top:
        sum = aDiffX + abs(new_diff)
        alt = binom(sum, aDiffX)
        return (sum, alt)
    
    aDiffX -= abs(new_diff)
    sum = aDiffX + abs(new_diff)
    alt = binom(sum, aDiffX)
    return (sum, alt)
    
def test():
    for i in range(6):
        print(binom(5, i))
    for i in range(7):
        print(binom(6, i))

    for i in range(2, 42):
        level = lower_bound(breakpoints, i)
        print(f"{i}: {level}")
    for i in range(1, 42):
        point = coordinates(i)
        print(f"{i}: {point}")
    
# test()

while True:
    line = input()
    (x, y) = [int(x) for x in line.split(" ")]
    if x == 0:
        break;
    
    # print(f"\nProcessing {x} {y}")
    fr = coordinates(x)
    to = coordinates(y)
    dist, alt = distance(fr, to)
    
    if alt == 1:
        print(f"There is {alt} route of the shortest length {dist}.");
    else:
        print(f"There are {alt} routes of the shortest length {dist}.");
