# Installing Secode

You can install Secode a number of different ways.
Choose the approach that's most convenient for you!
The options (described below) are (1) pip, (2) package for Unix-like system, (3) source install, and (4) run directly.

## Prior to install

Meet the requirements

~~~~
apt update | apt upgrade 
~~~~

## 1. Cloning the repository


~~~~
git clone https://github.com/NoelV11/Secode.git
~~~~

## 2. PIP

For many, the simple approach is to first install Python
(2.7 or something reasonable in the 3.X series).
Then use `pip` to install Secode
(this will normally download the package):

![1](https://user-images.githubusercontent.com/77625109/147364745-2a377835-3625-4691-91b9-64f2737e0584.png)

One advantage for using pip is that you'll generally get the
*current* released version.

## 3.PACKAGE FOR UNIX-LIKE SYSTEM (including Cygwin):

If you use an RPM-based system (e.g., Red Hat) or deb-based system
(e.g., Debian), you can use their respective RPM or debian installation
program and just install it; then ignore the rest of these instructions.
For a ports-based system where you have a current port, just use that.

This will work out-of-the-box; it may not be the most recent version.

One way to accomplish this is:

P.S Make sure that you have Python 3.0 or higher
~~~~
sudo apt install secode
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
