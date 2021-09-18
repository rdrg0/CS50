import cs50
import math

change = -1.0

while change <= 0:
    change = cs50.get_float("Change owed: ")

cents = round(change*100)

coins25 = math.floor(cents / 25)
remainder25 = cents % 25
coins10 = math.floor(remainder25 / 10)
remainder10 = remainder25 % 10
coins5 = math.floor(remainder10 / 5)
coins1 = remainder10 % 5
coinstotal = coins1 + coins5 + coins10 + coins25

print(coinstotal)

