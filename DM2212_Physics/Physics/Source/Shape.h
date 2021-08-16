#ifndef SHAPE_H
#define SHAPE_H

//Used for Physics Collision
//Original Solution was use GameObject_Type to figure out the shape. I.e. GO_WALL = RECTANGLE so calc base on rectangle shape
//But this meant RenderGO() we had to do switch case base on GameObject_Type. If we have 800 GameObject_Types, we can't
//possibly do a 800 case switch case so we need to move the GEOMETRY_TYPE Mesh into GameObject class itself and call
//go->RenderMesh();
enum SHAPE_TYPE
{
	RECTANGLE,
	CIRCLE,
};

#endif