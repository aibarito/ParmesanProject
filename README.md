# Purpose

This repository contains a modified parmesan fuzzer which is able to show the efficiency of its fuzzing strategy. With this modified version, we can check both the number of times parmesan fuzzer tried to reach its desired functions and the number of times it actually reached them.

### Sample output of fuzzing **objdump** binary for one day:

<img width="500" alt="STATS" src="https://user-images.githubusercontent.com/56426592/156591663-e9be92f3-4954-448c-9f73-93ac9ee7208d.png">
 
*This statistics shows that out of 147539 binary executions that tried to reach some specified function, 98602 of them did not get into desired places. The buckets field is only needed if either of number of executions or number of reached times is larger than the maximum of u64, in this case the number of buckets shows how many times a result overflowed the u64. For example, 1234 binary executions and 2 buckets means that total binary executions was u64 x 2 + 1234*

## General overview of implementation

<img width="863" alt="Screen Shot 2022-03-03 at 21 50 47" src="https://user-images.githubusercontent.com/56426592/156600574-b8d26cb4-5916-4cc6-b5bc-bc4dc8cbad45.png">

## Changes made to original parmesan

### 1) Instrument the fuzzing binary with new llvm pass

During the compilation of required files, we add aibarFunc() in the beginning of every function in the application. The code in the aibarFunc() allows us to access the shared memory associated with each function. In the result, in each execution of instrumented binary, we will access the shared memory binded to those functions that were reached.

The contens of aibarFunc() can be found in *aibar-measurement/llvm_mode/aibar_rt/aibar/aibar.cc*

To instrument the binary with new pass (by adding aibarFunc() to each function), we have to add the flags stored in "aibarThings" during the compilation of fuzzed application, as it is shown in the *aibar-measurement/tools/compile_bc.py* file. In this file you will see "aibarThings" contents and how it is added to compile files.

### 2) Modify the parmesan source files

The only files that were modified are *aibar-measurement/fuzzer/src/fuzz_loop.rs* and *aibar-measurement/fuzzer/src/executor/executor.ls*. In the fuzz_loop.rs we add initializers for shared memory variables and the check of their final contents at the end of the loop. Also, we added getNameStart() function that takes cmpid as argument and returns the name of the function that is associated with this cmpid. We need the name of the function because we bind the shared memory variable to the function names.

The executor.rs has only a small change - we add a localCounter member to the Executor class which we increase before every execution of the fuzzed binary. This localCounter is used to find out the number of times the binary was executed and thus, the number of times fuzzer tried to reach the desired function. 


## Prerequisities

There are some places where I use constant pathes strings like /data/shm or /data/infos. Therefore, to avoid the running issues you have to have these directories present:
* /data/infos - in this directory we create targetsInfo.txt file that contains mapping from bbid to function name, and it is accessed in aibar-measurement/fuzzer/src/fuzz_loop.rs and aibar-measurement/tools/llvm-diff-parmesan/id-assigner-pass/src/IDAssigner.cpp
* /data/shm - in this directory we create shared memory files that are accessed by my added pass (in the aibarFunc()) and accessed in the fuzz_loop.rs
* /parmesan/misc/workdir/build/cmp.map - this cmp.map file is created by the parmesan after it is done compiling fuzzing application. The cmp.map contains mapping from cmpid to bbid and this file is read in the /aibar-measurement/fuzzer/src/fuzz_loop.rs. Thus, the cmp.map file must be present in the misc/workdir/build/, or if the cmp.map is located elsewhere, then you should change "/parmesan/misc/workdir/build/cmp.map" to its real path in the fuzz_loop.rs

### Usaful commands:

* To build the parmesan including the passes and its fuzzer u need look at: aibar-measurement/build/build.shared
* To compile the target application and created all files required for fuzzing u need to look at: aibar-measurement/misc/compile_bc.py

## Notes

It should be carefully handled when adding the aibarFunc() during the compilation. Consider these steps:

<img width="1016" alt="Screen Shot 2022-03-10 at 19 37 37" src="https://user-images.githubusercontent.com/56426592/157673067-22c80f26-c71e-4640-acba-5262ff5a7c08.png">


Steps 1 and 3 are needed to find sanitized blocks by comparing the sanitized version of binary and non-sanitized in step 6

Steps 2 and 4 are needed to generate binaries used by the parmesan fuzzer

Then, when adding aibarFunc() to the binaries, you should consider the fact that addition of new function can also affect the pass added by ASAN and pass added by Angora
