# Lognsh

A minimal Unix shell implemented in C, with a focus on correctness, modularity, and explicit design tradeoffs.

Lognsh is a small but faithful reimplementation of essential Unix shell behavior, written in C. It handles:

- Tokenization and lexical parsing
- Argument and variable expansion
- Pipes and redirections
- Built-in commands (e.g. `cd`, `export`, `exit`)
- Signal handling
- Environment management
- Heredoc support

Written a disciplined, modular shell with ~150 thoughtfully named source files. Investigated corner cases like locked-out directories and $PWD fallback. Cleanly separated token lexing from token expansion via meta token trees. Avoided global parser state. Designed memory ownership and iteration carefully.

### Design Philosophy

- **Explicit control over state.** No global parser state. Memory is allocated and freed with discipline.
- **Modular structure.** Each subsystem lives in its own module (e.g. lexer, parser, command/executor, memory, state).
- **Good enough is enough.** The shell rejects Bash's more obscure or magical behaviors (e.g. quote-newline continuation).
- **Readable C.** Avoided cleverness. Emphasized separation of concerns and traceable behavior.
