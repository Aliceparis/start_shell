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


新问题： 
我们的path删了 env还能用 但是bash里面是不能用的 我们是否让env用不了如果我们把path删了。其他的话我更新了一下environ cat ls 当path删了 现在就不能用了 跟bash是一样的。 
在main里加了if (!shell_program.ast-＞left) 
Dispatch 就用不了了
比如我们写 echo ishs＞out 这个out的fichier就没有创造
