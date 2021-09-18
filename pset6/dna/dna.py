import sys
import cs50
import csv


def main():
    
    # check for correct usage
    if len(sys.argv) != 3:      
        exit(1)
    # create a list to store the largest number of repetitions for each STR in the same order as de csv database
    person = []     
    
    # open dna sequence file 
    with open(sys.argv[2], mode='r') as file:  
        # store dna sequence inside list
        dna_sample = list(file)[0]   
    # open dna database and store it using DictReader
    with open(sys.argv[1], mode='r') as csv_file:  
        dna_db = csv.DictReader(csv_file)       
        # store number of STR inside variable
        str_num = len(dna_db.fieldnames) - 1    
        # loop through STRs in order to find the largest number of repetitions for each one   
        for i in range(str_num):            
            gene = dna_db.fieldnames[i+1]
            # store the largest number of repetitions for each STR using findlongest function
            longest = int(len(findlongest(gene, dna_sample, gene)) / len(gene))      
            # store the numbers in person list
            person.append(longest)          
        # loop through stored csv database
        for row in dna_db:
            # create list in order to compare each individual in the database against the sequence yet to identify 
            tmplist = []                    
            for i in range(str_num):
                # iterates through each individual and stores their STRs inside the list
                tmplist.append(int(row[dna_db.fieldnames[i+1]]))    
            if tmplist == person:
                # if all the STRs match with an individual in the database the name is printed and the program exits
                print(row[dna_db.fieldnames[0]])    
                exit(0)
            else:
                continue
        # if the sequence doesnt match any of the individuals in the database the program prints "No match"
        print("No match")       
        

def findlongest(gene, dna_sample, gene0):       
    
    if gene in dna_sample:
        # looks for 1 instance of the STR string
        gene = gene + gene0 
        # if the STR string is found, recursively looks for the STR plus one repetition 
        return findlongest(gene, dna_sample, gene0)
    else:
        return gene[:-len(gene0)]
        # if the STR is not found, substracts one instance of the original STR string and returns it
        
        
main()