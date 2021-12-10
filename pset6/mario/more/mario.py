# Ask user for integer input until he gives an integer
while True:
    try:
        height = int(input("Height: "))
        if (height > 0 and height < 9):
            break
    except ValueError:
        # nothing. no print out wanted.
        print("", end="")


bricks = "#"
gap = " " * 2
for row in range(height):
    # build left tower
    print(" " * (height - row - 1), end="")
    print(bricks * (row + 1), end="")

    # build gap
    print(gap, end="")

    # build right tower
    print(bricks * (row + 1), end="")
    # print("%" * (height - row - 1), end="")
    print("")