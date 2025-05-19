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
1. token中的单双引号是否保留
3. free ast有问题
6. $?值返回的设置在expansion那一部分还没有加进去



**ft_fprintf OK**


**Les commades qui marche**:
- echo
- exit
- pwd
- env
- ls
