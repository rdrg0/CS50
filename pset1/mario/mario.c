#include <stdio.h>
#include <cs50.h>
void pyramid(int h);
int main(void)
{
//prompt user for height until a value between 1 and 8 is entered
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);
//run pyramid function
    pyramid(h);

}
// Define pyramid function
void pyramid(int h)
{
    for (int j = 0; j < h; j++)
    {
        for (int k = 0; k < h - j - 1; k++)
        {
            printf(" ");
        }
        for (int i = 0; i <= j; i++)
        {
            printf("#");
        }
        printf("  ");
        for (int i = 0; i <= j; i++)
        {
            printf("#");
        }
        printf("\n");
    }


}
