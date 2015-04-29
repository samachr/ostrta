Readme file for the OSTRTA document search engine

1.0 Compiling

Compiling is easiest on a linux based operating system using g++. Simply execute
g++ main.cpp. In windows, one could add the source files to an empty project and
compile. Simple. Just one file.

2.0 Running

To run the OSTRTA program, simply execute the compiled executable and follow
instructions given. Basically it will automatically index the database, granted
correct file structure, and be ready for queries. Input queries and hit enter
and a list of pages will be given. Boolean operators are allowed, with no limit
on the number of chained booleans. However, the boolean operators will not follow
order of operations, but simply chain commands together left to right.

3.0 File Structure

The minimum files required to run the program is the compiled executible and the
folder full of documents, named documents. It is important that the documents 
begin with cranfield00 and do not contain any file extensions, or the search 
engine will not recognize them.

On my machine, it looks like this.

    ./documents
    main.cpp
    a.out
    readme.txt
    

