# MO
Mobile Objects for Music (MOM), Mobile Object for Drawing (MOD), and ...

## Installation

First: clone this repo:
```
git clone git@github.com:batchku/MO.git
```

Then: This repo relies on a [aLib-pd](https://github.com/batchku/aLib-pd) as a submodule. You can either clone that repo separately, or use this command within the MO repository to pull the submodule:

```
git submodule update --init --recursive
```
### nameChange.sh

To test for resulting name change use "test":


```./nameChanger.sh test searchTerm wordToReplaceWith```

To actually change use "change":


```./nameChanger.sh change searchTerm wordToReplaceWith```
