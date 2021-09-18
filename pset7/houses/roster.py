import sys
import cs50
import csv

# check for correct usage
if len(sys.argv) != 2:      
    exit(1)
    
db = cs50.SQL("sqlite:///students.db")

houses = {
    "Gryffindor": db.execute("SELECT first, middle, last, birth FROM students WHERE house = 'Gryffindor' ORDER BY last, first;"),
    "Slytherin": db.execute("SELECT first, middle, last, birth FROM students WHERE house = 'Slytherin' ORDER BY last, first;"),
    "Ravenclaw": db.execute("SELECT first, middle, last, birth FROM students WHERE house = 'Ravenclaw' ORDER BY last, first;"),
    "Hufflepuff": db.execute("SELECT first, middle, last, birth FROM students WHERE house = 'Hufflepuff' ORDER BY last, first;")
}


for rows in houses.get(sys.argv[1]):
    if rows['middle'] == None:
        print(f"{rows['first']} {rows['last']}, born {rows['birth']}")
    else:
        print(f"{rows['first']} {rows['middle']} {rows['last']}, born {rows['birth']}")
    
