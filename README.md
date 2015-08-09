# MPI-Checker
A static analysis checker for [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface) code
written in C using Clang's [Static Analyzer](http://clang-analyzer.llvm.org/).

<img src="https://github.com/0ax1/MPI-Checker/blob/master/screenshots/double_wait.jpg" width="500">

## Integrated checks
#### AST-Checks
- `unmatched point to point call`: Point to point schema validation.
- `unreachable call`: Unreachable calls caused by deadlocks from blocking calls.
- `type mismatch`: Buffer type and specified MPI type do not match.
- `invalid argument type`: Non integer type used where only integer types are allowed.
- `collective call in rank branch`: Collective call inside a rank branch.

#### Path-Sensitive-Checks
- `double nonblocking`: Double request usage of nonblocking calls without intermediate wait.
- `double wait` : Double wait on the same request without intermediate nonblocking call.
- `missing wait`: Nonblocking call without matching wait.
- `unmatched wait`: Waiting for a request that was never used by a nonblocking call.

### Point to Point Schema Validation
If only additions for an argument are used operands are accepted as a match if they appear as a permutation.
Else if subtractions are used operands have to be in the same order.
To match point to point calls the checker has to make some assumptions how
arguments are specified. To match variables across cases and functions the same variable name has to be used.
For the rank argument the checker expects the last operator to be "inverse"
and the last operand to be equal.
If none except the last operator are additions also rank operands (excluding the last) are matched
if they appear as a permutation:
<br>`MPI_Isend(&buf, 1, MPI_INT, f() + N + 3 + rank + 1, 0, MPI_COMM_WORLD, &sendReq1);`<br>
`MPI_Irecv(&buf, 1, MPI_INT, N + f() + 3 + rank - 1, 0, MPI_COMM_WORLD, &recvReq1);`<br>

### Rank Variables
Rank variables are identified as such if they get passed to `MPI_Comm_rank`.
Different rank variables can be used within translation unit. They can be
a common variable or contained inside of a struct. Rank variables initialized in
another translation unit are not detected.

```
int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if (rank == 0) {...}
else if (rank == 1) {...}
```

## Prerequisites
Current versions of: `your default compiler`, `zsh`, `svn`, `git`, `cmake`,
`ninja`, `sed` (install gnu-sed with brew if you're on osx)

## Installation
Download [`fullSetup.sh`]
(https://raw.githubusercontent.com/0ax1/MPI-Checker/master/setup/fullSetup.sh),
make it executable with `chmod +x` and run it to setup LLVM 3.7 with
[Clang](http://clang.llvm.org/) and **MPI-Checker**. This will download, config
and build all components in `./llvm37`. <br><br>In "one line": `wget
https://raw.githubusercontent.com/0ax1/MPI-Checker/master/setup/fullSetup.sh &&
chmod +x fullSetup.sh && ./fullSetup.sh && rm fullSetup.sh` <br><br>If you want
libcxx or the compiler-rt to be included you can comment them in, removing
`#` from `# svn co (libcxx|compiler-rt)` in `fullSetup.sh`.

If you have the LLVM 3.7 source already, `cd` to the top of the repository,
execute [`addMPI-Checker.sh`]
(https://raw.githubusercontent.com/0ax1/MPI-Checker/master/setup/addMPI-Checker.sh)
and then rerun your build system manually.

Finally add these locations to your search path:<br>
`llvm37/build/release/bin`<br>
`llvm37/repo/tools/clang/tools/scan-build`<br>
`llvm37/repo/tools/clang/tools/scan-view`<br>

## Update
To update MPI-Checker call `git pull` in `llvm37/repo/tools/clang/lib/StaticAnalyzer/Checkers/MPI-Checker`.
Then run `ninja && ninja ClangUnitTests` in `llvm37/build/release`.


## Examples
Have a look at the [examples folder](https://github.com/0ax1/MPI-Checker/tree/master/examples).

## Tests
See the [tests folder](https://github.com/0ax1/MPI-Checker/tree/master/tests).

## Limitations
- Point to point communication must be enclosed within the scope of a translation unit.
  This stems from the general limitation of Clang's Static Analyzer to analyze one translation unit in isolation.
- Unreachable calls can only be detected if caused by blocking MPI calls. The reason for this is that
  point to point schema validation can only be achieved by non path sensitive (ast) analysis
  while request var usage must be checked path sensitive. So deadlocks caused by waits can not be detected.
- There can't be any assumptions made at compile time about `MPI_Waitany` and `MPI_Waitsome` since their effect
  depends on what is done at runtime. Because of that they are not taken into account.
- Rank variables initialized in another translation unit cannot be detected.
- The analysis is limited to C. Analyzing C++ code is currently not supported.
