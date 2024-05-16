Hello! This is a bit confusing to run since I have a mac and the user running this may have boost stored in a 
different (more normal) place.

To Run:
1.) Set your boost directory in terminal: export BOOST_HOME="directory"
for me that was: export BOOST_HOME=/usr/local
(mine is in /usr/local but please update this to what your boost directory is)

2.) Set your boost lib directory in terminal: export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:"directory"
for me that was: export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/usr/local/lib
For some reason for me, I have to do this for it to run it can't find it on its own from the make. 

3.) Lastly run in terminal: ./jumping_jim
And everything should work

IFF this is not working you can also just update the include files in the program to where your boost is
and run the classic "g++ -std=c++17 jumping_jim.cpp" and then "./a.out" but this should work! I have tested it many 
times on mine.

I do not have issues running it this way, but please let me know if there are any problems running it!

