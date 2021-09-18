import cs50


text = cs50.get_string("Text: ")

letters = 0.0
words = 1.0
sentences = 0.0

n = len(text)

c = text[0].upper()

text = text + "#"

for i in range(n):
    c = text[i].upper()
    if c.isalpha() == True:
        letters += 1
    elif c == " " and text[i + 1] != " " and i != n - 1 and i != 0:
        words += 1
    elif c == "." and text[i + 1] != "." and i != 0:
        sentences += 1
    elif c == "?" and text[i + 1] != "?" and i != 0:
        sentences += 1
    elif c == "!" and text[i + 1] != "!" and i != 0:
        sentences += 1
        
if letters == 0:
    words = 1
    sentencees = 1

    
L = letters * 100 / words
S = sentences * 100 / words

index = round(0.0588 * L - 0.296 * S - 15.8)

if index >= 16:
    print("Grade 16+")
elif index <= 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")
    
