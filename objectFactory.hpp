#ifndef OBJECTFACTORY_HPP
#define OBJECTFACTORY_HPP

#include <map>
#include <unordered_map>
#include "vectormap.hpp"


template<typename T>
class ObjectFactory {
    public:
        using callbackType = std::unique_ptr<T>;
        using callback = callbackType (*)(); // #TODO compare this with std::function

        static void registerObject(const std::string& type, callback callback_){
            s_objects.insert({type,callback_});
        }

        static void unregistObject(const std::string& type){
            s_objects.erase(type);
        }

        static callbackType createObjectOnTheFly(const std::string& type){
            auto iterator = s_objects.find(type);
            if(iterator!=s_objects.end()){
                callback createObject = iterator->second;
                return createObject(); // invoke function which returns IGameObject*
            }
            throw std::runtime_error("No callback function found for object type" + type);
        }

    private:
        //typedef std::map<std::string,CreateObjectCallback> callbackMap;
        //using callbackMap = std::unordered_map<std::string,callback>;
        using callbackMap = vectormap<std::string,callback>; 
        static callbackMap s_objects;

};


// template <typename T>
// std::unordered_map<std::string, typename ObjectFactory<T>::callback> ObjectFactory<T>::s_objects;

template <typename T>
vectormap<std::string, typename ObjectFactory<T>::callback> ObjectFactory<T>::s_objects;


#endif 