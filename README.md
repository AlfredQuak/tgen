tgen
====
tgen is a codegenerator for tntet, this is the first dev-version. Bugs, feature request please add to issues.  
This is not stable! Use it on your own risk!



Compile and install tgen
========================  
tgen needs cxxtools, tntdb, boost_system, boost_filesystem.  
simply make tgen and copy tgen into /usr/bin.   


cd tgen  
make  
cp tgen /usr/bin  
sudo chmod +x /usr/bin/tgen  



New tgen Project
================

Create a new tntnet standalone project:  

tgen new testproj  
cd testproj  
make  
./testproj  

Open your webbrowser localhost:8000 and you see your index.html

tgen functions
==============

<code>tgen new foobar</code>        create tntnet foobar project structur

<code>tgen model foobar</code>      create model foobar with insert update delete from database table

<code>tgen view foobar</code>       create foobar html and ecpp file

<code>tgen controller foobar</code> create foobar controller foobar 

<code>tgen --parse-html</code>      create ecpp files for all html files  

<code>tgen --create-make</code>     create makefile or if exists, update include makefile
  


Path structure
==============

+ application 
      - component      
      - model 
      - model/generated 
      - controllers
      - view
      - view/html
      - view/html/index.html
      - view/ecpp
      - view/ecpp/index.ecpp
+ config
      - settings.ini
      - url_mapping.h
+ helper
+ lib
+ log
+ plugin

Todo
====
- create scaffold function
- create makefiles for netbeans
- create release mysql create table files
- ...
