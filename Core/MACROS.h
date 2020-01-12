#ifndef MACROS_H
#define MACROS_H

#define __FILENAME__ strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__

#define indexof(type) std::type_index(typeid(type))
#define nameof(type) std::type_index(typeid(type)).name()
#define hashof(type) std::type_index(typeid(type)).hash_code()

#define vfind(vector, item) std::find(vector.begin(), vector.end(), item)

#endif