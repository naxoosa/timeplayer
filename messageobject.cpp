#include "messageobject.h"

MessageObject::MessageObject(QObject *parent) : QObject(parent)
{

}

void MessageObject::setType(QString type){
    this->type = type;
}

QString MessageObject::getType(){
    return this->type;
}

void MessageObject::setAction(QString action){
    this->action = action;
}

QString MessageObject::getAction(){
    return this->action;
}

