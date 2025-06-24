# 💻 Minishell

Minishell is a simplified Unix shell implemented in C as part of the 42 Network curriculum. The goal is to understand process management, parsing, file descriptors, and the behavior of a real shell — by rebuilding the essential features of **bash** or **zsh**.

---

## 🧠 Project Objectives

- Recreate a working shell with basic built-in commands
- Handle pipes, redirections, environment variables, and signals
- Understand how processes and file descriptors work at the OS level

---

## ⚙️ Features

✅ Command execution (with `PATH` resolution)  
✅ Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`  
✅ Pipes and multiple chained commands (`|`)  
✅ Input/output redirection (`>`, `>>`, `<`)  
✅ `heredoc` (`<<`) support  
✅ Signal handling (`Ctrl+C`, `Ctrl+\`, `Ctrl+D`)  
✅ Quoting: single (`'`) and double (`"`) quotes  
✅ Environment variable expansion (`$VAR`)  
✅ Error handling and return codes



