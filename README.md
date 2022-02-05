# Secode

[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

- Secode is an insecure Code Detector tool, that analyzes your code, to find anomalies, say Insecure Functions in your codes, that makee it insecure. 
- But, it doesn't stop there - as a precautionary measure, Secode provides you with safer function alternatives - like for the use of `strcpy()` which is vulnerable to Buffer Overflow,you must instead use secure alternative(s) like `lstrcy()`, that you can implement on your code, to make it secure.

# How is it different?
- Secode is powered by a **stringent and comprehensive ruleset**, that can detect vulnerabilities pretty easily, which will be updated regularly, 
as more vulnerabilities and exploits emerge.
- Secode enforces the concept of "**Secure Coding" and "Secure Software Devloepment Life Cycle (Secure SDLC)"** which is now gaining much needed traction during the 
development process of any software.

## What does a Secode scan on the Code(s) you provide,give you?

Your Code Report provides you information about:-

- Hit count of vulnerable functions (total count)
- Vulnerability at hand
- CWE Number associated with it
- A brief description about the vulnerability
- Risk level of your code (graded from 1 - 5,on the severity basis)
- Secure alternative(s) that you must use which are totally safe from exploits.


## Installation Guide

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

~~~~
python3 secode.py Vulnerable_1.c >> Results.txt
~~~~

## Output Formats supported

- txt and
- CSV

## Output Structure

This is the code report one gets after scanning their .c file(in this case,the analyzed code contains insecure functions,which we had previously identified and compiled in the ruleset file)

![H1](https://user-images.githubusercontent.com/77625109/152655310-0899ba05-f684-4e5c-89ff-bff3c99e91c6.png)

From the sample output we can note the following:-

- Hits - number of insecure functions detected
- An appropriate countermeasure function or best practice,that can be used to remove the vulnerable function
- Lines Analyzed - derived from the total number of lines in the code
- Severity levels - graded from 1 to 5
- Risk level of analyzed code

Please note that exploits are attempted everyday,leading to functions being vulnerable.So there may be chances that a secure function today,may become insecure tommorow


# Under the hood

More technically, secode uses lexical scanning to find tokens
(such as function names) that suggest likely vulnerabilities, estimates their
level of risk (e.g., by the text of function calls), and reports the results.
secode does not use or have access to information about control flow,
data flow, or data types.  Thus, secode will necessarily
produce many false positives for vulnerabilities and fail to report
many vulnerabilities.  On the other hand, secode can find
vulnerabilities in programs that cannot be built or cannot be linked.
secode also doesn't get as confused by macro definitions
and other oddities that more sophisticated tools have trouble with.

## Roadmap
- Dockerize it to provide support across all platforms
- Extending Scanning functionalities within the tool to: 
    1. Web Technologies like HTML, CSS, Js and PHP
    2. App Development languages like Java and Kotlin
- GitHub Actions workflows Integration.

# Contributions

We love contributions!  For more information on contributing, see
the file [CONTRIBUTING.md](CONTRIBUTING.md).

# License

Secode is released under the GNU GPL license version 2 or later (GPL-2.0+).
See the [LICENSE](LICENSE) file for license information.
