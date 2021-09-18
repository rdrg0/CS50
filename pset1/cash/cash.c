#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
// Prompt user for valid input
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change <= 0);
    
//Round input (dollars) and convert it to cents by multiplying by 100
    int cents = round(change * 100);

//Divide the amount of cents by 25 to know how many 25c coins are needed. Do the same with the remaining cents and lower value coins going in decreasing order.
    int coins25 = cents / 25;

    int remainder25 = cents % 25;

    int coins10 = remainder25 / 10;
    
    int remainder10 = remainder25 % 10;
    
    int coins5 = remainder10 / 5;
    
    int coins1 = remainder10 % 5;
    
    int coinstotal = coins1 + coins5 + coins10 + coins25;
    


    printf("%i\n", coinstotal);
}