#ifndef CCONTROLLER_H
#define	CCONTROLLER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/algorithm/string.hpp>

class CController {
public:
    CController();
    CController(const CController& orig);
    virtual ~CController();
    
    void createController(string controller);
private:

};

#endif	/* CCONTROLLER_H */

