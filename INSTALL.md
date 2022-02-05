# Installing Secode

You can install Secode by cloning it,using git

## Prior to install

Meet the requirements

~~~~
apt update | apt upgrade 
~~~~

## Cloning the repository


~~~~
git clone https://github.com/NoelV11/Secode.git
~~~~

## Tool Operation

You can also simply run the program in the directory you've unpacked it
into.   It's a simple Python program, just type into a command line:

![2](https://user-images.githubusercontent.com/77625109/147364977-ea883dfd-81af-45ff-949b-5fa0f5a221c3.png)

## Output Formats supported

- txt and
- CSV

For example:-


![3](https://user-images.githubusercontent.com/77625109/147365058-d9d8c2ef-fbea-4e64-a504-a11b77a266bc.png)

Output Format

This is the code report one gets after scanning their .c file(in this case,the analyzed code contains insecure functions,which we had previously identified and compiled in the ruleset file)

![3](https://user-images.githubusercontent.com/77625109/147365491-f5684c4f-690a-419f-8033-d90bc060bd2e.png)

From the sample output we can note the following:-

- Hits - number of insecure functions detected
- Lines Analyzed - derived from the total number of lines in the code
- Severity levels - graded from 1 to 5
- Risk level of analyzed code

Please note that exploits are attempted everyday,leading to functions being vulnerable.So there may be chances that a secure function today,may become insecure tommorow
