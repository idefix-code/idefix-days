# Data visualization with Python (yt and nonos): pre-requisites

These tutorials require Python 3.8 or newer.

Both tutorials include requirement files. It is strongly advised to create virtual
environments before installing anything specifically for these tutorials. To get
started, navigate to a tutorial's subdirectory

```shell
$ cd idefix-days/tutorials/viz/<dir>
```
(where `<dir>` is a placeholder)

and create a virtual environment...

... with conda (`<name>` is a placeholder)
```shell
$ conda create --name=<name> python=3.10
$ conda activate <name>
```

... or with `venv`
```shell
$ python -m venv .venv
$ source .venv/bin/activate
```

Then proceed to install requirements
```shell
$ python -m pip install -r requirements.txt
```
