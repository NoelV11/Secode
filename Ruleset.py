/*
   Stringent and comprehensive ruleset, for C/ C++ programming language

    !!! This ruleset wiil be updated frequently !!!
*/

C-Ruleset = {"gets|_getts":
	(normal, 5, "Does not check for buffer overflows (CWE-120, CWE-20)",
	"Use fgets() or get_s instead", "buffer", "", {'input': 1}),

	"sizeof(sizeof())": "Indicates the presence of bugs,if the size of the input is not accounted for","Use sizeof()", "buffer", "" , {'input': 1}),

	"return" :
	In case memory that is allocated is not freed at the end of the program,then it raises a chance of Memory Leak,"Use free", "buffer", "" , {'input': 1}),

	"bsearch" : "Usage of this insecure function will place unknown demands on stack resources and undefined behaviour from the initialized stack","Use bsearch_s ", "buffer", "" , {'input': 1}),

	"qsort" : "Usage of this insecure function will place unknown demands on stack resources and undefined behaviour from the initialized stack","Use qsort_s ", "buffer", "" , {'input': 1}),

	 "getpass":
	 (normal, 4," "getpass","This function is obsolete and not portable.It takes input of password from user,but leaves the cleartext password visible in the process' address space".In addition, some implementations overflow buffers. (CWE-676, CWE-120, CWE-20)",,
	 "misc", "", {'input': 1}),

	"atol | _wtoi | _wtoi64 " :(normal, 2,
	 "Unless checked, the resulting number can exceed the expected range "
	 "(CWE-190)",
	 "If source untrusted, check both minimum and maximum, even if the"
	 " input had no minus sign (large numbers can roll over into negative",
	 "integer", "dangers-c", {}),

	 "printf|vprintf|vwprintf|vfwprintf|_vtprintf|wprintf":
	 (c_printf, 4,
	 "If format strings can be influenced by an attacker, they can be exploited (CWE-134)",
	 "Use a constant for the format specification",
	 "format", "", {}),

	"getpw":
	(normal, 4,
	 "This function is dangerous; it may overflow the provided buffer. It extracts data from a 'protected' area, but most systems have many commands to let users modify the protected area, and it's not always clear what their limits are. Best to avoid using this function altogether (CWE-676, CWE-120)",
	 "Use getpwuid() instead",
	 "buffer", "", {}),

	"wscnpy" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use wcsncpy_s", "buffer", "" , {'input': 1}),

	 "cuserid":
	 (normal, 4,
	 " When passed a non-null parameter, there's a risk of a buffer overflow if the passed-in buffer is not at least L_cuserid characters long (CWE-120)",
	 "Use getpwuid(geteuid()) and extract the desired information instead",
	 "misc", "", {}),

	 "vfprintf|_ftprintf|_vftprintf|fwprintf|fvwprintf":
	 (c_printf, 4,
	 "If format strings can be influenced by an attacker, they can be exploited (CWE-134)",
	 "Use a constant for the format specification",
	 "format", "", {'format_position': 2}),

	"strncat" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use strncat_s", "buffer", "" , {'input': 1}),

	"wcscat" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use wcscat_s ", "buffer", "" , {'input': 1}),

	 "getlogin":
	 (normal, 4,
	 "It's often easy to fool getlogin. Sometimes it does not work at all, because some program messed up the utmp file. Often, it gives only the first 8 characters of the login name. The user currently logged in on the controlling tty of our program need not be the user who started it. Avoid getlogin() for security-related purposes (CWE-807)",
	 "Use getpwuid(geteuid()) and extract the desired information instead",
	 "misc", "", {}),

	"strcat" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use strcat_s ", "buffer", "" , {'input': 1}),

	"wcslen" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use wcslen_s ", "buffer", "" , {'input': 1}),

	"strlen | _mbslen | _mbstrlen | StrLen| lstrlen" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use strlen_s ", "buffer", "" , {'input': 1}),

	"wcscpy" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use wcscpy_s ", "buffer", "" , {'input': 1}),

	"_itoa | _itow | _i64toa | _ui64toa | _ultoa " : "These functions can be exploited to bypass security controls in order to execute malicious code or cause a system crash.","Use_itoa_s, "buffer", "" , {'input': 1}),

	" _i64tow | _ui64tot | _ui64tow | _ultot | _ultow" : "These functions can be exploited to bypass security controls in order to execute malicious code or cause a system crash.","Use_itow_s, "buffer", "" , {'input': 1}), 

	"abort" : "It has implementation-defined behaviors such as destroying of initialized variables and threads, and should therefore be avoided","Use _exit ", "buffer", "" , {'input': 1}),

	 "AddAccessAllowedAce":
	 (normal, 3,
	 "This doesn't set the inheritance bits in the access control entry (ACE) header (CWE-732)",
	 "Make sure that you set inheritance by hand if you wish it to inherit",
	 "misc", "", {}),

	"exit" : "It has implementation-defined behaviors such as destroying of initialized variables and threads, and should therefore be avoided","Use _exit ", "buffer", "" , {'input': 1}),

	"system" : "It has implementation-defined behaviors such as destroying of initialized variables and threads, and should therefore be avoided","Use execve ", "buffer", "" , {'input': 1}),


	"strccpy|strcadd":
	 (normal, 1,
	 "Subject to buffer overflow if buffer is not as big as claimed (CWE-120)",
	 "Ensure that destination buffer is sufficiently large",
	 "buffer", "", {}),


	"_vsnprintf | _vsnwprintf | _vsntprintf | wvnsprintf | wvnsprintfA | wvnsprintfW" : "It's usage can lead to Buffer Overflow vulnerability","Use vsntprintf_s ", "buffer", "" , {'input': 1}),

	"SetSecurityDescriptorDacl":
	 (c_hit_if_null, 5,
	 "Never create NULL ACLs; an attacker can set it to Everyone (Deny All Access), "
	 "which would even forbid administrator access (CWE-732)",
	 "",
	 "misc", "", {'check_for_null': 3}),

	"wvsprintf | wvsprintfA | wvsprintfW | vsprintf | _vstprintf | vswprintf" : "It's usage can lead to Buffer Overflow vulnerability and unexpected program errors ","Use _vstprintf_s ", "buffer", "" , {'input': 1}),

	"fprintf | snprintf" : "It's usage can lead to vulnerabilities,such as Information Leakage and Mmeory Overwriting","Use fgets ", "buffer", "" , {'input': 1}),

	 "LoadLibrary|LoadLibraryEx":
	 (normal, 3, "Ensure that the full path to the library is specified, or current directory may be used (CWE-829, CWE-20)",
	 "Use registry entry or GetWindowsDirectory to find library path, if you aren't already",
	 "misc", "", {'input': 1}),

	"cftime" : "No bounds checking","Use strftime ", "buffer", "" , {'buf',sizeof(buf),fmt,&tm: 4}),

	"atof" : 
	"It is a function, that converts strings to numbers, and has undefined behavior when the strings cannot be converted, and should therefore be avoided.","Use strtol", "buffer", "" , {'input': 1}),

	"chdir" : "Race Conditions take place using this function and can cause unexpected behaviours","Use fchdir ", "buffer", "" , {'input': 1}),

	 "streadd|strecpy":
	 (normal, 4,
	 "This function does not protect against buffer overflows (CWE-120)",
	 "Ensure the destination has 4 times the size of the source, to leave room for expansion",
	 "buffer", "dangers-c", {}),

	"chmod" : "It causes race conditions","Use fchmod ", "buffer", "" , {'input': 1}),

	"wordexp" : "It is vulnerable to Shell Code Injection","Use glob ", "buffer", "" , {'input': 1}),

	 "strtrns":
	 (normal, 3,
	 "This function does not protect against buffer overflows (CWE-120)",
	 "Ensure that destination is at least as long as the source",
	 "buffer", "dangers-c", {}),

	"tempnam" : "Race Conditions occur with usage of this insecure function","Use mkstem ", "buffer", "" , {'input': 1}),

	"notnull" : 
	"It is a function, that converts strings to numbers, and has undefined behavior when the strings cannot be converted, and should therefore be avoided.","Do not assign non zero values to notnull function", "buffer", "" , {'input': 1}),

	 "EnterCriticalSection | InitializeCriticalSection":
	 (normal, 3, "On some versions of Windows, exceptions can be thrown in low-memory situations",
	 "Use InitializeCriticalSectionAndSpinCount instead",
	 "misc", "", {}),

	"fopen" : 
	"This insecure function can cause Resource Exhaustion,if resources are not released after use .","Use fclose", "buffer", "" , {'input': 1}),

	"sprintf | _snwprintf | _snprintf | _sntprintf | nsprintf" : 
	"It is a function, that converts strings to numbers, and has undefined behavior when the strings cannot be converted, and should therefore be avoided.","Use snprintf", "buffer", "" , {'input': 1}),

	"malloc ":"By using malloc(),an attacker may be able to create a situation where the computed buffer size is less than the data to be placed in it.","Use talloc ", "buffer", "" , {'input': 1}),

	 "equal|mismatch|is_permutation":
	 (cpp_unsafe_stl,
	 # Like strlen, this is mostly a risk to availability; at worst it
	 # often causes a program crash.
	 1,
	 "Function does not check the second iterator for over-read conditions (CWE-126)",
	 "This function is often discouraged by most C++ coding standards in favor of its safer alternatives provided since C++14. Consider using a form of this function that checks the second iterator before potentially overflowing it",
	 "buffer", "", {}),
	"rand" : 
	"When software generates predictable values in a context requiring unpredictability, it may be possible for an attacker to guess the next value that will be generated, and use this guess to impersonate another user or access sensitive information..","Use randombytes_uniform", "buffer", "" , {'input': 1}),

	"strncpy" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use strncpy_s", "buffer", "" , {'input': 1}),
	 	 "RpcImpersonateClient|ImpersonateLoggedOnUser|CoImpersonateClient|""ImpersonateNamedPipeClient|ImpersonateDdeClientWindow|ImpersonateSecurityContext|"
	 "SetThreadToken":
	 (normal, 4, "If this call fails, the program could fail to drop heightened privileges (CWE-250)",
	 "Make sure the return value is checked, and do not continue if a failure is reported",
	 "access", "", {}),

	"scanf | fscanf ":"There is no limit to the length of data,that can be input to the function","Use scanf("%[max array size-1]s,variable", "buffer", "" , {'input': 1}),

	"memset |memcpy | memove | CopyMemory | bcopy |RtlCopyMemory " : "memset() function when called,will be removed by the compiler. The buffer is no longer used after the memset() call. And the compiler removes the function call for the sake of optimization.Highlighted in CWE-788 and CWE-805","Use calloc or memcpy_s ", "buffer", "" , {'input': 1}),

	 "recv|recvfrom|recvmsg|fread|readv":
	 (normal, 0, "Function accepts input from outside program (CWE-20)",
	 "Make sure input data is filtered, especially if an attacker could manipulate it",
	 "input", "", {'input': 1}), 

	 "getopt|getopt_long":
	 (normal, 3,
	 "Some older implementations do not protect against internal buffer overflows (CWE-120, CWE-20)",
	 "Check implementation on installation, or limit the size of all string inputs",
	 "buffer", "dangers-c", {'input': 1}),

	"wmemcpy | " :"This insecure function copies objects from on-heap to off-heap.Highlighted in CWE 120 and 121","Use wmemcpy_s ", "buffer", "" , {'input': 1}),

	 "usleep":
	 (normal, 1,
	 "This C routine is considered obsolete (as opposed to the shell command by the same name). The interaction of this function with SIGALRM and other timer functions such as sleep(), alarm(), setitimer(), and nanosleep() is unspecified (CWE-676)",
	 "Use nanosleep(2) or setitimer(2) instead",
	 "obsolete", "", {}),

	"_splitpath, | tsplitpath, | _ splitpath" : "Split path function succeeds in breaking a file path into components,but presents a Buffer Overrun vulnerability ","Use _splitpath_s ​", "buffer", "" , {'input': 1}),

	"memalign":
	 (normal, 1,
	 "On some systems (though not Linux-based systems) an attempt to free() results from memalign() may fail. This may, on a few systems, be exploitable. Also note that memalign() may not check that the boundary parameter is correct (CWE-676)",
	 "Use posix_memalign instead (defined in POSIX's 1003.1d). Don't switch to valloc(); it is marked as obsolete in BSD 4.3, as legacy in SUSv2, and is no longer defined in SUSv3. In some cases, malloc()'s alignment may be sufficient",
	 "free", "", {}),

	"lstrcpyn|wcsncpy|_tcsncpy|_mbsnbcpy":
	 (c_buffer,
	 1, # Low risk level, because this is often used correctly when FIXING security
	 # problems, and raising it to a higher risk levle would cause many false
	 # positives.
	 "Easily used incorrectly; doesn't always \\0-terminate or "
	 "check for invalid pointers [MS-banned] (CWE-120)",
	 "",
	 "buffer", "", {}),

	 "g_get_tmp_dir | "g_get_tmp_dir":
	 (normal, 3, "This insecure function returns untrustable input if the environment can be"
	 "set by an attacker. It can have any content and length, "
	 "and the same variable can be set more than once (CWE-807, CWE-20)",
	 "Check environment variables carefully before using them",
	 "buffer", "", {'input': 1}),
	":
	 (normal, 3, "This insecure function returns untrustable input if the environment can be"
	 "set by an attacker. It can have any content and length, "
	 "and the same variable can be set more than once (CWE-807, CWE-20)",
	 "Check environment variables carefully before using them",
	 "buffer", "", {'input': 1}),

	 "getenv|curl_getenv":
	 (normal, 3, "Environment variables are untrustable input if they can be"
	 " set by an attacker. They can have any content and"
	 " length, and the same variable can be set more than once (CWE-807, CWE-20)",
	 "Check environment variables carefully before using them",
	 "buffer", "", {'input': 1}),

	"_i64tow | _ui64tot | _ui64tow | _ultot | _ultow" : "These functions can be exploited to bypass security controls in order to execute malicious code or cause a system crash.","Use_itow_s, "buffer", "" , {'input': 1}),

	"snscanf | snwscanf | _sntscanf | wscanf | _tscanf | swscanf | _stscanf" : "Usage of these input functions does not ensure data sanitization or check of it's length","Use _sscanf_s ", "buffer", "" , {'input': 1}),

	"wscncat" : "Error can lead to a security vulnerability, for instance, giving access to sensitive data or allowing arbitrary code execution.","Use wscncat_s ", "buffer", "" , {'input': 1}),

	 "ulimit":
	 (normal, 1,
	 "This C routine is considered obsolete (as opposed to the shell command by the same name, which is NOT obsolete) (CWE-676)",
	 "Use getrlimit(2), setrlimit(2), and sysconf(3) instead",
	 "obsolete", "", {}),

	 "gsignal|ssignal":
	 (normal, 2,
	 "These functions are considered obsolete on most systems, and very non-portable (Linux-based systems handle them radically different, basically if gsignal/ssignal were the same as raise/signal respectively, while System V considers them a separate set and obsolete) (CWE-676)",
	 "Switch to raise/signal, or some other signalling approach",
	 "obsolete", "", {}),

	"_tcsncat | _mbsncat | _mbsnbcat | StrCatN | StrCatNA | StrCatNW | StrNCatA | StrNCatW | lstrncat | lstrcatnA | lstrcatnW | lstrcatn | _fstrncat" : "This insecure function is vulnerable to Buffer Overflow","Use strncat_s ​", "buffer", "" , {'input': 1}),

	"strcpy | stpcpy":
	 (c_buffer, 4,
	 "Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120)",
	 "Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused)",
	 "buffer", "", {}),

	"strcmp" : "It's usage causes a string to truncate,leading to a Buffer Overflow attack","Use strncmp ", "buffer", "" , {'input': 1}),

	"mktemp" : "Though this function helps us to create a temperory file,it causes race conditions","Use mkstemp ", "buffer", "" , {'input': 1}),

	"lscat | stat | fstatat " : "Too dangerous to use in libraries or privileged commands and daemons because they find the executable by searching the directories in the PATH environment variable, which is under the complete control of the user. They should be avoided for most other programs and are vulnerable to undpredictibilty,due to race conditions","Use execl , execv or execve or mkstemp", "buffer", "" , {'input': 1}),

	"dup ":"Memory leakage ","Use fcntl", "buffer", "" , {'input': 1}),

	"makepath | _tmakepath | _makepath | _wmakepath" : "Makepath creates a composite path string from individual components and is vulnerable to file manipulation attack instance .Highlighted in CWE-26 and CWE-367","Use _makepath_s ​", "buffer", "" , {'input': 1}),

	"strtok | _tcstok | wcstok | _mbstok" : "This insecure function is vulnerable to Buffer Overflow.Highlighted in CWE-741","Use strtok_s ​", "buffer", "" , {'input': 1}),

	"EVP_des_ecb|EVP_des_cbc|EVP_des_cfb|EVP_des_ofb|EVP_desx_cbc":
	 (normal, 4,
	 "DES only supports a 56-bit keysize, which is too small given today's computers (CWE-327)",
	 "Use a different patent-free encryption algorithm with a larger keysize, "
	 "such as 3DES or AES",
	 "crypto", "", {}),

	 "crypt|crypt_r":
	 (normal, 4,
	 "The crypt functions use a poor one-way hashing algorithm; "
	 "since they only accept passwords of 8 characters or fewer "
	 "and only a two-byte salt, they are excessively vulnerable to "
	 "dictionary attacks given today's faster computing equipment (CWE-327)",
	 "Use a different algorithm, such as SHA-256, with a larger, "
	 "non-repeating salt",
	 "crypto", "", {}),

	 "drand48|erand48|jrand48|lcong48|lrand48|mrand48|nrand48|random|seed48|setstate|srand|strfry|srandom|g_rand_boolean|g_rand_int|g_rand_int_range|g_rand_double|g_rand_double_range|g_random_boolean|g_random_int|g_random_int_range|g_random_double|g_random_double_range":
	 (normal, 3,
	 "This function is not sufficiently random for security-related functions such as key and nonce creation (CWE-327)",
	 "Use a more secure technique for acquiring random values,like randombytes_uniform",
	 "random", "", {}),

	 "CreateProcess":
	 (c_hit_if_null, 3,
	 "This causes a new process to execute and is difficult to use safely (CWE-78)",
	 "Specify the application path in the first argument, NOT as part of the second, "
	 "or embedded spaces could allow an attacker to force a different program to run",
	 "shell", "", {'check_for_null': 1}),

	 "CreateProcessAsUser|CreateProcessWithLogon":
	 (normal, 3,
	 "This causes a new process to execute and is difficult to use safely (CWE-78)",
	 "Especially watch out for embedded spaces",
	 "shell", "", {}),

	 "execl|execlp|execle|execvp|popen|WinExec|ShellExecute":
	 (normal, 4,
	 "This causes a new program to execute and is difficult to use safely (CWE-78)",
	 "try using a library call that implements the same functionality "
	 "if available",
	 "shell", "", {}),

	 "GetTempFileName":
	 (normal, 3,
	 "Temporary file race condition in certain cases "
	 "(e.g., if run as SYSTEM in many versions of Windows) (CWE-377)",
	 "",
	 "tmpfile", "avoid-race", {}),

	 "syslog":
	 (c_printf, 4,
	 "If syslog's format strings can be influenced by an attacker, "
	 "they can be exploited (CWE-134)",
	 "Use a constant format string for syslog",
	 "format", "", {'format_position': 2}),

	 "access": 
	 (normal, 4,
	 "This usually indicates a security flaw. If an "
	 "attacker can change anything along the path between the "
	 "call to access() and the file's actual use (e.g., by moving "
	 "files), the attacker can exploit the race condition (CWE-362/CWE-367!)",
	 "Set up the correct permissions (e.g., using setuid()) and "
	 "try to open the file directly",
	 "race",
	 "avoid-race#atomic-filesystem", {}),

	 "getchar|fgetc|getc|read|_gettc":
	 (normal, 1,
	 "Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20)",
	 "",
	 "buffer", "dangers-c", {'input': 1}),

	 "getwd":
	 (normal, 3,
	 "This does not protect against buffer overflows "
	 "by itself, so use with caution (CWE-120, CWE-20)",
	 "Use getcwd instead",
	 "buffer", "dangers-c", {'input': 1}),

	 "realpath":
	 (normal, 3,
	 "This function does not protect against buffer overflows, "
	 "and some implementations can overflow internally (CWE-120/CWE-785!)",
	 "Ensure that the destination buffer is at least of size MAXPATHLEN, and"
	 "to protect against implementation problems, the input argument "
	 "should also be checked to ensure it is no larger than MAXPATHLEN",
	 "buffer", "dangers-c", {}),

	"wsclen|_tcslen|_mbslen":
	 (normal,
	 # Often this isn't really a risk, and even when it is, at worst it
	 # often causes a program crash (and nothing worse).
	 1,
	 "Does not handle strings that are not \\0-terminated; "
	 "if given one it may perform an over-read (it could cause a crash "
	 "if unprotected) (CWE-126)",
	 "Use strlen_s",
	 "buffer", "", {}),

	 "sscanf|vsscanf|vfscanf|_ftscanf|fwscanf|vfwscanf|vswscanf":
	 (c_scanf, 4,
	 "The scanf() family's %s operation, without a limit specification, "
	 "permits buffer overflows (CWE-120, CWE-20)",
	 "Specify a limit to %s, or use a different input function,like puts() or putchar()",
	 "buffer", "", {'input': 1, 'format_position': 2})
