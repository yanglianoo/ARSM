#pragma once

#include <string>
#include <typeinfo>
#include <unordered_map>
#include <iostream>

class AbstractClassFactoryBase {
public:
    AbstractClassFactoryBase(const std::string &className, const std::string &baseClassName) ;
    virtual ~AbstractClassFactoryBase()
    {

    }

    const std::string GetClassName() const;
    const std::string GetBaseClassName() const;

protected:
    std::string m_baseClassName;
    std::string m_className;
};


template <typename Base> class AbstractClassFactory : public AbstractClassFactoryBase {
public:
    AbstractClassFactory(const std::string &className, const std::string &baseClassName)
        : AbstractClassFactoryBase(className, baseClassName)
    {}

    virtual Base *CreateObj() const = 0;

private:
    AbstractClassFactory();
};

template <typename ClassObject, typename Base> class ClassFactory : public AbstractClassFactory<Base> {
public:
    ClassFactory(const std::string &className, const std::string &baseClassName)
        : AbstractClassFactory<Base>(className, baseClassName)
    {}

    Base *CreateObj() const
    {
        return new ClassObject;
    }
};

extern void RegisterClassFactory(const std::string &className, AbstractClassFactoryBase *classFactoryObj);



template <typename Derived, typename Base>
void RegisterClass(const std::string &className, const std::string &baseClassName)
{
    auto newClassFactoryObj =
        new ClassFactory<Derived, Base>(className, baseClassName);
    RegisterClassFactory(className, newClassFactoryObj);
}

#define CLASS_LOADER_REGISTER_CLASS_INTERNAL(Derived, Base, UniqueID) \
    namespace {                                                       \
    struct ProxyType##UniqueID {                                      \
        ProxyType##UniqueID()                                         \
        {                                                             \
            RegisterClass<Derived, Base>(#Derived, #Base);            \
        }                                                             \
    };                                                                \
    static ProxyType##UniqueID g_register_class_##UniqueID;           \
    }


#define CLASS_LOADER_REGISTER_CLASS_INTERNAL_1(Derived, Base, UniqueID) \
    CLASS_LOADER_REGISTER_CLASS_INTERNAL(Derived, Base, UniqueID)

// register class macro
#define CLASS_LOADER_REGISTER_CLASS(Derived, Base) \
  CLASS_LOADER_REGISTER_CLASS_INTERNAL_1(Derived, Base, __COUNTER__)