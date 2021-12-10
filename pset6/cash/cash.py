# THIS PROGRAM OUTPUTS THE MINIMUM NUMBER OF COINS
# TO RETURN FOR EACH AMOUNT OF CHANGE OWED


# Get user input repeatedly until reasonable value given
# VERSION: WITHOUT CS50
while True:
    try:
        change = float(input("Change owed: "))
        if change >= 0:
            break
    except ValueError:
        # do nothing
        print("", end="")

## BACKGROUND ##
# Coins available:
# 1. Quarter ($0.25)
# 2. Dime ($0.10)
# 3. Nickel ($0.05)
# 4. Penny ($0.01)

coins = [0.25, 0.10, 0.05, 0.01]
count = 0

for coin in coins:
    change = round(change, 2)
    while (change >= coin):
        change -= coin
        count += 1
        print(change)

print(count)