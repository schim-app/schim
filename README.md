
<div align="center">
    <img alt="schim logo" src="res/img/icon.svg" width="80px" />
    <h1>Schim</h1>
    <h3>A schematic editor for the modern computer user</h3>
</div>

---

[![Docs](https://img.shields.io/readthedocs/schim?color=blue&label=ReadTheDocs)](https://schim.rtfd.io)
[![License](https://img.shields.io/badge/license-GPL%203-blueviolet)](https://github.com/schim-app/schim/blob/master/LICENSE)

Welcome to the official Schim repository.

<div align="center">
<h3 style="font-weight:normal"> Schim = <i><b>SCH</b>ematic <b>IM</b>proved</i> </h3>
</div>

Schim is a tool used to create electrical schematics, mainly for industrial
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

Schim comes with a very comprehensive user manual and developer documentation.
Both can be found [here](https://schim.rtfd.io). The documentation is actively
maintained [here](https://github.com/schim-app/schim-doc).

## Installing

### Windows (TODO)

### Linux (TODO)

The program can be found on the repositories of the following distros:

#### Arch Linux

TODO: Make AUR repo

## Building

### Cloning

```shell
git clone --recurse-submodules 'https://github.com/HarisGusic/schim'
```

### Build dependencies (TODO check versions)

* [Qt5](https://www.qt.io/)
* [CMake](https://cmake.org/) >= 3.10
* Make >= TODO

### Needed to build manpages and help

* [Python](https://python.org) >= 3.7
* [Sphinx](https://www.sphinx-doc.org/en/master/usage/installation.html) >= 3.0.0 for help
* qt5-assistant

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

If you wish to install Schim into a custom directory, run the following command:

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

## Contributing
TODO
