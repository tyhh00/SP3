#include "Item.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Item::Item()
{
}

Item::Item(GROUP_TYPE _groupType, ITEM_TYPE _itemType, int _quantity, int _maxQuantity, bool _isStackable)
	: groupType(_groupType)
	, itemType(_itemType)
	, quantity(_quantity)
	, maxQuantity(_maxQuantity)
	, isStackable(_isStackable)
{
}

Item::~Item()
{
}

void Item::Init()
{
	
}

void Item::Update(double dt)
{
	
}

Item::GROUP_TYPE Item::GetGroupType()
{
	return groupType;
}

Item::ITEM_TYPE Item::GetType()
{
	return itemType;
}

int Item::GetQuantity()
{
	return quantity;
}

void Item::SetQuantity(int _quantity)
{
	quantity = _quantity;
}

int Item::AddQuantity(int _quantity)
{
	//new qty to be added
	int newQuantity = quantity + _quantity;

	//if new qty exceeds max qty, return remainder
	if (newQuantity > maxQuantity)
	{
		int remainderQuantity = newQuantity - maxQuantity;
		return remainderQuantity;
	}

	//else set qty to new qty
	quantity = newQuantity;
	return 0;
}

int Item::RemoveQuantity(int _quantity)
{
	//new qty to be added
	int newQuantity = quantity - _quantity;

	//if new qty lesser than 0, return new qty
	if (newQuantity < 0)
	{
		return newQuantity;
	}

	//else set qty to new qty
	quantity = newQuantity;
	return 0;
}

int Item::GetMaxQuantity()
{
	return maxQuantity;
}

void Item::SetMaxQuantity(int _maxQuantity)
{
	maxQuantity = _maxQuantity;
}

bool Item::GetIsStackable()
{
	return isStackable;
}
