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


***************待解决的问题***********
1. token中，需要对quote是否闭合进行检测，如果没有闭合，error
2. token中的单双引号是否保留
3. expansion里面 $？的输出值没有添加在内，在change variable中需要添加进去
4. expansion 中，expand-command 和ft_expand_ast没有进行检测，expand部分没有问题
5. 
