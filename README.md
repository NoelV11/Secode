# Secode
- Secode is Insecure Code Detector tool, that analyzes your code, to find Anomalies in your codes say Insecure Functions, that makes your code insecure. 
- But, it doesn't stops there, it as a precautionary measure, provides you with safer function alternatives like for use of `strcpy()` you must use Secure alternative's like `lstrcy()`, that you can implement on your code, to make it Safe and Secure.

# How it's Different
- Secode is powered by a **stringent and comprehensive ruleset**, that can detect vulnerabilities pretty easily, which is updated regularly, 
as more vulnerabilities and exploits emerge.
- Secode enforces the concept of "**Secure Coding" and "Secure Software Devloepment Life Cycle (Secure SDLC)"** which is now gaining much needed attention during the 
development process of any software
- **But, it doesn't stops there, _it provides you with safer function alternatives like for use of `strcpy()` you must use Secure alternative's like `lstrcy()`, that you can implement on your code, to make it Safe and Secure._**

## What Does Secode Scan on The Code(s) You Provide?

Your Code Report provides you information about:-

- Hit count of vulnerable functions (total count)
- Vulnerability at hand
- CWE Number associated with it
- A brief description about the vulnerability
- Risk level of your code (graded from 1 - 5,on the severity basis)
- Secure alternative(s) that you must use which are totally safe from exploits.

## Roadmap
- Dockerize to provide support across all platforms
- Extending Scanning functionalities within the tool to: 
    1. Web Technologies like HTML, CSS, Js and PHP
    2. App Development languages like Java and Kotlin
- GitHub Actions workflows Integration.

## Installation Guide
The manual page (secode.1 or secode.pdf) describes how to use
secode (including its various options) and related information
(such as how it supports CWE).  For example, the `--html` option generates
output in HTML format. The `--help` option gives a brief list of options.

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

# Contributions

We love contributions!  For more information on contributing, see
the file [CONTRIBUTING.md](CONTRIBUTING.md).

# License

Secode is released under the GNU GPL license version 2 or later (GPL-2.0+).
See the [LICENSE](https://github.com/AliasgarSabunwala/Secode/blob/e86853abb4df4906418c806cd0d3dfbc351cf25c/LICENSE) file for license information.
