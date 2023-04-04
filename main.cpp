#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include "objectFactory.hpp"
#include "containers.hpp"
#include "vectormap.hpp"




class IGameObject{
    public:
        virtual ~IGameObject(){}
        virtual void Render()=0;
        virtual void Update()=0;
        //virtual static std::unique_ptr<IGameObject> create()=0;
};


class Plane: public IGameObject{
    public:
        Plane()=default;

        Plane(int x, int y){

        }
        virtual void Render() override{
            std::cout<< " Plane render" << std::endl;
        }
        virtual void Update() override{
            std::cout<< " Plane update" << std::endl;
        }

        static std::unique_ptr<IGameObject> create(){
            return std::make_unique<Plane>();
        }

        // static IGameObject* create(){
        //     return new Plane();
        // }
        ~Plane(){
            std::cout<<" destructor Plane"<<std::endl;
        }



};

class Boat: public IGameObject{
    public:

        Boat()=default;
        Boat(int x, int y){

        }

        virtual void Render() override{
            std::cout << " Boat render"<< std::endl;
        }
        virtual void Update() override{
            std::cout << " Boat update"<< std::endl;
        }
        //static std::unique_ptr<IGameObject> create
};

enum class ObjectType{PLANE,BOAT};

std::unique_ptr<IGameObject> createObjectFactory(ObjectType _objectType){
    std::unique_ptr<IGameObject> itsObject;
    switch (_objectType)
    {
    case ObjectType::PLANE:
        itsObject=std::make_unique<Plane>(100,200);
        break;
    
    default:
        break;
    }
    return itsObject;
}

// GOAL: create objects at run time with NEW TYPES 

class MyGameObjectFactory{
    public:
        //callback function for creating an object
        /*The (*CreateObjectCallback) syntax in the typedef statement is used to define a new function pointer type called CreateObjectCallback. 
        The () at the end of the statement are necessary to indicate that this is a function pointer type.*/
        //typedef IGameObject* (*CreateObjectCallback)(); //OLD WAY
        //using createObjecCallBackReturnType = IGameObject*;
        using createObjecCallBackReturnType = std::unique_ptr<IGameObject>;
        using createObjectCallback = createObjecCallBackReturnType (*)();

        static void registerObject(const std::string& type, createObjectCallback _createObjectCallback){
            s_objects.insert({type,_createObjectCallback});
        }

        static void unregistObject(const std::string& type){
            s_objects.erase(type);
        }

        static createObjecCallBackReturnType createObjectOnTheFly(const std::string& type){
            callbackMap::iterator iterator = s_objects.find(type);
            if(iterator!=s_objects.end()){
                createObjectCallback createObject = iterator->second;
                return createObject(); // invoke function which returns IGameObject*
            }
            throw std::runtime_error("No callback function found for object type" + type);
        }


    private:
        //typedef std::map<std::string,CreateObjectCallback> callbackMap;
        using callbackMap = std::unordered_map<std::string,createObjectCallback>;
        static callbackMap s_objects;

        /*
        In general, std::map is a good choice when the order of elements is important,
         and when the number of elements is relatively small. std::unordered_map is a good choice
          when the order of elements is not important, and when the number of elements is 
          relatively large or when fast lookup times are critical. However, 
          the best choice of container depends on the specific requirements of your program,
           and may require benchmarking and profiling to determine the optimal solution.*/
};


std::unordered_map<std::string, MyGameObjectFactory::createObjectCallback> MyGameObjectFactory::s_objects; 

int main() {

    auto itsPlane = createObjectFactory(ObjectType::PLANE);
    //std::unique_ptr<IGameObject> gameObj = std::make_unique<DerivedGameObject>();

    const std::string objectType = "plane";
    MyGameObjectFactory::registerObject(objectType,Plane::create);
    std::vector<std::unique_ptr<IGameObject>> objectColletion;
    objectColletion.emplace_back(std::move(MyGameObjectFactory::createObjectOnTheFly(objectType)));

    //ObjectFactory<IGameObject>::registerObject("plane",Plane::create);


    Adaptor<int,std::vector> a1;
    a1.push_back(10);

    std::cout<< a1.at(0)<<std::endl;
    
    MyMap<std::string,int> itsMap;

    itsMap["jolin"]=5;
    itsMap["jolin2"]=10;

    std::cout << " it MAP "<< itsMap["jolin"] << std::endl;
    std::cout << " it MAP "<< itsMap["jolin2"] << std::endl;

    vectormap<std::string,int> itsVectorMap;
    itsVectorMap["jolin"] = 5;
    itsVectorMap.insert({"jolin2",10});

    std::cout<<" its vector map "<< itsVectorMap["jolin"] << std::endl;
    std::cout<<" its vector map 2 "<< itsVectorMap["jolin2"] << std::endl;

    itsVectorMap.erase("jolin2");


    ObjectFactory<IGameObject>::registerObject("plane",Plane::create);


    std::cout<<" hello world "<< std::endl;

    return 0;
}