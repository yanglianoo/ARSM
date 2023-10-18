#include "LIJU.h"


int main()
{
    LIJU test;
    test.init(DEST_ADDR,UDP_PORT);
    test.UDPDirectInit(0x01);
    while(1)
    {

        test.UDP_GetData();
        test.print_RAWData();
        test.calc_resault();
        test.print_resault();
        sleep(1);
    }

    return 0;
}