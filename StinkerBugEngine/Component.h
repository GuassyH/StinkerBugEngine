#ifndef ComponentCore_H
#define ComponentCore_H

#include "ComponentCore.h"

#include "Transform.h"

class Component : public ComponentCore {
public:	
	std::shared_ptr<Transform> transform = nullptr;
};



#endif