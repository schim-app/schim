<p align="center">
    <img alt="schim logo" src="res/img/icon.svg" width="80px" />
    <h1 style="border-bottom: none" align="center">Schim</h1>
    <h3 align="center">A schematic editor for the modern computer user</h2>
</p>
---

Welcome to the official Schim repository.

<h3 style="font-weight:normal" align="center"> Schim = <i><b>SCH</b>ematic <b>IM</b>proved</i> </h4>

**Schim** is a tool used to create electrical schematics, mainly for industrial
applications. It aspires to become the standard open source tool for this
purpose.

Schim is available on **Linux** and **Windows**.

#### The perks

* Completely free and open source software
* Comes with a GUI and CLI
* Supports advanced component insertion
* Uses standard filetypes
* Exports into multiple formats
* Has optional Vim keybindings
* Adheres to the [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy)

## Documentation

Schim comes with a very comprehensive user manual and documentation for
developers. Both can be found [here](https://schim.rtfd.io). For more detail on
how the documentation is maintained, have a look at the [README](docs/README.md).

## Installing

### Windows (TODO)

### Linux (TODO)

The program can be found on the repositories of the following distros:

#### Arch Linux

TODO: Make AUR repo

## Building

To build Schim, you need to have the following dependencies installed:

### Build dependencies (TODO check versions)

* [Qt5](https://www.qt.io/)
* [CMake](https://cmake.org/) >= 3.10
* Make >= TODO

### Needed to build manpages and help

* [Python](https://python.org) >= 3.7
* [Sphinx](https://www.sphinx-doc.org/en/master/usage/installation.html) >= 3.0.0 for help
* [python-docutils](https://pypi.org/project/docutils/) for man pages

### Linux

Run the following command:

```shell
make
```

This will create the launch script `_build/release/schim.sh`. The
application can be run like this:

```shell
cd _build/release/;
./schim.sh
```

If you wish to install Schim into a custom directory, run the following command

```shell
make install INSTALL_DIR=<DIR>
```

replacing `<DIR>` with the desired destination directory. This will create a
directory structure inside `<DIR>` mimicking `/` on a Linux system. In this
case, the application can be run as:
```shell
cd <DIR>/bin/;
./schim
```
*Note*: `make install` will deploy the application, including the help files
and man pages.

## Contributing
TODO
