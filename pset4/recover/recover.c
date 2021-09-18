#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //create files

    FILE *file = fopen(argv[1], "r");

    FILE *img = fopen("000.jpg", "w");

    unsigned char *buffer = malloc(512 * sizeof(unsigned char));

    char *filename = malloc(8 * sizeof(char));

    int counter = 0;
    
    
    // Read 512 bytes into buffer there is no more data to read
    while (fread(buffer, sizeof(unsigned char), 512, file) == 512)
    {
        // Check for start of new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if first jpeg write to from buffer to img file
            if (counter == 0)
            {

                fwrite(buffer, sizeof(unsigned char), 512, img);

                counter++;

            }
            
            // if not first jpeg close img file and open new img file with new filename
            else
            {
                fclose(img);

                sprintf(filename, "%03i.jpg", counter);

                img = fopen(filename, "w");


                fwrite(buffer, sizeof(unsigned char), 512, img);

                counter++;

            }
        }
        
        // check if counter is greater than 0 in case first jpeg hasn't been found yet
        else if (counter > 0)
        {
            fwrite(buffer, sizeof(unsigned char), 512, img);
        }
    }
    
    // Close open files and free memory
    fclose(img);
    fclose(file);
    free(filename);
    free(buffer);

    





}
