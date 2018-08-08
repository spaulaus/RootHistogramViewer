# ROOT Histogram Viewer
This program allows users to view histograms in a [ROOT](https://root.cern.ch/) file. Users can read histograms while
another program fills the ROOT file. 

# Prerequisites
* cmake 3.10 +
* ROOT 6.13/03 +

# Build Instructions
1. Create a build directory : `mkdir build`
2. Change to the build directory : `cd build`
3. Build the project : `cmake ../`
4. Make/Install the project : `sudo make -j8 install`

*Note* : You may need to export the lib directory to your LD_LIBRARY_PATH.
