## <p id="commitizen-header"> [Commitizen](https://commitizen-tools.github.io/commitizen/)


### Installation:

Recommended:

```pip install -U commitizen```

```pip install pre-commit```

```pre-commit install --hook-type commit-msg --hook-type pre-push```

```pre-commit autoupdate```

### Usage:

```cz commit```

or without the Commitizen UI: 

```git commit```

## Branching

Consistent branches are **main** and **develop**. Feature branches will be created from **develop**. \
Once a feature is implemented and doesn't break the program a merge request is to be opened to **develop**.\
**Develop** will be periodically merged into **main**.

If you want to update your **feature branch** to the newest version of **develop**, create a copy of **develop** and merge your \
**feature branch** into it. The **develop** copy turns into the new **feature branch**. This is to keep the git log cleaner.

## Commit Syntax

The commit syntax has to respect the norm set by the [Commitizen config file](.cz.json).\
Following the [Commitizen installation](#commitizen-header) is enough to enforce this rule.