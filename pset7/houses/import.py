import sys
import cs50
import csv

# check for correct usage
if len(sys.argv) != 2:      
    exit(1)
    
db = cs50.SQL("sqlite:///students.db")


with open(sys.argv[1], "r") as characters:
    reader = csv.DictReader(characters)
    
    for row in reader:
        row['name'] = row['name'].split()
        if len(row['name']) == 2:
            first = row['name'][0]
            last = row['name'][1]
            middle = None
        elif len(row['name']) == 3:
            first = row['name'][0]
            middle = row['name'][1]
            last = row['name'][2]
        house = row['house']
        birth = row['birth']
        
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", 
                   first, middle, last, house, birth)
        
