#ifndef COMPONENT_H
#define COMPONENT_H


class Component {
public:	
	Component() = default;
	virtual ~Component() = default;
	virtual void DrawInInspector() {}
};



#endif