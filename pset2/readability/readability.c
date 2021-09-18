#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    //Prompt user for text
    string text = get_string("Text:");

    float letters = 0;
    float words = 1;
    float sentences = 0;

    //count letters, words and sentences as defined in pset2
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        int c = toupper(text[i]);
        if (c >= 65 && c <= 90)
        {
            letters++;
        }
        else if (c == 32 && text[i + 1] != 32 && i != n - 1 && i != 0)
        {
            words++;
        }
        else if (c == 46 && text[i + 1] != 46 && i != 0)
        {
            sentences++;
        }
        else if (c == 63 && text[i + 1] != 63 && i != 0)
        {
            sentences++;
        }
        else if (c == 33 && text[i + 1] != 33 && i != 0)
        {
            sentences++;
        }

    
    }
    if (letters == 0)
    {
        words = 0;
        sentences = 0;
    }
    
    //calculate index
    float L = letters * 100 / words;
    
    float S = sentences * 100 / words;
    
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    
    //print grade
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    
   




    
}