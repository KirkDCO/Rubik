#include "ISearchableNode.h"

bool NodeSort(ISearchableNode *left, ISearchableNode *right)
{
    //std::cout << "NodeSort: ";
    //std::cout << left->F() << " - " << right->F() << " - " << (left->F() < right->F()) << std::endl;
    return left->F() > right->F();
};
