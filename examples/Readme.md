### !
This page is currently being updated.


### Invoke analysis
To invoke the analysis, it is recommended to use the latest [scan-build
package](https://github.com/rizsotto/scan-build).  You can either use the
scripts being contained in the LLVM trunk
(`llvm/tools/clang/tools/scan-build-py/bin`) or install the scripts with 
`pip install scan-build`. Note that `make install` LLVM copies old
Perl versions of the scripts into `/usr/local/bin` which therefore 
overwrite the recent pip versions.

### Examples

There are distinct ways to invoke the Clang Static Analyzer.
- Call `scan-build --enable-checker=optin.mpi.MPI-Checker (ninja|make|..)`
  In case of invoking `scan-build` only built sources files are
  analyzed. To do a complete analysis you have to do a complete build. Clean
  your analysis target to ensure this.

Please see `scan-build -h` for further settings.



<!-- #### CMake -->
<!-- Inspect one of the `CMakeLists.txt` files to see how it is set up for MPI and -->
<!-- static analysis.  To invoke the MPI-Checker add [this] -->
<!-- (https://github.com/0ax1/MPI-Checker/blob/master/examples/analyze.sh) to your -->
<!-- `.zshrc`. Then run `checkMPI` in the projects `CMakeLists.txt` path. -->

<!-- #### Make -->
<!-- If you use Make you can do ```scan-build --use-analyzer `which clang` -->
<!-- -enable-checker mpi.MPI-Checker -V make debug ANALYZE=1```. -->
