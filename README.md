# ROOT Histogram GUI
This is prototype code addressing [PAASS-LC Issue 126](https://github.com/spaulaus/paass-lc/issues/126). 
It creates a GUI interface that users can use to read online and offline histograms from a `.root` file.
Think of it like the TBrowser, but it automatically refreshes the keys when you try plot again.  

# Prerequisites
* cmake 3.5 +
* ROOT 6.13/03 +

# Build Instructions
1. Create a build directory : `mkdir build`
2. Change to the build directory : `cd build`
3. Build the project : `cmake ../`
4. Make the project : `make -j8`
5. Run the program : `./issue126`