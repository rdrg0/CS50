import cs50

h = -1

while h < 1 or h > 8:
    h = cs50.get_int("Height: ")
    
for j in range(h):
    for k in range(h - j - 1):
        print(" ", end="")
    for i in range(j + 1):
        print("#", end="")
    print("  ", end="")
    for i in range(j + 1):
        print("#", end="")
    print()
    