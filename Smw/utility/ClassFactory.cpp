#include "ClassFactory.hpp"


AbstractClassFactoryBase::AbstractClassFactoryBase(const std::string &className, const std::string &baseClassName)
{
    m_baseClassName = baseClassName;
    m_className = className;
}

const std::string AbstractClassFactoryBase::GetBaseClassName() const
{
    return m_baseClassName;
}

const std::string AbstractClassFactoryBase::GetClassName() const
{
    return m_className;
}