update list: 

Ajout dossier de pipex

**Builtins:**
- echo
- pwd
- env
- cd
- unset
- export
- exit

**Dispatch_cmd**
将我们的cmd 分成 builtin/简单cmd 和 pipe 然后execute

***************待解决的问题***********
1. echo der > out vs  echo "$USER" > out
3. echo 'der' < defg (un fichier n'existe pas)
4. echo 'der' > deaf(un fichier n'existe pas)
5. heredoc + ctrl c
6. ctrl d
7. 

**tout ok mais pb de free et norm**
cat main.c | cat ok
ls | grep.c ok 
