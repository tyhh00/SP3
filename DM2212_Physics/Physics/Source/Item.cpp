#include "Item.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Item::Item() 
	: scene(NULL)
{
}

Item::Item(GROUP_TYPE _groupType, ITEM_TYPE _itemType, int _quantity, bool _isStackable, Mesh* _mesh, float _durability)
	: groupType(_groupType)
	, itemType(_itemType)
	, quantity(_quantity)
	, isStackable(_isStackable)
	, scene(NULL)
	, mesh(_mesh)
	, durability(_durability)
	, isDurable(false)
{
}

Item::Item(GROUP_TYPE _groupType, ITEM_TYPE _itemType, int _quantity, bool _isStackable, float _durability)
	: groupType(_groupType)
	, itemType(_itemType)
	, quantity(_quantity)
	, isStackable(_isStackable)
	, scene(NULL)
	, durability(_durability)
	, isDurable(false)
{
}

Item::~Item()
{
}

void Item::Init()
{
	
}

/**
 @brief Returns a GROUP_TYPE enum for the item's group type
 */
Item::GROUP_TYPE Item::GetGroupType()
{
	return groupType;
}

/**
 @brief Returns an ITEM_TYPE enum for the item's type
 */
Item::ITEM_TYPE Item::GetType()
{
	return itemType;
}

/**
 @brief Returns item's quantity
 */
int Item::GetQuantity()
{
	return quantity;
}

/**
 @brief Set item's quantity
 @param _quantity An int to set the item's quantity
 */
void Item::SetQuantity(int _quantity)
{
	quantity = _quantity;
}

/**
 @brief Remove item's quantity, Returns the regative amt that was removed
 @param newItem An Item* which gets added into the inventory
 */
int Item::RemoveQuantity(int _quantity)
{
	//new qty to be added
	int newQuantity = quantity - _quantity;

	//if new qty lesser than 0, return the negative amt
	if (newQuantity < 0)
	{
		return newQuantity;
	}

	//else set qty to new qty
	quantity = newQuantity;
	return 0;
}

/**
 @brief Returns item's durability
 */
float Item::GetDurability()
{
	return durability;
}

void Item::SetDurability(float amt)
{
	durability = amt;
}

void Item::ReduceDurability(float amt)
{
	durability -= amt;
}

bool Item::GetIsStackable()
{
	return isStackable;
}

bool Item::GetIsDurable()
{
	return isDurable;
}

void Item::SetScene(SceneBase* scene)
{
	this->scene = scene;
}

Mesh* Item::GetItemMesh()
{
	return mesh;
}
