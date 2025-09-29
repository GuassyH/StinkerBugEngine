#ifndef GIZMO_BEHAVIOUR_H
#define GIZMO_BEHAVIOUR_H


class GizmoBehaviour {
public:
	GizmoBehaviour() = default;
	virtual ~GizmoBehaviour() = default;

	virtual void Init() {}
	virtual void Update() {}
	
};


#endif