# team/coding conventions

#### Installation:

Install commitizen globally and pre-commit into your current repositories `.git` folder:

```bash
$ pip install -U commitizen

$ pip install pre-commit
```

Now install the hook for commit message checking:

```bash
$ pre-commit install --hook-type commit-msg

$ pre-commit autoupdate
```

#### Usage:

`$ cz commit`

or without the Commitizen UI:

`$ git commit`

### Branching & Merge Requests

Consistent branches are **main** and **develop**. Feature branches will be created from **develop**.\
Once a feature is implemented and doesn't break the program a merge request is to be opened to **develop**.\
**Develop** will be periodically merged into **main**.

If you want to update your **feature branch** to the newest version of **develop**, create a copy of **develop** and merge your\
**feature branch** into it. The **develop** copy turns into the new **feature branch**. This is to keep the git log cleaner.

### Bugs and feature requests

When submitting a bug or feature request use the GitHub [Issues feature](https://github.com/Epitech-R-Type/R-Type/issues). Make sure to add labels fitting your topic\
and as a developer to create a branch using the generated branch name tied to this issue.

### Coding Style

We use clang-format to lint our code, please follow the configuration set by `.clang-format`. Please fix the issues pointed out by the automatic comments added to changed files during a MR/PR.

### Commit Syntax

The commit syntax has to respect the norm set by the [Commitizen config file](https://github.com/Epitech-R-Type/R-Type/blob/main/.cz.json).\
Following the [Commitizen installation](team-coding-conventions.md#installation) is enough to enforce this rule.
