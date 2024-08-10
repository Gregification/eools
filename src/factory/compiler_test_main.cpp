/**
* should run without error and the messages printed make sense.
* 
* troubleshooting
*   1. all class id's are the same : counter does not work, simple problem.
*       if your lazy you can replace with `__COUNTER__` but this can lead
*       to lots of wasted memory.
*   2. error about vector out of bounds : your screwed (idk y it does that)
*   3. issue not described here : womp womp. would apperishate if you solved it.
*/

#include <vector>
#include <functional>
#include <iostream>

#include "Factory.hpp"

#define FACTORYCONTROLLED(CLASSNAME, BASE) CLASSNAME : public Factory::FactoryInstable<BASE, CLASSNAME>

#define BASE(C) struct C : public Messageable {\
    virtual void packMessage(Message&, MsgDiffType) override\
    {\
        std::cout << #C << " [pack] called" << std::endl;\
    }\
    virtual void unpackMessage(Message&, MsgDiffType) override\
    {\
        std::cout << #C << " [unpack] called" << std::endl;\
    }\
};

#define CAP(C, B) struct FACTORYCONTROLLED(C, B), public B {\
    virtual void packMessage(Message&, MsgDiffType) override\
    {\
        std::cout << #C << " [pack] called" << std::endl;\
    }\
    virtual void unpackMessage(Message&, MsgDiffType) override\
    {\
        std::cout << #C << " [unpack] called" << std::endl;\
    }\
};

#define PRINT(C) std::cout << #C << " cid:" << C::factory.class_id << std::endl;

BASE(BaseA);
CAP(DerivedAA, BaseA);
CAP(DerivedAB, BaseA);
CAP(DerivedAC, BaseA);
CAP(DerivedAD, BaseA);
CAP(DerivedAE, BaseA);
CAP(DerivedAF, BaseA);

BASE(BaseB);
CAP(DerivedBA, BaseB);
CAP(DerivedBB, BaseB);
CAP(DerivedBC, BaseB);
CAP(DerivedBD, BaseB);
CAP(DerivedBE, BaseB);
CAP(DerivedBF, BaseB);

BASE(BaseC);
CAP(DerivedCA, BaseC);
CAP(DerivedCB, BaseC);
CAP(DerivedCC, BaseC);
CAP(DerivedCD, BaseC);
CAP(DerivedCE, BaseC);
CAP(DerivedCF, BaseC);

int main() {

    /*PRINT(DerivedAA);
    PRINT(DerivedAB);
    PRINT(DerivedAC);
    PRINT(DerivedAE);
    PRINT(DerivedAF);

    PRINT(DerivedBA);
    PRINT(DerivedBB);
    PRINT(DerivedBC);
    PRINT(DerivedBD);
    PRINT(DerivedBE);
    PRINT(DerivedBF);

    PRINT(DerivedCA);
    PRINT(DerivedCB);
    PRINT(DerivedCC);
    PRINT(DerivedCD);
    PRINT(DerivedCE);
    PRINT(DerivedCF);*/

    std::cout << std::endl;

    int x = 0;
    
    std::cout << "should call DerivedAA's version of pack" << std::endl;
    Factory::InstFactory<BaseA>::PackAs(DerivedAA::GetClassID(), x, x, 0);

    std::cout << std::endl;

    std::cout << "should call DerivedAD's version of unpack" << std::endl;
    Factory::InstFactory<BaseA>::UnpackAs(Factory::FactoryInstable<BaseA, DerivedAD>::GetClassID(), x, x, 0);

    std::cout << std::endl;

    std::cout << "should call DerivedAC's version of pack" << std::endl;
    std::shared_ptr<BaseA> c = Factory::InstFactory<BaseA>::GetInstance(DerivedAC::GetClassID());
    c->packMessage(x, x);

    return 0;
}
