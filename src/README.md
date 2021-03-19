<div align="center">
<pre>
.xNMMMMMMMMMMMMMMMMMMMNx.
XMMMMMMMMMMMMMMMMMMMMMMMX
MMMMMMMMMMMMMMMMMMMMMMMMM
dddddddddddKMMMMOdddddddd
           dMKL         
MMMMMMMM:  ˘   ,kWMMMMMMM
MMMMMMMM:   'dNMMMMMMMMMM
MMMMMMMM:  dMMMMMMMMMMMMM
KMMMMMMM:  dMMMMMMMMMMMMK
 oNMMMMM:  dMMMMMMMMMMNd 
</pre>
<h1> The Source </h1>
</div>

This is where Schim's source lives. A detailed overview of this subdirectory can
be found in the [developer documentation](https://schim.rtfd.io/en/latest/dev/getting-started.html).

Here we will just briefly explain how the build process works.

It's simple. When you call `cmake` in this directory, it will build Schim using
the contents of `CMakeLists.txt`. Inside this file, the file `common.cmake` is
included to build the part that is common between the `schim` executable and the
`unittests` executable (see [`tests/README.md`](../tests/README.md)).

You should not build Schim by calling `cmake` directly. This would not create
the resource files which are necesary for Schim to work correctly.  Instead, you
should call `make` from the root directory of the project. This will create the
necessary additional files.

