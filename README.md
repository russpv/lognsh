# Lognsh

A minimal interactive Unix shell implemented in C from scratch on top of GNU `readline`.


## Feature Summary
🔸 Tokenization, expansion, and parsing with explicit internal structure  
🔸 Pipes (`|`), redirections (`>`, `<`, `>>`), wildcards, logicals (`&&`, `||`), subs (`()`)  
🔸 Built-in commands (`cd`, `exit`, `export`, `unset`, `pwd`, `echo`, `env`, `set`)  
🔸 Signal handling (`CTRL+C`, heredocs, process kill safety)  
🔸 Fully memory-managed with internal memory tracking  
🔸 Toggle-able log, debug, verbose debug modes  
❌ No job control (`fg`, `&`, `CTRL+Z`, backgrounding)  
❌ No scripting / `if`, `for`, or functions

## Performance 

Snapshot: `lognsh` is optimized for fast startup and execution. It consistently matches bash in cold and warm runs:

```
$ time bash -c "echo hello"
hello
bash -c "echo hello"  0.00s user 0.00s system 70% cpu 0.004 total

$ time ./lognsh "echo hello"
hello
./lognsh "echo hello"  0.00s user 0.00s system 72% cpu 0.003 total
```

- Heap allocations are freed in bulk between commands to reduce fragmentation and malloc churn.
- Tokenization and expansion are stable even under large, deeply quoted command lines.
- Shell startup completes in <1ms on modern systems, with <2MB memory footprint.
- Pipes and redirections are processed with minimal setup overhead, deferring to OS scheduling.

## Quick Start

```bash
# git clone https://github.com/russpv/lognsh.git <directory>
make      # make tst
./lognsh  # ./tester   <-- inspect end-to-end tests (has also been fuzzed)
```

Ctrl + D to exit. Try:
```
ls | grep *lib > out.txt && (echo "Done, $USER.")
cat < out.txt
set debug
```

---

## Architecture

`lex/` – Tokenization and lexical analysis  
`token/` – Token ADT: groups, quoting, and expansion methods  
`parse/` – Building command trees, handling redirections and heredocs  
`command/` - Building commands and orchestration  
`execute/` – Process and pipe execution  
`builtins/` – Internal shell commands  
`state/, mem/` – Memory tracking and error state  
`data_structures/` – Internal lists, stacks, and hashmaps  
`globals/` – Shell-wide constants and signal and debug flags

## Design Philosophy

- **Explicit control over state.** No global parser state. Memory is allocated and freed with discipline.
- **Modular structure.** Each subsystem lives in its own First-class ADT module.
- **DOTADIW.** The shell rejects Bash's more obscure or magical behaviors (e.g. quote-newline continuation).
- **Readable C.** Emphasis on separation of concerns and traceable behavior.

## License

MIT License – see [LICENSE](LICENSE.md) for details.

## Contributors

- archshire - built-in commands and env scaffolding

---

> Programs are too bloated today. Here's a simple shell with just enough to get things done. You're welcome.

