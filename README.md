tgen
====
tgen is a codegenerator for tntet, this is the first dev-version. Bugs, feature request please add to issues.
This is not stable! Use it on your own risk!

tgen needs cxxtools, tntdb, boost_system, boost_filesystem


tgen new foobar           create tntnet project structur
tgen model foobar         create model with insert update delete from database table
tgen view foobar          create html file and ecpp file
tgen controller foobar    create controller foobar
tgen --parse-html         create ecpp files for all html files


+ application - model
+ application - model - generated
+ application - controllers
+ application - view
+ application - view - html
+ application - view - ecpp
+ config
      - database.ini
      - url_mapping.h
+ helper
+ lib
+ log
+ plugin

Todo:
- create makefile includes for ecpp
- create makefiles for netbeans
- create release mysql create table files
- ...
