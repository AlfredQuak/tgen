tgen
====
tgen is a codegenerator for tntet, this is the first dev-version. Bugs, feature request please add to issues.  
This is not stable! Use it on your own risk!

tgen needs cxxtools, tntdb, boost_system, boost_filesystem


<code>tgen new foobar</code>        create tntnet foobar project structur

<code>tgen model foobar</code>      create model foobar with insert update delete from database table

<code>tgen view foobar</code>       create foobar html and ecpp file

<code>tgen controller foobar</code> create foobar controller foobar 

<code>tgen --parse-html</code>      create ecpp files for all html files  

<code>tgen --create-make</code>     create makefile or if exists, update include makefile
  
  
Path structure

+ application 
      - component      
      - model 
      - model/generated 
      - controllers
      - view
      - view/html
      - view/ecpp
+ config
      - database.ini
      - url_mapping.h
+ helper
+ lib
+ log
+ plugin

Todo:
- create scaffold function
- create makefile includes for ecpp
- create makefiles for netbeans
- create release mysql create table files
- ...
