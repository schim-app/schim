
# <img align="left" width="25" src="../res/img/icon.svg" alt="Icon"/> Schim documentation

The documentation is built using [Sphinx](https://www.sphinx-doc.org/en/master/)
and hosted on the great [ReadTheDocs](https://schim.rtfd.io). Also, the HTML
version uses the beautiful theme of the same name.

## Building

The documentation can be generated in various formats:

* HTML
* Man pages
* PDF via LaTeX
* Qt Help

The above formats are the ones that are used by this project, although Sphinx
supports many more.

The following dependencies are required:
(TODO check the exact versions)

### Dependencies

#### Python dependencies

* [Python](https://python.org) >= 3.7
* [Sphinx](https://www.sphinx-doc.org/en/master/usage/installation.html) >= 3.0.0
* [ReadTheDocs theme](https://pypi.org/project/sphinx-rtd-theme/)
* [Breathe](https://breathe.readthedocs.io/en/latest/index.html) >= 4.26.1
* [python-docutils](https://pypi.org/project/docutils/) for man pages

The python-based dependencies can be installed using:

```shell
pip install sphinx breathe docutils sphinx-rtd-theme
```

#### Other dependencies (TODO add more)
* qt5-tools (for Qt Help)

---

Once the dependencies are satisfied, the documentation can be built by the
following command:

```shell
make <format>
```

This will build the documentation in the directory `_build/<format>`. The
`<format>` can be any of the following: `html`, `man`, `qthelp`. You can also
specify multiple formats separated by spaces.
