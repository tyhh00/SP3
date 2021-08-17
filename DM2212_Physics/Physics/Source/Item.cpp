
#include "Item.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Item::Item()
{
}

Item::Item(GROUP_TYPE _groupType, ITEM_TYPE _itemType, int _quantity)
	: groupType(_groupType)
	, itemType(_itemType)
	, quantity(_quantity)
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
