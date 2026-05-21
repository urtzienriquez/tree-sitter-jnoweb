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

## Installation

### npm

```bash
npm install tree-sitter-jnoweb
```

### Neovim with nvim-treesitter

If the parser is [registered with nvim-treesitter](https://github.com/nvim-treesitter/nvim-treesitter), install with:

```
:TSInstall jnoweb
```

Otherwise, add this to your config (edit the URL to match the published version):

```lua
vim.api.nvim_create_autocmd('User', {
  pattern = 'TSUpdate',
  callback = function()
    require('nvim-treesitter.parsers').jnoweb = {
      install_info = {
        url = 'https://github.com/urtzienriquez/tree-sitter-jnoweb',
        revision = 'v0.2.0',
      },
    }
  end,
})

vim.treesitter.language.register('jnoweb', 'jnoweb')
```

Then run `:TSInstall jnoweb`.

### Other editors

- **Helix**: add this to your `languages.toml`:
  ```toml
  [[language]]
  name = "jnoweb"
  scope = "source.jnoweb"
  file-types = ["jnw"]
  ```
- **VS Code**: see the [VS Code extension marketplace](#) (coming soon)

## Queries

- `highlights.scm` — syntax highlighting for chunk delimiters, options, and `\Sexpr{}`
- `injections.scm` — injects Julia into code chunks and `\Sexpr{}`, LaTeX into prose

## Development

```bash
npm install       # install dependencies
tree-sitter test  # run corpus tests
tree-sitter build --wasm  # build WASM binary
```

## License

GPL-3.0-or-later
