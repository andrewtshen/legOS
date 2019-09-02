import os

with open("download.bin", mode='rb') as file: # b is important -> binary
    fileContent = file.read()

os.system("printf 3\r > guest.in");
os.system("printf 2\r > guest.in");

os.system("printf " + str(len(fileContent)) + "\r > guest.in");
print("size: " + str(len(fileContent)))

os.system("cat download.bin > guest.in");
# print(fileContent)

os.system("printf 4\r > guest.in");
os.system("printf 2\r > guest.in");