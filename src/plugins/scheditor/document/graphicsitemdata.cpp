#include "graphicsitemdata.h"

GraphicsItemData::GraphicsItemData(SchDocumentObject *parent):
    SchDocumentObject(parent), m_locked(false), m_visible(true), m_hMirrored(false), m_vMirrored(false)
{

}

