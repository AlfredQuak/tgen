#ifndef CVIEW_H
#define	CVIEW_H

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <sys/stat.h>

class CView {
public:
    CView();
    virtual ~CView();
    
    void createView(string view);
    void parseHtmlView();
private:

};

#endif	/* CVIEW_H */

