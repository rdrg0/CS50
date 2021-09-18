#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //prompt user for credit card number

    long ccnum = get_long("Number: ");

    //get the number of digits by rounding its base 10 log to the lower integer plus 1

    float log10ccnum = log10(ccnum);

    int n = floor(log10ccnum) + 1;

    //run a for loop that multiplies digit number i by 2 and then adds  the products digits to the variable v. i matches every other digit starting from the second digit on the right.
    int u = 0;

    int v = 0;

    for (int i = 1; i <= n; i += 2)
    {

        long p10i1 = pow(10, i + 1);
        long p10i = pow(10, i);

        u = floor((ccnum % p10i1) / p10i);
        v = v + floor((2 * u) / 10) + ((2 * u) % 10);

    }
    //run a for loop to store the remaining digits in the variable y
    u = 0;
    int y = 0;

    for (int i = 0; i <= n; i += 2)
    {
        long p10i1 = pow(10, i + 1);
        long p10i = pow(10, i);
        u = floor((ccnum % p10i1) / p10i);
        y = y + u;

    }

    //add y + v in order to test for luhn's algorithm

    int w = y + v;

    //run if statements to check for card number requirements

    if (w % 10 > 0)
    {
        printf("INVALID\n");
    }

    else if ((floor(ccnum / pow(10, 13)) == 34 || floor(ccnum / pow(10, 13)) == 37) && n == 15)
    {
        printf("AMEX\n");
    }
    else if (floor(ccnum / pow(10, 14)) >= 51 && floor(ccnum / pow(10, 14)) <= 55 && n == 16)
    {
        printf("MASTERCARD\n");
    }
    else if ((floor(ccnum / pow(10, 12)) == 4 && n == 13) || (floor(ccnum / pow(10, 15)) == 4 && n == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }






}