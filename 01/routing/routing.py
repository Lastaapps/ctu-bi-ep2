#!/bin/python

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
            

def coordinates(point: int) -> (int, int):
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


def test():
    for i in range(2, 42):
        level = lower_bound(breakpoints, i)
        print(f"{i}: {level}")
    for i in range(1, 42):
        point = coordinates(i)
        print(f"{i}: {point}")
    
    
test()
