n = 3

start = '1' * n
print(start) // 111
while start != '4' * n:
    i = n - 1
    while start[i] == '4':
        i -= 1
    start = start[:i] + str(int(start[i]) + 1) + '1' * (n - i - 1)
    print(start)
print(start) // 444