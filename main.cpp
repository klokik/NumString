#include <iostream>

#include "NumString.h"

void autotest()
{
    NumString<int,0,2048> numstr;

    numstr.push_back(0xD);
    numstr.push_back(0xE);
    numstr.push_back(0xA);
    numstr.push_back(0xD);
    numstr.push_back(0xB);
    numstr.push_back(0xE);
    numstr.push_back(0xE);
    numstr.push_back(0xF);

    NumString<int,0,64> expected;

    expected.push_back(45);
    expected.push_back(43);
    expected.push_back(61);
    expected.push_back(58);
    expected.push_back(62);
    expected.push_back(3);

    auto lol = numstr.toBase<64>(16);
    if(lol.size() != expected.size())
    {
        std::cout<<"autotest - size fail\n";
        return;
    }

    for(int q=0;q<lol.size();q++)
        if(lol[q]!=expected[q])
        {
            std::cout<<"autotest - content fail\n";
            for(auto item:lol) std::cout<<item<<"'";
            std::cout<<std::endl;
            return;
        }
    std::cout<<"autotest - passed\n";
}

void handle_user_preferences()
{
    std::cout<<"Hello, dummy user!\nI heard u like numbers?\n1 or 2?\n> ";
    int pref = 3;
    std::cin>>pref;

    if(pref == 3)
    {
        autotest();
        return;
    }

    if(pref == 1)
    {
        std::cout<<"You've chosen loading from file"<<std::endl;
        std::cout<<"No way dummy user, there is no point in files for numbers!"<<std::endl;
        std::cout<<"Running autotest:\n";
        autotest();
    }
    else
    {
        std::cout<<"You've chosen manual input:\n";
        std::cout<<"\tSpecify base: ";
        uint base = 0;
        std::cin>>base;
        std::cout<<"Input number one by one digit (Little endian), -1 to finish:\n";
        int d = 0;
        NumString<int,0,2048> numstr;
        while(1)
        {
            std::cin>>d;
            if(d<0)
                break;
            numstr.push_back(d);
        }
        std::cout<<"\nyour number is:";
        for(auto item:numstr) std::cout<<item<<"'";

        auto base2 = numstr.toBase<2>(base);
        auto base3 = numstr.toBase<3>(base);
        auto base8 = numstr.toBase<8>(base);
        auto base10 = numstr.toBase<10>(base);
        auto base16 = numstr.toBase<16>(base);
        auto base27 = numstr.toBase<27>(base);
        auto base135 = numstr.toBase<135>(base);

        std::cout<<"\nyour number in base 2: ";
        for(auto item:base2) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 3: ";
        for(auto item:base3) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 8: ";
        for(auto item:base8) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 10: ";
        for(auto item:base10) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 16: ";
        for(auto item:base16) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 27: ";
        for(auto item:base27) std::cout<<item<<"'";

        std::cout<<"\nyour number in base 135: ";
        for(auto item:base135) std::cout<<item<<"'";

        std::cout<<std::endl;
    }
}

int main(int argc, char const *argv[])
{
    handle_user_preferences();

	return 0;
}
