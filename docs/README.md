
# <img align="left" width="25" src="../res/img/icon.svg" alt="Icon"/> Schim documentation

The documentation is built using [Sphinx](https://www.sphinx-doc.org/en/master/)
and hosted on the great [ReadTheDocs](https://schim.rtfd.io). Also, the HTML
version uses the beautiful theme of the same name.

## Building

The documentation can be generated in various formats:

* HTML
* Man pages
* Qt Help
* PDF via LaTeX

The above formats are the ones that are used in this project, although Sphinx
supports many more.

The documentation can be built by the following command:

```shell
make <format>
```

This will build the documentation in the directory `_build/<format>`. The
`<format>` can be any of the following: `html`, `man`, `qthelp`, `pdf`. You can also
specify multiple of these formats separated by spaces.

### Dependencies

#### Python dependencies

* [Python](https://python.org) >= 3.7
* [Sphinx](https://www.sphinx-doc.org/en/master/usage/installation.html) >= 3.0.0
* [ReadTheDocs theme](https://pypi.org/project/sphinx-rtd-theme/)
* [Breathe](https://breathe.readthedocs.io/en/latest/index.html) >= 4.26.1

The python-based dependencies can be installed using:

```shell
pip install sphinx sphinx-rtd-theme breathe 
```

#### Other dependencies
* Make >= TODO
* Doxygen >= TODO
* qt5-tools >= TODO (for Qt Help)

