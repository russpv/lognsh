# Lognsh

A minimal interactive Unix shell implemented in C. Designed for login use, not scripting. No job control, no weirdness, just fundament.


## ✅ Feature Summary
🔸 Tokenization, expansion, and parsing with explicit internal structure  
🔸 Pipes (`|`) and redirections (`>`, `<`, `>>`)  
🔸 Built-in commands (`cd`, `exit`, `export`, `unset`, `pwd`, `echo`, `env`)  
🔸 Signal handling (`CTRL+C`, heredocs, process kill safety)  
🔸 Fully memory-managed with (soon) internal garbage collection  
❌ No job control (`fg`, `&`, `CTRL+Z`, backgrounding)  
❌ No scripting / `if`, `for`, or functions


## 🚀 Quick Start

```bash
make
./lognsh
```

Ctrl + D to exit. Try:
```
ls | grep .c > out.txt
cat < out.txt
```

---

## 🏗️ Architecture

lex/ – Tokenization and lexical analysis  
token/ – Token groups, quoting, and expansion  
parse/ – Building command trees, handling redirections and heredocs  
execute/ – Process execution and pipe management  
builtins/ – Internal shell commands  
state/, mem/ – Memory tracking and error state  
data_structures/ – Internal lists, stacks, and hashmaps  
globals/ – Shell-wide constants and signal flag

## Design Philosophy

- **Explicit control over state.** No global parser state. Memory is allocated and freed with discipline.
- **Modular structure.** Each subsystem lives in its own module (e.g. lexer, parser, command/executor, memory, state).
- **Good enough is enough.** The shell rejects Bash's more obscure or magical behaviors (e.g. quote-newline continuation).
- **Readable C.** Avoided cleverness. Emphasized separation of concerns and traceable behavior.

---

## 🧭 Why I Use This as My Login Shell

I didn’t write this to replace Bash. I wrote it to understand what a shell really is and to have something I could trust and debug fully. `lognsh` doesn't support scripting or job control. It doesn’t try to guess what you meant. But it’s solid, minimal, and... mine!


Written a disciplined, modular shell with ~150 thoughtfully named source files. Investigated corner cases like locked-out directories and $PWD fallback. Cleanly separated token lexing from token expansion via meta token trees. Avoided global parser state. Designed memory ownership and iteration carefully.

with a focus on correctness, modularity, and explicit design tradeoffs.


- Tokenization and lexical parsing
- Argument and variable expansion
- Pipes and redirections
- Built-in commands (e.g. `cd`, `export`, `exit`)
- Signal handling
- Environment management
- Heredoc support