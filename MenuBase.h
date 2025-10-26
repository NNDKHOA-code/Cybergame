#ifndef MENUBASE_H
#define MENUBASE_H

class MenuBase {
public:
    virtual ~MenuBase(){}
    virtual void run() = 0;
};

#endif