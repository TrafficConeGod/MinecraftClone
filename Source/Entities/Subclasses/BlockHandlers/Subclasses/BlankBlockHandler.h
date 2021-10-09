#pragma once
#include "BlockHandler.h"

class BlankBlockHandler : public virtual BlockHandler {
    public:
        GIVE_TYPE_ID_1(16, BlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(BlankBlockHandler)
        explicit BlankBlockHandler() {}
        virtual ~BlankBlockHandler() {}

        virtual bool IsTransparent(const Block&, const Block&) const override;
};