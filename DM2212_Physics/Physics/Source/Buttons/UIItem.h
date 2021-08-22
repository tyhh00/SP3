#pragma once

/******************************************************************************/
/*!
\file	UIItem.h
\author Tan Yong Hong
\brief
Used by Buttons and Text Objects to store their attributes.
*/
/******************************************************************************/

/******************************************************************************/
/*!
		Class UIItem:
\brief Used by Buttons and Text Objects to store their attributes.
*/
/******************************************************************************/
struct UIItem {
	float originX, originY;
	float sizeX, sizeY;

	UIItem() {}

	UIItem(UIItem& item) {
		this->originX = item.originX;
		this->originY = item.originY;
		this->sizeX = item.sizeX;
		this->sizeY = item.sizeY;
	}
};