#include "Consumable.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Consumable::Consumable(ITEM_TYPE type, Mesh* _mesh) : Item(G_CONSUMABLE, type, 1, true, mesh)
{
	mesh = _mesh;
}

void Consumable::Init()
{
	
}

void Consumable::Update(double dt)
{

}
