import serial


value = input("Write your words here \n")
value += " "
for i in value:
    word = []
    buffer = ""
    for i in value:
        word.append(buffer)
        buffer = i
    word[0] = value[-1]
    print (word)
    value = word
