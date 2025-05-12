// Jeremy Arias
//CS116

#ifndef BAG6_H 
#define BAG6_H
#include <cstdlib>     
#include "bintree.h"   

namespace main_savitch_10
{
    template <class Item>
    class bag
    {
    public:
        // TYPEDEFS
	typedef std::size_t size_type;
        typedef Item value_type;
        // CONSTRUCTORS and DESTRUCTOR
        bag( );
        bag(const bag& source);
        ~bag( );	
        // MODIFICATION functions
        size_type erase(const Item& target);
        bool erase_one(const Item& target);
        void insert(const Item& entry);
        void operator +=(const bag& addend);
        void operator =(const bag& source);	
        // CONSTANT functions
        size_type size( ) const;
        size_type count(const Item& target) const;
    private:
        binary_tree_node<Item> *root_ptr; // Root pointer of binary search tree
        void insert_all(binary_tree_node<Item>* addroot_ptr);
    };

    
    template <class Item>
    bag<Item> operator +(const bag<Item>& b1, const bag<Item>& b2);
}

#include "bag6.template"
#endif