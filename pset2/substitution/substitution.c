#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//get key as command line argument

int main(int argc, string argv[])
{



    string plaint = 0;

//validate key    

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");

        return 1;
    }

    string key = argv[1];
    
    
    int keylen = strlen(key);

    if (keylen != 26)
    {
        printf("Key must contain 26 characters.\n");

        return 1;
    }
    
    for (int n = 0; n < 26; n++)
    {
        for (int m = 1; m <= 25 - n; m++)
        {
            int keyup = toupper(key[n]);
                
            int keyup2 = toupper(key[n + m]);
                
            int z = keyup - keyup2;
                
            if (z == 0)
            {
                printf("Usage: ./substitution key\n");
                   
                return 1;
            }
            
            else if (keyup < 65 || keyup > 90)
            {
                printf("Usage: ./substitution key\n");
                   
                return 1;
            }
        }
    }

//get plaintext 

    plaint = get_string("paintext: ");


//encipher

    int t = strlen(plaint);

    for (int i = 0; i < t; i++)
    {
        if (plaint[i] >= 65 && plaint[i] <= 90)
        {
            int x = plaint[i] - 65;
            plaint[i] = toupper(key[x]);
        }
        else if (plaint[i] >= 97 && plaint[i] <= 122)
        {
            int x = plaint[i] - 97;
            plaint[i] = tolower(key[x]);
        }
    }
//print ciphered text

    printf("ciphertext: %s\n", plaint);



}