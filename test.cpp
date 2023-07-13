#include <map>
#include <iostream>
#include <utility/ClassFactory.hpp>

// 全局 unordered_map，用于存储类工厂对象
std::unordered_map<std::string, AbstractClassFactoryBase*> classFactories;

// 注册类工厂的函数
void RegisterClassFactory(const std::string& className, AbstractClassFactoryBase* factory) {
    classFactories[className] = factory;
}

// 创建对象的函数
template <typename Base>
Base* CreateObject(const std::string& className) {
    auto it = classFactories.find(className);
    if (it != classFactories.end()) {
        AbstractClassFactory<Base>* factory = static_cast<AbstractClassFactory<Base>*>(it->second);
        if (factory) {
            return factory->CreateObj();
        }
    }
    return nullptr;
}

class IMU {
public:
    virtual ~IMU() {}
    virtual void Initialize() = 0;
    virtual void ReadData() = 0;
    // 其他接口和成员函数
};

class LPMS : public IMU {
public:
    void Initialize() override {
        std::cout<<"init" <<std::endl;
    }

    void ReadData() override {
        std::cout<<"get data" <<std::endl;
    }
};
CLASS_LOADER_REGISTER_CLASS(LPMS,IMU)

class CAMERA {
public:
    virtual ~CAMERA() {}
    virtual void Initialize() = 0;
    virtual void ReadData() = 0;
    // 其他接口和成员函数
};

class D435 : public CAMERA {
public:
    void Initialize() override {
        std::cout<<"init" <<std::endl;
    }

    void ReadData() override {
        std::cout<<"get data" <<std::endl;
    }
};
CLASS_LOADER_REGISTER_CLASS(D435,CAMERA)
int main()
{
    IMU * newimu = CreateObject<IMU>("LPMS");
    newimu->Initialize();
    newimu->ReadData();

    CAMERA* d435 = CreateObject<CAMERA>("D435");
    d435->Initialize();
    d435->ReadData();
    return 0;
}

