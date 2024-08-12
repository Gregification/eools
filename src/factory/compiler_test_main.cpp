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

struct Base : public Factory::FactoryInstable<Base, Base>, public Messageable {
    void packMessage(Message&, MsgDiffType) override {
        std::cout << "base [pack]" << std::endl;
    }
    void unpackMessage(Message&, MsgDiffType) override {
        std::cout << "base [unpack]" << std::endl;
    }
};
struct ClassA : public Factory::FactoryInstable<Base, ClassA>, public Base {
    void packMessage(Message&, MsgDiffType) override {
        std::cout << "A [pack]" << std::endl;
    }
    void unpackMessage(Message&, MsgDiffType) override {
        std::cout << "A [unpack]" << std::endl;
    }
};
struct ClassB : public Factory::FactoryInstable<Base, ClassB>, public ClassA {
    void packMessage(Message&, MsgDiffType) override {
        std::cout << "B [pack]" << std::endl;
    }
    void unpackMessage(Message&, MsgDiffType) override {
        std::cout << "B [unpack]" << std::endl;
    }
};

int main() {
    std::vector<std::shared_ptr<Base>> arr = {
        std::make_shared<Base>(),
        std::make_shared<ClassA>(),
        std::make_shared<ClassB>()
    };

    //geting the number for specific classes
    // prints out emojies... as long as they match
    std::cout << "Base's   num: " << Base::factory.GetClassId<Base>() << std::endl;
    std::cout << "ClassA's num: " << Base::factory.GetClassId<ClassA>() << std::endl;
    std::cout << "ClassB's num: " << Base::factory.GetClassId<ClassB>() << std::endl;

    for (std::shared_ptr<Base> v : arr) {
        Class_Id cid = Base::factory.GetClassId(v.get());

        std::cout << cid << std::endl;
        
        Message m;
        if (cid != BAD_CLASS_ID)
            Base::factory.PackAs(cid, 0, m, v.get());

    }

    return 0;
}