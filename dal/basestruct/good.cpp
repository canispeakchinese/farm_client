#include "good.h"

Good createGood(GoodType type, int kind, int num)
{
    if(type == Fruit) {
        Good aim(goodDefin[Seed][kind]);
        aim.type = Fruit;
        aim.num = num;
        return aim;
    } else {
        Good aim(goodDefin[type][kind]);
        aim.num = num;
        return aim;
    }
}
