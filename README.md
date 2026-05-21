# tree-sitter-jnoweb

[tree-sitter](https://tree-sitter.github.io) grammar for **jnoweb** (Julia NoWeb) files — a lightweight literate programming format for Julia documents.

Inspired by [tree-sitter-rnoweb](https://github.com/tree-sitter-grammars/tree-sitter-rnoweb) and [knitr](https://yihui.org/knitr/).

## File type

`.jnw` files contain LaTeX prose interleaved with Julia code chunks:

```latex
\documentclass{article}
\begin{document}

<<chunk-label, echo=true>>=
x = randn(100)
println(mean(x))
@

The mean is \Sexpr{round(mean(x), digits=3)}.

\end{document}
```

## Neovim setup

### 1. Symlink query files

```bash
mkdir -p ~/.config/nvim/queries/jnoweb/
ln -sf ~/path/to/tree-sitter-jnoweb/queries/highlights.scm ~/.config/nvim/queries/jnoweb/highlights.scm
ln -sf ~/path/to/tree-sitter-jnoweb/queries/injections.scm ~/.config/nvim/queries/jnoweb/injections.scm
```

Replace `~/path/to/tree-sitter-jnoweb` with the actual path to this repo.

### 2. Compile the parser

```bash
mkdir -p ~/.local/share/nvim/site/parser/
gcc -O2 -shared \
  -I src \
  src/parser.c src/scanner.c \
  -o ~/.local/share/nvim/site/parser/jnoweb.so
```

### 3. Register the filetype

Tell Neovim to treat `.jnw` files as `jnoweb`. This is required for ALL `jnoweb` features — highlighting, formatting, injections, etc. Without it, nothing below will work.

```lua
vim.filetype.add({
  extension = { jnw = "jnoweb" },
})
```

### 4. Register the parser

Point tree-sitter to the compiled grammar:

```lua
vim.treesitter.language.add("jnoweb", {
  path = vim.fn.expand("~/.local/share/nvim/site/parser/jnoweb.so"),
})
```

### 5. Verify

Open a `.jnw` file and run `:InspectTree`. You should see proper syntax nodes with no red `ERROR` tags.

## Installation from GitHub release

Download `tree-sitter-jnoweb.wasm` or `tree-sitter-jnoweb.tar.gz` from the [latest release](https://github.com/urtzienriquez/tree-sitter-jnoweb/releases).

## Formatting

This repo ships `formatters/jnoweb-fmt`, a standalone formatter for `.jnw` files. It formats Julia code chunks with [JuliaFormatter](https://github.com/domluna/JuliaFormatter.jl) and LaTeX prose with [latexindent](https://ctan.org/pkg/latexindent).

### Installation

```bash
cp formatters/jnoweb-fmt ~/.local/bin/
chmod +x ~/.local/bin/jnoweb-fmt
```

Dependencies (must be on `$PATH` or overridden via env vars):

| Tool | Env var override | Install |
|------|------------------|---------|
| `latexindent` | `JNOWEB_LATEXINDENT` | `sudo apt install latexindent` or via Mason (`MasonInstall latexindent`) |
| `julia` | `JNOWEB_JULIA` | [julialang.org](https://julialang.org) |
| `JuliaFormatter` (Julia package) | `JNOWEB_JULIA_PROJECT` (Julia project; default `@lang_serv`) | `using Pkg; Pkg.add("JuliaFormatter")` within the project |

### Usage

```bash
jnoweb-fmt file.jnw          # format in-place
jnoweb-fmt < input.jnw > output.jnw   # stdin/stdout
```

### Neovim (conform.nvim)

Requires the filetype registration from [step 3](#3-register-the-filetype) above.
Without `vim.filetype.add`, conform won't recognize `.jnw` files.

```lua
require("conform").setup({
  formatters = {
    jnoweb_fmt = {
      command = "jnoweb-fmt",
      stdin = true,
    },
  },
  formatters_by_ft = {
    jnoweb = { "jnoweb_fmt" },
  },
})
```

## Queries

- `highlights.scm` — syntax highlighting for chunk delimiters, options, and `\Sexpr{}`
- `injections.scm` — injects Julia into code chunks and `\Sexpr{}`, LaTeX into prose

## Development

```bash
tree-sitter generate    # regenerate parser.c from grammar.js
tree-sitter test        # run corpus tests
tree-sitter build --wasm  # build WASM binary
```

To recompile the `.so` for local testing after changing the grammar:

```bash
tree-sitter generate
gcc -O2 -shared -I src src/parser.c src/scanner.c -o ~/.local/share/nvim/site/parser/jnoweb.so
```

## License

GPL-3.0-or-later
